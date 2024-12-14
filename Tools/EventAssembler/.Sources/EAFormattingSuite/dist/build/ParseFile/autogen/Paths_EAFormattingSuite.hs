{-# LANGUAGE CPP #-}
{-# LANGUAGE NoRebindableSyntax #-}
{-# OPTIONS_GHC -fno-warn-missing-import-lists #-}
module Paths_EAFormattingSuite (
    version,
    getBinDir, getLibDir, getDynLibDir, getDataDir, getLibexecDir,
    getDataFileName, getSysconfDir
  ) where

import qualified Control.Exception as Exception
import Data.Version (Version(..))
import System.Environment (getEnv)
import Prelude

#if defined(VERSION_base)

#if MIN_VERSION_base(4,0,0)
catchIO :: IO a -> (Exception.IOException -> IO a) -> IO a
#else
catchIO :: IO a -> (Exception.Exception -> IO a) -> IO a
#endif

#else
catchIO :: IO a -> (Exception.IOException -> IO a) -> IO a
#endif
catchIO = Exception.catch

version :: Version
version = Version [1,0] []
bindir, libdir, dynlibdir, datadir, libexecdir, sysconfdir :: FilePath

bindir     = "/mnt/c/Users/Owner/Desktop/Gaming/Nintendo/Game_Boy_Advance/FE8/fe8u-cskillsys-kernel/Tools/EventAssembler/Tools"
libdir     = "/home/username/.cabal/lib/x86_64-linux-ghc-8.8.4/EAFormattingSuite-1.0-IPrNphCJghCJm5m3aAmNp8"
dynlibdir  = "/home/username/.cabal/lib/x86_64-linux-ghc-8.8.4"
datadir    = "/home/username/.cabal/share/x86_64-linux-ghc-8.8.4/EAFormattingSuite-1.0"
libexecdir = "/home/username/.cabal/libexec/x86_64-linux-ghc-8.8.4/EAFormattingSuite-1.0"
sysconfdir = "/home/username/.cabal/etc"

getBinDir, getLibDir, getDynLibDir, getDataDir, getLibexecDir, getSysconfDir :: IO FilePath
getBinDir = catchIO (getEnv "EAFormattingSuite_bindir") (\_ -> return bindir)
getLibDir = catchIO (getEnv "EAFormattingSuite_libdir") (\_ -> return libdir)
getDynLibDir = catchIO (getEnv "EAFormattingSuite_dynlibdir") (\_ -> return dynlibdir)
getDataDir = catchIO (getEnv "EAFormattingSuite_datadir") (\_ -> return datadir)
getLibexecDir = catchIO (getEnv "EAFormattingSuite_libexecdir") (\_ -> return libexecdir)
getSysconfDir = catchIO (getEnv "EAFormattingSuite_sysconfdir") (\_ -> return sysconfdir)

getDataFileName :: FilePath -> IO FilePath
getDataFileName name = do
  dir <- getDataDir
  return (dir ++ "/" ++ name)
