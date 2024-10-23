# Input: Script. Also uses frames (PNGs) mentioned in script.
# Output: Installer EA file.
# More details in the README.
# 
# Many thanks to 7743's FEBuilderGBA. This implementation
# started out as a rewrite of FEBuilderGBA's implementation.
# I just added some more bells & whistles for personal use.
# I also referenced circleseverywhere's Animation Assembler a lot,
# so many thanks to them too!
import os
import math as m
import time
import argparse
import lzss
import hashlib
from PIL import Image
from PIL import ImageChops

# Initialized to False. Can be set by extra setting in battle anim script.
# If True, palettedata will not be compressed. This can be useful if trying
# to display >2 battle animations.
UNCOMPPALDATA = False

# Initialized to False. Can be set by extra setting in battle anim script.
# If True, framedata will not be compressed. This can be useful if trying
# to display >2 battle animations, as it can be hard to find the free RAM
# to decompress framedata to.
UNCOMPFRAMEDATA = False

# Initialized to False. Can be set by extra setting in battle anim script.
# If True, both ltr- and rtloamdata will not be compressed. This can be
# useful if trying to display >2 battle animations, as it can be hard to
# find the free RAM to decompress oamdata to.
UNCOMPOAMDATA = False

# Initialized to False. Can be set by extra setting in battle anim script.
# If True, sprite sheets will be  one-dimensional.
# Normally, either OAM can be conserved at the cost of more sheets,
# (which costs more ROM and leads to more decompression in equal time)
# or more OAM will be consumed in exchange for fewer sheets.
# 1-dimensional sprites can conserver OAM and sheets, however,
# all other vanilla sprites in battle anims are in 2dimensional
# and would need re-doing to not get scrambled in 1-dim sprite mode.
# In my, admittedly limited, testing earlier frames on 1-dim sheets
# still consume the same amount of sprites, whereas later ones
# consume fewer (makes sense in theory, I guess).
_1D_SPRITES = False

# Initialized to False. Can be set by extra setting in battle anim script.
# If true, sprites will not be split into smaller chunks if there's otherwise no room
# left on the current sheet. Instead a new sheet will be created. More sheets means 
# more ROMcost and more frequent decompression, but fewer sprites.
# This flag is ignored if 1D sprites are turned on, as it's functionally pointless.
NOSPLITSPRITES = False

# Initialized to 0. Can be set by extra setting in battle anim script.
# Indicates how many empty tiles will be allowed in 64-tiled sprite.
# This value is used for both 1D and 2D spritesheets.
# Divide this value by two to get how many empty tiles will be allowed in a
# 32-tiled sprite, etc. The more empty tiles are allowed in a sprite,
# the fewer sprites are necessary to represent a frame.
# This provides the same advantages and disadvantages as NOSPLITSPRITES.
EMPTYTILEPER64 = 0

# Initialized to False. Can be set by extra setting in battle anim script.
# If this is enabled, frames will be split in two, each using a different palette.
# This way it's possible to display two palettes at the cost of two free paletteslots,
# and more sprites (meaning more sheets, therefore more ROM consumption and more frequent
# decompression.
_2PALETTES = False

# Initialized to False. Can be set by extra setting in battle anim script.
# If this is enabled, sheets will only be half the usual size (256x32 instead of 256x64).
# This allows for more battle anims to display at once (pair-up here we go?).
# The split is horizontal (256x32 instead of 128x64) to avoid having to decompress the sheet
# elsewhere before then moving it to VRAM. This does mean 64x64 and 32x64 sprites won't fit
# and will be split into smaller sprites.
HALFSIZESHEETS = False

# If PATCH_AutoGenLeftOAM is installed, then we need not to load _ltr
LEFTOAM_NOLOAD = False

# I like to see how hash collisions happen.
HASHCOLLISIONS = 0
IMGCOMPARISONCOUNT = 0

SHEET_WIDTH_PIXELS = 0
SHEET_HEIGHT_PIXELS = 0
SHEET_WIDTH_TILES = 0
SHEET_HEIGHT_TILES = 0
SHEET_SIZE_PIXELS = 0
SHEET_SIZE_TILES = 0
FRAME_WIDTH_PIXELS = 240
FRAME_WIDTH_TILES = FRAME_WIDTH_PIXELS>>3
FRAME_HEIGHT_PIXELS = 160
FRAME_HEIGHT_TILES = FRAME_HEIGHT_PIXELS>>3
FRAME_SIZE_PIXELS = FRAME_WIDTH_PIXELS * FRAME_HEIGHT_PIXELS
FRAME_SIZE_TILES = FRAME_WIDTH_TILES * FRAME_HEIGHT_TILES

# Is there a smarter way of doing this? Probably. Can I be bothered? Definitely not.
hexSet = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f'}

# Used when splitting sprites to obtain a list of possible smaller dimensions.
# The order of the tuple-elements is chosen to avoid a concave shape when picking
# spritechunks. This means we can safely assume some useful things.
spriteCropDims = {
  (8, 8): ((4,8), (8,4), (4,4), (2,4), (1,4), (4,2), (2,2), (1,2), (4,1), (2,1), (1,1)),
  (4, 8): ((4,8), (4,4), (2,4), (1,4), (4,2), (2,2), (1,2), (4,1), (2,1), (1,1)),
  (2, 8): ((2,4), (1,4), (2,2), (1,2), (2,1), (1,1)),
  (1, 8): ((1,4), (1,2), (1,1)),
  (8, 4): ((8,4), (4,4), (2,4), (1,4), (4,2), (2,2), (1,2), (4,1), (2,1), (1,1)),
  (8, 2): ((4,2), (2,2), (1,2), (4,1), (2,1), (1,1)),
  (8, 1): ((4,1), (2,1), (1,1)),
  (4, 4): ((4,4), (2,4), (1,4), (4,2), (2,2), (1,2), (4,1), (2,1), (1,1)),
  (2, 4): ((2,4), (1,4), (2,2), (1,2), (2,1), (1,1)),
  (1, 4): ((1,4), (1,2), (1,1)),
  (4, 2): ((4,2), (2,2), (1,2), (4,1), (2,1), (1,1)),
  (2, 2): ((2,2), (1,2), (2,1), (1,1)),
  (1, 2): ((1,2), (1,1)),
  (4, 1): ((4,1), (2,1), (1,1)),
  (2, 1): ((2,1), (1,1)),
  (1, 1): ((1,1),)
}

# Takes a bytes object and returns BYTE AA BB CC DD
def b_to_EA(data):
  return 'BYTE ' + ' '.join([hex(x) for x in data])

# Takes an int and size and returns EA format.
def int_to_EA(data, size):
  return b_to_EA(data.to_bytes(size, byteorder='little'))

# Taken from https://stackoverflow.com/a/49692185
# Takes an image and returns a (smaller) value corresponding to it.
# Beware: different images can map to the same hash.
def hashImage(img):
  #img = img.resize((10, 10), Image.ANTIALIAS)
  img = img.convert("L")
  pixel_data = list(img.getdata())
  avg_pixel = sum(pixel_data)/len(pixel_data)
  #return avg_pixel
  bits = "".join(['1' if (px >= avg_pixel) else '0' for px in pixel_data])
  return str(hex(int(bits, 2)))[2:][::-1].upper()

# Used to compare images if hashes match.
# Increments HASHCOLLISIONS if images are different. 
def compareImages(img1, img2):
  global HASHCOLLISIONS
  global IMGCOMPARISONCOUNT
  IMGCOMPARISONCOUNT += 1
  if not ImageChops.difference(img1, img2).getbbox():
    return True
  HASHCOLLISIONS += 1
  return False

# Used to separate palettes if _2PALETTES is set.
def maskPal0(pixel):
  return 0 if pixel > 15 else pixel

# Used to separate palettes if _2PALETTES is set.
def maskPal1(pixel):
  return 0 if pixel < 16 else (pixel&0xF)

class Frame:
  def __init__(self, name, frameImage, index, sheets, oamOffset):
    self.index = index
    self.name = name
    self.palette = frameImage.getpalette()
    self.spriteChunks = list()
    self.spriteChunksP = list()
    self.sheetIndex = 0
    
    # Mark all tiles that have at least one nontransparent pixel.
    spriteChunks = self.spriteChunks
    emptyTile = Image.new(mode='P', size=(8, 8))
    frameImageCrop = frameImage.crop((0, 0, 240, 160))
    frameImageCropMasked = Image.eval(frameImageCrop, maskPal0)
    palID = 0
    finished2ndpal = not _2PALETTES
    finishedPierce = False
    
    # Execute loop twice for pierce frames, once otherwise.
    while True:
      frameOccupiedTiles = [False for i in range(FRAME_SIZE_TILES)]
      for i in range(FRAME_SIZE_TILES):
        a = (i%FRAME_WIDTH_TILES)<<3
        b = (int(i/FRAME_WIDTH_TILES))<<3
        tile = frameImageCropMasked.crop((a, b, a+8, b+8))
        diff = ImageChops.difference(emptyTile, tile)
        if diff.getbbox():      # Check if tile differs from empty tile.
          frameOccupiedTiles[i] = True
      
      # Split image into sprite chunks.
      frameTileID = -1
      while frameTileID+1 < (FRAME_SIZE_TILES):
        frameTileID += 1
        if frameOccupiedTiles[frameTileID]:
          (findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 8, 8, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 8, 4, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 4, 8, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 4, 4, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 4, 2, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 2, 4, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 2, 2, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 4, 1, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 1, 4, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 2, 1, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 1, 2, palID) or
           findChunk(frameImageCropMasked, spriteChunks, frameOccupiedTiles, frameTileID, 1, 1, palID))
      
      if finished2ndpal:
        palID = 0
      else:
        finished2ndpal = True
        frameImageCropMasked = Image.eval(frameImageCrop, maskPal1)
        palID = 1
        continue
      
      if finishedPierce:
        break
      elif frameImage.size == (488, 160):
        frameImageCrop = frameImage.crop((240, 0, 480, 160))
        frameImageCropMasked = Image.eval(frameImageCrop, maskPal0)
        spriteChunks = self.spriteChunksP
        finishedPierce = True
        finished2ndpal = not _2PALETTES
      else:
        break
    self.parseFrameIntoSheet(sheets)
    
    # Set OAM offsets.
    if not len(self.spriteChunks):
      self.oamOffset1 = 0
    else:
      self.oamOffset1 = oamOffset
    
    if not len(self.spriteChunksP):
      self.oamOffsetP = 0
    else:
      if self.oamOffset1:
        self.oamOffsetP = self.oamOffset1 + (len(self.spriteChunks)+1)*12
      else:
        self.oamOffsetP = oamOffset
  
  # Parse frame into a sheet.
  # Input: array of sheets (ref), frame.
  # Returns: nothing. Array of sheets may have been extended. Frame's spriteChunks(P) may have changed.
  def parseFrameIntoSheet(self, sheets):
    totalNewSpriteChunks1 = list()
    totalNewSpriteChunksP = list()
    totalNewSpriteChunks = totalNewSpriteChunks1
    spriteChunks = self.spriteChunks
    finished = False
    
    # Try existing sheets.
    index = -1
    for index, sheet in enumerate(sheets):
      copySheet = Sheet()
      copySheet.copy(sheet)
      while True:
        for spriteChunk in spriteChunks:
          (success, newSpriteChunks) = copySheet.addSpriteChunk(spriteChunk, NOSPLITSPRITES)
          if not success:
            finished = False
            totalNewSpriteChunks = totalNewSpriteChunks1
            spriteChunks = self.spriteChunks
            totalNewSpriteChunks1.clear()
            totalNewSpriteChunksP.clear()
            break
          totalNewSpriteChunks.extend(newSpriteChunks)
        else:
          if finished:    # Return
            sheets[index] = copySheet
            self.sheetIndex = index
            self.spriteChunks = totalNewSpriteChunks1
            self.spriteChunksP = totalNewSpriteChunksP
            return
          else:   # Pierce frame.
            finished = True
            totalNewSpriteChunks = totalNewSpriteChunksP
            spriteChunks = self.spriteChunksP
            continue
        break
      continue
      
    # No room in any existing sheet.
    # We create a new sheet.
    totalNewSpriteChunks = totalNewSpriteChunks1
    spriteChunks = self.spriteChunks
    finished = False
    sheet = Sheet(self.palette)
    while True:
      for spriteChunk in spriteChunks:
        (success, newSpriteChunks) = sheet.addSpriteChunk(spriteChunk, NOSPLITSPRITES)
        if not success:
          print("ERROR: frame: " + self.name + ", image too big; Doesn't fit on a single sheet.")
          exit()
        totalNewSpriteChunks.extend(newSpriteChunks)
      if finished:
        sheets.append(sheet)
        self.sheetIndex = index + 1
        self.spriteChunks = totalNewSpriteChunks1
        self.spriteChunksP = totalNewSpriteChunksP
        return
      else:   # Pierce frame.
        finished = True
        totalNewSpriteChunks = totalNewSpriteChunksP
        spriteChunks = self.spriteChunksP
  
# If w by h chunk at tileID contains no more
# empty tiles than is allowed (see EMPTYTILEPER64),
# adds it to the spriteChunks list and returns True.
# Otherwise, returns false.
def findChunk(image, spriteChunks, occupiedTiles, tileID, w, h, palID):
  if (HALFSIZESHEETS and (not _1D_SPRITES) and h > 4):
    return False
  emptyTileCount = EMPTYTILEPER64 >> (6 - int(m.log(w*h, 2)))
  occupiedTilesCopy = occupiedTiles.copy()
  x = tileID % FRAME_WIDTH_TILES
  y = int(tileID / FRAME_WIDTH_TILES)
  if x+w < FRAME_WIDTH_TILES and y+h < FRAME_HEIGHT_TILES:
    if _1D_SPRITES:
      spriteChunkImage = Image.new(mode='P', size=((w*h)<<3, 8))
    else:
      spriteChunkImage = Image.new(mode='P', size=(w<<3, h<<3))
    spriteChunkImage.putpalette(image.getpalette())
    i = 0   # Keeps track of each width-by-8 chunk added to the spriteChunk.
    j = 0   # Also keeps track of each width-by-8 chunk added to the spriteChunk.
    for k in range(tileID, tileID + FRAME_WIDTH_TILES*h, FRAME_WIDTH_TILES):
      for l in range(k, k+w):
        if not occupiedTiles[l]:
          if emptyTileCount > 0:
            emptyTileCount -= 1
          else:
            break
        occupiedTilesCopy[l] = False
      else:
        y2 = int(k / FRAME_WIDTH_TILES)<<3
        spriteChunkChunkImage = image.crop((x<<3, y2, (x<<3)+(w<<3), y2+8))
        if _1D_SPRITES:
          spriteChunkImage.paste(spriteChunkChunkImage, (i, 0))
        else:
          spriteChunkImage.paste(spriteChunkChunkImage, (0, j))
        i += (w<<3)
        j += 8
        continue
      break
    else:
      # Chunk fits. Mark used tiles as unoccupied.
      for id, bool in enumerate(occupiedTilesCopy):
        occupiedTiles[id] = bool
      spriteChunks.append(SpriteChunk(spriteChunkImage, x, y, w, h, palID))
      return True
  return False

class Sheet:
  def __init__(self, palette=False):
    self.image = Image.new(mode='P', size=(SHEET_WIDTH_PIXELS, SHEET_HEIGHT_PIXELS))
    if palette:
      self.image.putpalette(palette)
    self.spriteChunks = list()
    self.occupiedTiles = [False for i in range(SHEET_SIZE_TILES)]
    self.freeTileCount = SHEET_SIZE_TILES     # Not used for 1D spritesheets.
  
  # NOTE: contents of "copy" argument are copied to "self"!
  def copy(self, copy):
    self.image = copy.image.copy()
    self.image.putpalette(copy.image.getpalette())
    self.spriteChunks = copy.spriteChunks.copy()
    self.occupiedTiles = copy.occupiedTiles.copy()
    self.freeTileCount = copy.freeTileCount
    return
  
  # Return true if spriteChunk is already in sheet
  # or has been added to sheet. Return false otherwise.
  # Also return list of spritechunks used this call (used by frame).
  # If not noSplitSprites, spriteChunk will be split if it
  # can't fit in sheet otherwise (not relevant for 1D sheets).
  # Also sets spriteChunk's OAM2.
  def addSpriteChunk(self, newSpriteChunk, noSplitSprites):
    global HASHCOLLISIONS
    totalNewSpriteChunks = list()
    tileOffset = 0
    for spriteChunk in self.spriteChunks:
      if (newSpriteChunk.w == spriteChunk.w and
          newSpriteChunk.h == spriteChunk.h and
          spriteChunk.hash == newSpriteChunk.hash and
          compareImages(spriteChunk.image, newSpriteChunk.image)):
        rtlOAM2 = int.from_bytes(spriteChunk.rtlOAM2, byteorder='little')
        ltrOAM2 = int.from_bytes(spriteChunk.ltrOAM2, byteorder='little')
        rtlOAM2 |= (newSpriteChunk.palID<<12)
        ltrOAM2 |= (newSpriteChunk.palID<<12)
        newSpriteChunk.rtlOAM2 = rtlOAM2.to_bytes(2, byteorder='little')
        newSpriteChunk.ltrOAM2 = ltrOAM2.to_bytes(2, byteorder='little')
        totalNewSpriteChunks.append(newSpriteChunk)
        break
      tileOffset += (spriteChunk.image.width>>3)
    else:   # Chunk not already in sheet.
    
      if not _1D_SPRITES:
        # Check if we can find a width by height block in the sheet.
        sheetCopy = Sheet()
        sheetCopy.copy(self)
        for h in range(SHEET_HEIGHT_TILES):
          if h+newSpriteChunk.h > SHEET_HEIGHT_TILES:
            break
          for w in range(SHEET_WIDTH_TILES):
            if w+newSpriteChunk.w > SHEET_WIDTH_TILES:
              break
            start = w + h*SHEET_WIDTH_TILES
            stop = start + newSpriteChunk.h*SHEET_WIDTH_TILES
            for i in range(start, stop, SHEET_WIDTH_TILES):
              for j in range(i, i+newSpriteChunk.w):
                if sheetCopy.occupiedTiles[j]:
                  sheetCopy.occupiedTiles = self.occupiedTiles.copy()
                  break
                sheetCopy.occupiedTiles[j] = True
              else:
                continue
              break
            else:
              # Found an empty block! Add newSpriteChunk and return True.
              self.occupiedTiles = sheetCopy.occupiedTiles.copy()
              self.freeTileCount -= (newSpriteChunk.w*newSpriteChunk.h)
              newSpriteChunk.rtlOAM2 = (start | (newSpriteChunk.palID<<12)).to_bytes(2, byteorder='little')
              newSpriteChunk.ltrOAM2 = (start | (newSpriteChunk.palID<<12)).to_bytes(2, byteorder='little')
              self.spriteChunks.append(newSpriteChunk)
              totalNewSpriteChunks.append(newSpriteChunk)
              self.image.paste(newSpriteChunk.image, (w<<3, h<<3))
              return True, totalNewSpriteChunks
        # Didn't find an empty block.
        if noSplitSprites:
          return False, list()
        # Try again by splitting spriteChunk.
        occupiedTiles = [True for i in range(newSpriteChunk.w*newSpriteChunk.h)]
        for i in range(newSpriteChunk.h):
          for j in range(newSpriteChunk.w):
            if not occupiedTiles[j+i*newSpriteChunk.w]:
              continue
            # Looping from left-to-right and top-to-bottom means that if the current tile is occupied,
            # so are all tiles to the right and below of this tile. This assumption requires spriteCropDims'
            # tuple order to be a certain way, to avoid a concave shape from forming.
            w = 2**int(m.log(newSpriteChunk.w-j,2))     # Map non-powers of two to
            h = 2**int(m.log(newSpriteChunk.h-i,2))     # the next lower power of two.
            for splitDims in spriteCropDims[(w, h)]:
              image = newSpriteChunk.image.crop((j<<3,i<<3,(j+splitDims[0])<<3,(i+splitDims[1])<<3))
              y = newSpriteChunk.y + i
              x = newSpriteChunk.x + j
              sheetCopy.copy(self)
              (success, newSpriteChunks) = sheetCopy.addSpriteChunk(SpriteChunk(image, x, y, splitDims[0], splitDims[1], newSpriteChunk.palID), True)
              if success:
                totalNewSpriteChunks.extend(newSpriteChunks)
                self.copy(sheetCopy)
                for k in range(i, i+splitDims[1]):
                  for l in range(j+k*newSpriteChunk.w, j+k*newSpriteChunk.w+splitDims[0]):
                    occupiedTiles[l] = False
                break
            else:
              return False, list()

      else:
        # 1D sprites. Check if sheet has enough room.
        if tileOffset + (newSpriteChunk.image.width>>3) >= SHEET_SIZE_TILES:
          return False, list()
        newSpriteChunk.rtlOAM2 = (tileOffset | (newSpriteChunk.palID<<12)).to_bytes(2, byteorder='little')
        newSpriteChunk.ltrOAM2 = (tileOffset | (newSpriteChunk.palID<<12)).to_bytes(2, byteorder='little')
        self.spriteChunks.append(newSpriteChunk)
        totalNewSpriteChunks.append(newSpriteChunk)
        self.image.paste(newSpriteChunk.image, (tileOffset<<3, 0))
        
    return True, totalNewSpriteChunks

class SpriteChunk:
  def __init__(self, image, x, y, w, h, palID):
    self.image = image
    self.hash = hashImage(image)
    self.x = x
    self.y = y
    self.w = w
    self.h = h
    self.palID = palID
    if w == h:
      shape = 0
      size = w>>1
    elif w > h:
      shape = 1
      size = 3 if w == 8 else (w>>2) + (h>>1)
    else:
      shape = 2
      size = 3 if h == 8 else (w>>1) + (h>>2)
    self.rtlOAM0 = (shape<<14).to_bytes(2, byteorder='little')
    self.rtlOAM1 = (size<<14).to_bytes(2, byteorder='little')
    self.rtlOAM2 = (palID<<12).to_bytes(2, byteorder='little')
    self.rtlX = ((x<<3) - 148).to_bytes(2, byteorder='little', signed=True)
    self.rtlY = ((y<<3) - 88).to_bytes(2, byteorder='little', signed=True)
    self.ltrOAM0 = (shape<<14).to_bytes(2, byteorder='little')
    self.ltrOAM1 = ((size<<14)|0x1000).to_bytes(2, byteorder='little')
    self.ltrOAM2 = (palID<<12).to_bytes(2, byteorder='little')
    self.ltrX = (148 - (x<<3) - (w<<3)).to_bytes(2, byteorder='little', signed=True)
    self.ltrY = ((y<<3) - 88).to_bytes(2, byteorder='little', signed=True)

# Adds frame's oamData to rtlOAMData and ltrOAMData.
def buildOAMData(spriteChunks, rtlOAMData, ltrOAMData):
  if not len(spriteChunks):
    return rtlOAMData, ltrOAMData
  for spriteChunk in spriteChunks:
    rtlOAMData += spriteChunk.rtlOAM0
    rtlOAMData += spriteChunk.rtlOAM1
    rtlOAMData += spriteChunk.rtlOAM2
    rtlOAMData += spriteChunk.rtlX
    rtlOAMData += spriteChunk.rtlY
    rtlOAMData += b'\x00\x00'
    ltrOAMData += spriteChunk.ltrOAM0
    ltrOAMData += spriteChunk.ltrOAM1
    ltrOAMData += spriteChunk.ltrOAM2
    ltrOAMData += spriteChunk.ltrX
    ltrOAMData += spriteChunk.ltrY
    ltrOAMData += b'\x00\x00'
  rtlOAMData += b'\x01' + b'\x00'*11  # Terminator.
  ltrOAMData += b'\x01' + b'\x00'*11  # Terminator.
  return rtlOAMData, ltrOAMData

# Resets sheet dimensions.
def setSheetDim(dim, half):
  global _1D_SPRITES
  global HALFSIZESHEETS
  global SHEET_WIDTH_PIXELS
  global SHEET_HEIGHT_PIXELS
  global SHEET_WIDTH_TILES
  global SHEET_HEIGHT_TILES
  global SHEET_SIZE_PIXELS
  global SHEET_SIZE_TILES
  
  HALFSIZESHEETS = half
  
  if dim == 1:
    _1D_SPRITES = True
    SHEET_WIDTH_PIXELS = 2048>>half
    SHEET_HEIGHT_PIXELS = 8
  else:
    _1D_SPRITES = False
    SHEET_WIDTH_PIXELS = 256
    SHEET_HEIGHT_PIXELS = 64>>half
  SHEET_WIDTH_TILES = SHEET_WIDTH_PIXELS>>3
  SHEET_HEIGHT_TILES = SHEET_HEIGHT_PIXELS>>3
  SHEET_SIZE_PIXELS = SHEET_WIDTH_PIXELS * SHEET_HEIGHT_PIXELS
  SHEET_SIZE_TILES = SHEET_WIDTH_TILES * SHEET_HEIGHT_TILES
  return

# Main parse script.
def main():
  global UNCOMPPALDATA
  global UNCOMPFRAMEDATA
  global UNCOMPOAMDATA
  global NOSPLITSPRITES
  global EMPTYTILEPER64
  global _2PALETTES
  global LEFTOAM_NOLOAD
  
  parser = argparse.ArgumentParser(prog="AAA",
                                   description="Assembles an FEGBA battle animation script and frames into an EA-insertable file.")
  parser.add_argument('filename', help='Input battle anim script .txt file.')
  parser.add_argument("--no-left-oam", action = 'store_true', help = "not load left oam (need PATCH_AutoGenLeftOAM installed)")
  args = parser.parse_args()

  if not os.path.isfile(args.filename):
    print("ERROR: Couldn't find input file: \"" + args.filename + "\".")
    exit()

  if args.no_left_oam:
    LEFTOAM_NOLOAD = True

  animName = args.filename[:-10]
  scriptFile = open(args.filename, "r")
  outputFile = open(animName+".banim.event", "w")
  mode = 1                            # Current mode. [1, 12].
  rtlOAMData = b'\x01' + b'\x00'*11   # Starts with terminator for empty pierce frames.
  ltrOAMData = b'\x01' + b'\x00'*11   # Starts with terminator for empty pierce frames.
  oamOffset = 0xC                     # Starts immediately past the first empty OAMData.
  sectionData = b'\x00'*4
  frameData = b''
  frameDataPierce = b''               # Pierce frames: Mode2 and Mode4.
  pierceFrames = True                 # True when in Mode1 or Mode3. False for the other modes.
  loopCount = -1                      # Frames since loop start. If == -1: no loop started.
  loopPossible = False                # Loops can be set immediately after C04 or C05.
  imageFileNameDic = {}               # Keeps track of image files already parsed.
  sheets = list()                     # Battle sprite sheets.
  header = True                       # Special @ instructions can be set at the start of the script.
  setSheetDim(2, False)               # Default: Use 2D sprites and fill the entire sheet.
  
  # Main parse loop.
  for i, line in enumerate(scriptFile):
    if header and line[0] == '@':           # Extra setting.
      
      if line[2:15] == "UNCOMPPALDATA":
        UNCOMPPALDATA = True
        continue
      elif line[2:17] == "UNCOMPFRAMEDATA":
        UNCOMPFRAMEDATA = True
        continue
      elif line[2:15] == "UNCOMPOAMDATA":
        UNCOMPOAMDATA = True
        continue
      elif line[2:11] == "1DSPRITES":
        setSheetDim(1, HALFSIZESHEETS)
        continue
      elif line[2:11] == "2PALETTES":
        # Two palettes instead of one.
        _2PALETTES = True
        continue
      elif line[2:16] == "NOSPLITSPRITES":
        # Disallow sprites to be split into multiple.
        NOSPLITSPRITES = True
        continue
      elif line[2:16] == "EMPTYTILEPER64":
        j = 17
        while line[j].isnumeric():
          EMPTYTILEPER64 = EMPTYTILEPER64*10 + int(line[j], 10)
          j += 1
        continue
      elif line[2:16] == "HALFSIZESHEETS":
        setSheetDim(_1D_SPRITES, True)
        continue
      else:
        print("ERROR: line: " + str(i+1) + ", Couldn't recognize @ code. I only recognize of \"@ 1DSPRITES\",  \"@ 2PALETTES\", \"@ NOSPLITSPRITES\", \"@ EMPTYTILERATIO: <num:denom>\".")
        exit()
    else:
      header = False;
  
    if line[0] == '~':          # Mode end.
      if loopCount != -1:
        print("ERROR: line: " + str(i+1) + ", I'm missing a C01 to finish a loop command.")
        exit()
      frameData += 0x80000000.to_bytes(4, byteorder='little') 
      if pierceFrames:
        frameDataPierce += 0x80000000.to_bytes(4, byteorder='little') 
      mode += 1
      if mode > 12:
        break
      sectionData += len(frameData).to_bytes(4, byteorder='little')
      if pierceFrames:
        frameData += frameDataPierce
        frameDataPierce = b''
        mode += 1
        sectionData += len(frameData).to_bytes(4, byteorder='little')
      # Already handled mode 1 pierce frames; No need to check.
      if mode == 3:
        pierceFrames = True
      else:
        pierceFrames = False
      loopPossible = False
      continue
    
    elif line[0] == 'C':        # 85 command.
      if not (line[1] in hexSet and line[2] in hexSet):
        print("ERROR: line: " + str(i+1) + ", Command: \"C" + line[1:3] + "\" isn't numeric. Make sure you're using exactly two digits.")
        exit()
      command = int(line[1:3], 16)
      if (command == 1 and loopCount != -1):
        # Loop
        if loopCount > 0xFF:
          print("ERROR: line: " + str(i+1) + ", Loop here is over 0xFF frames long.")
          exit()
        if loopCount < 3:
          print("ERROR: line: " + str(i+1) + ", Loop here is under 3 frames long.")
          exit()
        command = (loopCount << 8) | 0x1
        loopCount = -1
        loopPossible = False
      elif command == 4 or command == 5:
        loopPossible = True
      else:
        loopPossible = False
      entry = (command & 0x00FFFFFF) | 0x85000000
      frameData += entry.to_bytes(4, byteorder='little')
      if pierceFrames:
        frameDataPierce += entry.to_bytes(4, byteorder='little') 
      continue
      
    elif line[0] == 'L':        # Loop.
      if loopCount != -1:
        print("ERROR: line: " + str(i+1) + ", A loop is started here before a previous one finished.")
        exit()
      if not loopPossible:
        print("ERROR: line: " + str(i+1) + ", The loop here needs to be preceded by either C04 or C05.")
        exit()
      loopCount = 0
      loopPossible = False
      continue
    
    elif line[0] == 'S':        # Sound effect.
      command = 0
      for i in range(4):
        if line[i+1] in hexSet:
          command = (command<<4) | int(line[i+1], 16)
        else:
          break
      entry = (command & 0xFFFF) << 8 | 0x85000048
      frameData += entry.to_bytes(4, byteorder='little')
      if pierceFrames:
        frameDataPierce += entry.to_bytes(4, byteorder='little') 
      continue
    
    elif line[0].isnumeric():   # 86 command.
      frameTime = 0
      j = 0
      while line[j].isnumeric():
        frameTime *= 10
        frameTime += int(line[j], 10)
        j += 1
      frameTime &= 0xFF
      if line[j:j+4] != " p- ":
        print("ERROR: line: " + str(i+1) + ", format should be \"<time> p- <image>\", but isn't.")
        exit()
      if loopCount != -1:     # Increment loop count if a loop is active.
        loopCount += 3
      imageFileName = line[j+4:-1]
      if (len(imageFileName) <= 0) or (not os.path.isfile(imageFileName)):
        print("ERROR: line: " + str(i+1) + ", image file not found.")
        exit()
      if imageFileName not in imageFileNameDic:
        frameImage = Image.open(imageFileName)
        frame = Frame(imageFileName, frameImage, len(imageFileNameDic), sheets, oamOffset)
        imageFileNameDic[imageFileName] = frame
        (rtlOAMData, ltrOAMData) = buildOAMData(frame.spriteChunks, rtlOAMData, ltrOAMData)
        (rtlOAMData, ltrOAMData) = buildOAMData(frame.spriteChunksP, rtlOAMData, ltrOAMData)
        frameImage.close()
        oamOffset1 = 0 if not len(frame.spriteChunks) else oamOffset
        oamOffsetP = 0 if not len(frame.spriteChunksP) else oamOffset1 + (len(frame.spriteChunks) + 1) * 12
        if len(frame.spriteChunks):
          oamOffset += (len(frame.spriteChunks)+1) * 12
        if len(frame.spriteChunksP):
          oamOffset += (len(frame.spriteChunksP)+1) * 12
      else:
        frame = imageFileNameDic[imageFileName]
        oamOffset1 = frame.oamOffset1
        oamOffsetP = frame.oamOffsetP
      entry1 = 0x86000000 | (frame.index << 16) | frameTime
      entry2 = frame.sheetIndex
      frameData += entry1.to_bytes(4, byteorder='little')
      frameData += entry2.to_bytes(4, byteorder='little')
      frameData += oamOffset1.to_bytes(4, byteorder='little')
      if pierceFrames:
        frameDataPierce += entry1.to_bytes(4, byteorder='little')
        frameDataPierce += entry2.to_bytes(4, byteorder='little')
        frameDataPierce += oamOffsetP.to_bytes(4, byteorder='little')
      continue
      
  scriptFile.close()
  
  # Write data to installer file.
  # Anim entry.
  settingBitfield = UNCOMPPALDATA | (UNCOMPFRAMEDATA<<1) | (UNCOMPOAMDATA<<2) | (_2PALETTES<<3)
  defs = """
#ifndef AnimTableEntry
  #define AnimTableEntry(index) \"ORG ClassAnimTable + ((index) * 0x20)\"
#endif"""

  animEntry_default = """\n\nPUSH
  AnimTableEntry({animName}) // CHANGE THIS TO THE SLOT YOU ARE REPLACING
  String("CoolAnim")
  WORD {settingBitfield}
  POIN Anim_{animName}_sectiondata
  POIN Anim_{animName}_framedata
  POIN Anim_{animName}_rtl Anim_{animName}_ltr Anim_{animName}_pal
POP"""

  animEntry_noleftoam = """\n\nPUSH
  AnimTableEntry({animName}) // CHANGE THIS TO THE SLOT YOU ARE REPLACING
  String("CoolAnim")
  WORD {settingBitfield}
  POIN Anim_{animName}_sectiondata
  POIN Anim_{animName}_framedata
  POIN Anim_{animName}_rtl Anim_{animName}_rtl Anim_{animName}_pal
POP"""

  if not LEFTOAM_NOLOAD:
    animEntry = animEntry_default
  else:
    animEntry = animEntry_noleftoam

  outputFile.write(defs)
  outputFile.write(animEntry.format(animName=animName, settingBitfield=str(settingBitfield)))
  
  # Palette.
  if not len(imageFileNameDic):
    print("ERROR: Script doesn't contain any images.")
    exit()
  outputFile.write("\n\nAnim_"+animName+"_pal:\n")
  frame = imageFileNameDic[next(iter(imageFileNameDic))]        # Grab a frame from this dictionary.
  pal = [i>>3 for i in frame.palette]
  pal2 = [pal[i] | (pal[i+1]<<5) | (pal[i+2]<<10) for i in range(0, len(pal), 3)]
  pal = b''
  for p in pal2:
    pal += p.to_bytes(2, byteorder='little')
  palsize = 128 << _2PALETTES
  if UNCOMPPALDATA:
    outputFile.write(b_to_EA(pal[:palsize]))                  # Uncompressed.
  else:
    outputFile.write(b_to_EA(lzss.compress(pal[:palsize])))   # Compressed.
  
  # SectionData.
  outputFile.write("\n\nAnim_"+animName+"_sectiondata:\n")
  outputFile.write(b_to_EA(sectionData))
  
  # FrameData. Most of this is stolen from circleseverywhere's Animation Assembler.
  size = len(frameData)
  outputFile.write("\n\nAnim_"+animName+"_framedata:")
  
  if UNCOMPFRAMEDATA:
    # Uncompressed.
    isPointer = False
    for i in range(0, size, 4):
      if isPointer:
        sheetNum = int.from_bytes(frameData[i:i+4], byteorder='little')
        outputFile.write("\nPOIN Anim_"+animName+"_Sheet_"+str(sheetNum))
        isPointer = False
      elif frameData[i+3] == 0x86:
        isPointer = True
        outputFile.write("\nWORD " + str(hex(int.from_bytes(frameData[i:i+4], byteorder='little'))))
      else: 
        outputFile.write("\nWORD " + str(hex(int.from_bytes(frameData[i:i+4], byteorder='little'))))
  else:
    # Compressed.
    outputFile.write("\n")
    outputFile.write(int_to_EA(((size<<8) | 0x10), 4))
    outputFile.write(" 0")
    i = 0
    addZero = False
    isPointer = False
    for i in range(0, size, 4):
      if isPointer:
        sheetNum = int.from_bytes(frameData[i:i+4], byteorder='little')
        outputFile.write("; POIN2 Anim_"+animName+"_Sheet_"+str(sheetNum)+"; BYTE")
        isPointer = False
      elif frameData[i+3] == 0x86:
        isPointer = True
        outputFile.write(" " + b_to_EA(frameData[i:i+4])[5:])
      else:
        outputFile.write(" " + b_to_EA(frameData[i:i+4])[5:])
      if addZero:
        addZero = False
        outputFile.write(" 0")
      else:
        addZero = True
  outputFile.write("\nALIGN 4")
  
  # OAMData.
  outputFile.write("\n\nAnim_"+animName+"_rtl:\n")
  if UNCOMPOAMDATA:
    outputFile.write(b_to_EA(rtlOAMData))
  else:
    outputFile.write(b_to_EA(lzss.compress(rtlOAMData)))
  
  if not LEFTOAM_NOLOAD:
    outputFile.write("\n\nAnim_"+animName+"_ltr:\n")
    if UNCOMPOAMDATA:
      outputFile.write(b_to_EA(ltrOAMData))
    else:
      outputFile.write(b_to_EA(lzss.compress(ltrOAMData)))
  
  # Sheets.
  for id, sheet in enumerate(sheets):
    imageData = sheet.image.tobytes()
    byteSheet = b''
    for i in range(SHEET_HEIGHT_TILES):
      for j in range(SHEET_WIDTH_TILES):
        for k in range(8):
          for l in range(4):
            index = l*2 + SHEET_WIDTH_PIXELS*k +(j<<3) + SHEET_WIDTH_PIXELS*8*i
            bytesheet1 = (imageData[index] & 0xF) | ((imageData[index+1] & 0xF)<<4)
            byteSheet += (bytesheet1).to_bytes(1, byteorder='little')
    outputFile.write("\n\nAnim_"+animName+"_Sheet_"+str(id)+":\n")
    outputFile.write(b_to_EA(lzss.compress(byteSheet)))
  
  outputFile.close()

start = time.time()
main()
end = time.time()
# print("Finished.\nTime taken in seconds: " + str(end - start))
# print("Number of comparisons: " + str(IMGCOMPARISONCOUNT) + ". Hash collisions: " + str(HASHCOLLISIONS) + ".")
# print("Collision ratio: " + str(HASHCOLLISIONS/max(IMGCOMPARISONCOUNT, 1)))
