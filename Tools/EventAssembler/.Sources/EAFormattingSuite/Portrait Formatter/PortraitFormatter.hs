{-
Formamts a mug from the now-conventional portrait format to the format actually used in-game,
then converts that to a palette-map used by the GBA.

By: Crazycolorz5
-}
{-# LANGUAGE TypeFamilies #-}
import GBALZ77
import GBAGraphics
import GBAUtilities (stripExtension, padTo4, makeError)
import FlagUtilities

import Data.ByteString hiding (take, dropWhile, head, tail, reverse, elem)
import System.Environment (getArgs)
import System.IO (openFile, IOMode (WriteMode), hClose, stdout)
import qualified Data.ByteString.Lazy as Lazy

--Dependant on JuicyPixels
--https://hackage.haskell.org/package/JuicyPixels
import Codec.Picture
import Codec.Picture.Types

import Control.Monad.IO.Class
import Control.Monad.Trans.Except
--emptyFormatImage::IO (MutableImage (PrimState IO) PixelRGBA8)
--emptyFormatImage = createMutableImage 256 32 (PixelRGBA8 0 0 0 0) --Image {imageWidth = 256, imageHeight = 32, imageData = empty}

main = do
    args <- getArgs
    let optFlags = getOptions args
    let toStdOut = Prelude.elem "--to-stdout" optFlags
    result <- mainAction args
    case result of
        Left err -> Prelude.putStr $ makeError toStdOut err
        Right _ -> return ()

mainAction :: [String] -> IO (Either String ())
mainAction args = runExceptT $ do
    let optFlags = getOptions args
    let unflaggedParams = getUnflaggedParams args
    let outputImages = Prelude.elem "--output-png" optFlags
    let toStdOut = Prelude.elem "--to-stdout" optFlags
    if Prelude.elem "--help" optFlags
    then throwE "usage: ./PortraitFormatter <filename.png> [-o <outputfilename.dmp>] [--output-png] [--to-stdout] [--help]"
    else if Prelude.length unflaggedParams /= 1
    then throwE "Incorrect number of parameters! Use ./PortraitFormatter --help for usage."
    else if elem "-o" args && getParamAfterFlag "-o" args == Nothing
    then throwE "No output file specified."
    else do
        let inputFileName = head unflaggedParams
        dynamicCinnamonRoll <- ExceptT $ readPng inputFileName
        let nonDynamicCinnamonRoll = case dynamicCinnamonRoll of ImageRGBA8 cinnamonRoll -> (pixelMap dropTransparency cinnamonRoll); ImageRGB8 cinnamonRoll -> cinnamonRoll; otherwise -> convertRGB8 dynamicCinnamonRoll
        let (palette, paletteIndices) = gbaPalettize nonDynamicCinnamonRoll
        let transparentColorIndex = pixelAt paletteIndices 0 0 -- Upper left pixel is transparent color
        let transparentColor = pixelAt nonDynamicCinnamonRoll 0 0 -- Upper left pixel is transparent color

        formattedMugImmutable <- liftIO $ do
            emptyMugImage <- newMutableImage 256 32
            formattedMug <-
                copySectionFromTo 2 0 paletteIndices 0 0 8 4 emptyMugImage >>= --red
                copySectionFromTo 2 4 paletteIndices 8 0 8 4 >>= --yellow
                copySectionFromTo 2 8 paletteIndices 16 0 4 2 >>= --green
                copySectionFromTo 6 8 paletteIndices 16 2 4 2 >>= --cyan
                copySectionFromTo 0 6 paletteIndices 20 0 2 4 >>= --blue
                copySectionFromTo 10 6 paletteIndices 22 0 2 4 >>= --magenta
                copySectionFromTo 12 6 paletteIndices 24 0 4 2 >>= --dim red
                copySectionFromTo 12 8 paletteIndices 24 2 4 2 >>= --dim yellow
                copySectionFromTo 12 10 paletteIndices 28 0 4 2 >>= --dim green
                copySectionFromTo 12 12 paletteIndices 28 2 4 2 --white
            freezeImage formattedMug

        formattedFramesImmutable <- liftIO $ do
            emptyFramesImage <- newMutableImage 384 8
            formattedFrames <-
                copySectionFromTo 0 10 paletteIndices 0 0 4 1 emptyFramesImage >>= --red
                copySectionFromTo 0 11 paletteIndices 4 0 4 1 >>= --dark red
                copySectionFromTo 4 10 paletteIndices 8 0 4 1 >>= --yellow
                copySectionFromTo 4 11 paletteIndices 12 0 4 1 >>= --dark yellow
                copySectionFromTo 8 10 paletteIndices 16 0 4 1 >>= --green
                copySectionFromTo 8 11 paletteIndices 20 0 4 1 >>= --dark green
                copySectionFromTo 0 12 paletteIndices 24 0 4 1 >>= --cyan
                copySectionFromTo 0 13 paletteIndices 28 0 4 1 >>= --dark cyan
                copySectionFromTo 4 12 paletteIndices 32 0 4 1 >>= --blue
                copySectionFromTo 4 13 paletteIndices 36 0 4 1 >>= --dark blue
                copySectionFromTo 8 12 paletteIndices 40 0 4 1 >>= --magenta
                copySectionFromTo 8 13 paletteIndices 44 0 4 1 --dark magenta
            freezeImage formattedFrames

        croppedMinimugImageImmutable <- liftIO $ newMutableImage 32 32 >>= copySectionFromTo 12 2 paletteIndices 0 0 4 4 >>= freezeImage

        let swapToTransparent = pixelMap (\pix->if pix == transparentColorIndex then 0 else if pix == 0 then transparentColorIndex else pix)
        --Swap transparent and non-transparent color
        let properlyTransparentMug = swapToTransparent formattedMugImmutable

        --Swap transparent and non-transparent color
        let properlyTransparentFrames = swapToTransparent formattedFramesImmutable

        --Chibi image
        let properlyTransparentMinimug = swapToTransparent croppedMinimugImageImmutable

        --Swap it within the palette
        let properlyTransparentPalette = generateImage (\x->(\y->(let thisPx = pixelAt palette x y in if thisPx == transparentColor then pixelAt palette 0 0 else if thisPx == pixelAt palette 0 0 then transparentColor else thisPx))) (imageWidth palette) (imageHeight palette)

        let mugOutput = addMugHeader . toROMFormat $ properlyTransparentMug
        let paletteOutput = toROMPalette properlyTransparentPalette
        let framesOutput = toROMFormat properlyTransparentFrames
        let minimugFormat = toROMFormat properlyTransparentMinimug
        let minimugOutput = padTo4 . gbaLZ77Compress $ minimugFormat
        let outputFileNameMaybe = getParamAfterFlag "-o" args
        maybe (return ()) (\outputFileName -> liftIO $ do
                outputHandle <- openFile outputFileName WriteMode
                hPut outputHandle mugOutput
                hPut outputHandle framesOutput
                hPut outputHandle paletteOutput
                hPut outputHandle minimugOutput
                hClose outputHandle) outputFileNameMaybe

        if toStdOut
        then liftIO $ do
            hPut stdout mugOutput
            hPut stdout framesOutput
            hPut stdout paletteOutput
            hPut stdout minimugOutput
        else if not (elem "-o" args)
        then liftIO $ do
            outputMug <- openFile (stripExtension inputFileName ++ "_mug.dmp") WriteMode
            hPut outputMug mugOutput
            hClose outputMug

            outputPal <- openFile (stripExtension inputFileName ++ "_palette.dmp") WriteMode
            hPut outputPal paletteOutput
            hClose outputPal

            outputFrames <- openFile (stripExtension inputFileName ++ "_frames.dmp") WriteMode
            hPut outputFrames framesOutput
            hClose outputFrames

            outputMinimug <- openFile (stripExtension inputFileName ++ "_minimug.dmp") WriteMode
            hPut outputMinimug minimugOutput
            hClose outputMinimug
        else return ()

        {-
        outputMinimugUncomp <- openFile (stripExtension inputFileName ++ "_debug.dmp") WriteMode
        hPut outputMinimugUncomp minimugOutputUnComp
        hClose outputMinimugUncomp
        -}

        --For debugging/visual display/bragging, just output the formatted file
        if outputImages
        then do
            inds <- ExceptT . return $ encodePalettedPng properlyTransparentPalette properlyTransparentMug
            liftIO $ do
                outputPng <- openFile ("FormattedMug_"++inputFileName) WriteMode
                Lazy.hPut outputPng inds
                hClose outputPng

            framesInds <- ExceptT . return $ encodePalettedPng properlyTransparentPalette properlyTransparentFrames
            liftIO $ do
                outputPngFrames <- openFile ("FormattedFrames_"++inputFileName) WriteMode
                Lazy.hPut outputPngFrames framesInds
                hClose outputPngFrames

            liftIO $ writePng ("FormattedPalette_"++inputFileName) properlyTransparentPalette
            return ()
        else return ()


addMugHeader::ByteString -> ByteString
addMugHeader = append (pack [0x00, 0x04, 0x10, 0x00])
