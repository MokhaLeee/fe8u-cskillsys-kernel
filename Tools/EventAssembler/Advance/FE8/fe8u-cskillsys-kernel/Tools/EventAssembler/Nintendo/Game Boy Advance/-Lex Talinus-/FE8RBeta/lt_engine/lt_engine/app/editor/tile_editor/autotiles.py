import glob
from collections import OrderedDict

from PyQt5.QtWidgets import QProgressDialog
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QImage, QColor, QPainter, qRgb

from app.editor import utilities as editor_utilities

from app.constants import TILEWIDTH, TILEHEIGHT, AUTOTILE_FRAMES

def similar(p1: list, p2: list) -> int:
    """
    Attempts to compare the pattern of the tiles, not the actual values themselves
    """
    mapping = {}
    count = 0
    for i, j in zip(p1, p2):
        if i == j:
            mapping[i] = j
        elif i in mapping and j != mapping[i]:
            count += 1
        else:
            mapping[i] = j
    return count

def similar_fast(p1: list, p2: list) -> int:
    """
    Attempts to compare the pattern of the tiles, not the actual values themselves
    """
    mapping = {}
    for i, j in zip(p1, p2):
        if i == j:
            mapping[i] = j
        elif i in mapping and j != mapping[i]:
            return TILEWIDTH * TILEHEIGHT
        else:
            mapping[i] = j
    return 0

class Series(list):
    def is_present(self, test) -> bool:
        test_palette = test.palette
        all_palettes = [im.palette for im in self]
        return any(similar_fast(test_palette, palette) for palette in all_palettes)

    def get_frames_with_color(self, color: tuple) -> list:
        return [im for im in self if color in im.colors]

class PaletteData():
    def __init__(self, im: QImage):
        self.im: QImage = im
        self.new_im: QImage = None
        self.colors: list = editor_utilities.get_full_palette(im)
        # Sort by most
        self.uniques: list = sorted(set(self.colors), key=lambda x: self.colors.count(x), reverse=True)
        self.palette: list = self.colors[:]

        for idx, pixel in enumerate(self.colors):
            # Each pixel in the palette is assigned its color id
            self.palette[idx] = self.uniques.index(pixel)
            # So palette is a unique string of ints

class AutotileMaker():
    def __init__(self, parent=None, fast=True):
        self.window = parent
        # Must be TILEWIDTH/TILEHEIGHT or the engine and editor don't display properly
        self.tilewidth = TILEWIDTH
        self.tileheight = TILEHEIGHT
        self.similar_func = similar_fast if fast else similar

        self.map_tiles = OrderedDict()
        self.books = []
        self.autotile_column_idxs = {}
        self.recognized_series = []
        self.companion_autotile_im = None
        self.color_change_flag = True

        msg = "Generating Autotiles..."
        self.progress_dialog = QProgressDialog(msg, "Cancel", 0, 100, self.window)
        self.progress_dialog.setAutoClose(True)
        self.progress_dialog.setWindowTitle(msg)
        self.progress_dialog.setWindowModality(Qt.WindowModal)
        self.progress_dialog.setWindowFlag(Qt.WindowContextHelpButtonHint, False)

        self.autotile_templates = self.gather_templates()
        self.should_run = self.load_autotile_templates()

    def clear(self):
        self.map_tiles.clear()
        self.autotile_column_idxs.clear()
        self.recognized_series.clear()
        self.companion_autotile_im = None

        for book in self.books:
            for series in book:
                for frame in series:
                    frame.new_im = None

    def run(self, tileset, color_change_flag=True):
        self.color_change_flag = color_change_flag
        print("Color Change Flag: %s" % color_change_flag)
        self.clear()
        # self.should_run = self.load_autotile_templates()
        if not self.should_run:
            return None, {}
        self.progress_dialog.setValue(20)
        self.tileset = tileset
        self.tileset_image = QImage(self.tileset.pixmap)
        if self.progress_dialog.wasCanceled():
            return None, {}
        self.palettize_tileset()
        self.progress_dialog.setValue(25)
        if self.progress_dialog.wasCanceled():
            return None, {}

        for idx, pos in enumerate(self.map_tiles):
            if self.progress_dialog.wasCanceled():
                return None, {}
            self.create_autotiles_from_image(pos)
            self.progress_dialog.setValue(25 + int(70 * idx / len(self.map_tiles)))

        if self.recognized_series:
            self.create_final_image()
        self.progress_dialog.setValue(99)
        if self.progress_dialog.wasCanceled():
            return None, {}

        final_column_idxs = {k: v[0] for k, v in self.autotile_column_idxs.items()}
        self.progress_dialog.setValue(100)
        return self.companion_autotile_im, final_column_idxs

    def gather_templates(self) -> list:
        templates = []
        for fn in sorted(glob.glob('resources/autotiles/*.png')):
            templates.append(fn)
        return templates

    def load_autotile_templates(self) -> bool:
        import time
        # Each autotile template becomes a book
        # A book contains a dictionary
        # Key: position
        # Value: Series
        self.num_tiles_x_list = []
        for idx, template in enumerate(self.autotile_templates):
            if self.progress_dialog.wasCanceled():
                return False
            print(template)
            image = QImage(template)
            width = image.width() // AUTOTILE_FRAMES
            height = image.height()
            num_tiles_x = width // self.tilewidth
            num_tiles_y = height // self.tileheight
            self.num_tiles_x_list.append(num_tiles_x)
            print(num_tiles_x, num_tiles_y)
            num = num_tiles_x * num_tiles_y
            minitiles = [Series() for _ in range(num)]

            # There are 16 frames, stacked horizontally with one another
            time1 = time.time_ns() / 1e6
            for frame in range(AUTOTILE_FRAMES):    
                x_offset = frame * width
                for x in range(num_tiles_x):
                    for y in range(num_tiles_y):
                        rect = (x_offset + x * self.tilewidth, y * self.tileheight, self.tilewidth, self.tileheight)
                        palette = image.copy(*rect)  # crop
                        d = PaletteData(palette)
                        minitiles[x + y * num_tiles_x].append(d)
            time2 = time.time_ns() / 1e6
            print(time2 - time1)

            assert all(len(series) == AUTOTILE_FRAMES for series in minitiles)
            self.books.append(minitiles)
            self.progress_dialog.setValue(20 * int(idx / len(self.autotile_templates)))
        return True

    def palettize_tileset(self):
        """
        Generates map tiles
        """
        self.map_tiles.clear()
        print("Palettizing current tileset...")

        for x in range(self.tileset.width):
            for y in range(self.tileset.height):
                rect = (x * self.tilewidth, y * self.tileheight, self.tilewidth, self.tileheight)
                tile = self.tileset_image.copy(*rect)  # Crop
                tile_palette = PaletteData(tile)
                self.map_tiles[(x, y)] = tile_palette

    def create_autotiles_from_image(self, pos):
        x, y = pos
        tile_palette = self.map_tiles[(x, y)]
        # No colors or only one color...
        # Don't bother finding an autotile for this
        if len(tile_palette.uniques) < 2:
            return

        print("Attempting to find match for position (%d, %d)..." % (pos[0], pos[1]))
        closest_series = None
        closest_frame = None
        closest_book = None
        min_sim = 4

        for book_idx, book in enumerate(self.books):
            for series_idx, series in enumerate(book):
                if not self.series_has_changes(series):
                    continue  # Don't bother checking if it doesn't have changes
                for frame_idx, frame in enumerate(series):
                    similarity = self.similar_func(frame.palette, tile_palette.palette)
                    if similarity < min_sim:
                        min_sim = similarity
                        closest_series = series
                        closest_frame = frame
                        closest_book = book
                        num_tiles_x = self.num_tiles_x_list[book_idx]
                        pos_x = series_idx%num_tiles_x
                        pos_y = series_idx//num_tiles_x
                        print("Similarity met for position %s using template: %s at template pos (%d, %d), frame %d" % 
                              (pos, self.autotile_templates[book_idx], pos_x, pos_y, frame_idx))

        if closest_series:
            if closest_series in self.recognized_series:
                column_idx = self.recognized_series.index(closest_series)
            else:
                # Add series to autotile column list if it is not already
                # If it's not added to autotile column image, make sure it uses the right colors
                self.recognized_series.append(closest_series)
                column_idx = len(self.recognized_series) - 1
                if self.color_change_flag:
                    self.color_change(tile_palette, closest_frame, closest_series, closest_book)
            print("Final column idx", column_idx)
            self.autotile_column_idxs[(x, y)] = (column_idx, closest_frame, closest_series, closest_book)
        else:
            print("No match.")

    def color_change(self, tile, closest_frame, closest_series, closest_book):
        # Converts color from closest frame to tile
        print("Color change")
        color_conversion = {}
        truecolor = {}
        for idx, color in enumerate(closest_frame.colors):
            truecolor[color] = tile.colors[idx]
            color_conversion[qRgb(*color)] = qRgb(*tile.colors[idx])

        def fix_missing_color(color: tuple):
            # Does that color show up in other frames in the autotile band?
            for series in closest_book:
                frames_with_color = series.get_frames_with_color(color)
                for f in frames_with_color:
                    if len(f.uniques) < 2:
                        continue
                    for map_tile in self.map_tiles.values():
                        if len(map_tile.uniques) < 2:
                            continue
                        # If so, do those frames show up in the map sprite?
                        if similar_fast(f.palette, map_tile.palette) == 0:
                            # If so, add to the color conversion
                            color_idx = f.colors.index(color)
                            new_color = map_tile.colors[color_idx]
                            truecolor[color] = new_color
                            color_conversion[qRgb(*color)] = qRgb(*new_color)
                            print("%s has become %s" % (color, new_color))
                            return

        for palette_data in closest_series:
            for idx, color in enumerate(palette_data.colors):
                if color not in truecolor:
                    print("Missing color: %s" % str(color))
                    fix_missing_color(color)

        for palette_data in closest_series:
            new_im = editor_utilities.color_convert(palette_data.im.copy(), color_conversion)
            palette_data.new_im = new_im

    def create_final_image(self):
        width = len(self.recognized_series) * self.tilewidth
        height = AUTOTILE_FRAMES * self.tileheight
        new_im = QImage(width, height, QImage.Format_ARGB32)
        new_im.fill(QColor(255, 255, 255, 0))

        painter = QPainter(new_im)
        for i, series in enumerate(self.recognized_series):
            for j, palette_data in enumerate(series):
                x, y = i * self.tilewidth, j * self.tileheight
                # Paste image
                if palette_data.new_im:
                    painter.drawImage(x, y, palette_data.new_im)
                else:
                    painter.drawImage(x, y, palette_data.im)
        painter.end()

        self.companion_autotile_im = new_im

    def series_has_changes(self, series: list) -> bool:
        no_changes = all(frame.palette == series[0].palette for frame in series)
        return not no_changes

AUTOTILEMAKER = None
def get_maker(fast=True):
    global AUTOTILEMAKER
    if not AUTOTILEMAKER:
        AUTOTILEMAKER = AutotileMaker(fast=fast)
    return AUTOTILEMAKER

if __name__ == '__main__':
    import os
    from PyQt5.QtGui import QPixmap

    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)

    maker = get_maker(width=TILEWIDTH//2, height=TILEHEIGHT//2, fast=False)

    class FakeTileset():
        def __init__(self, img_path):
            self.full_path = img_path
            self.pixmap = QPixmap(img_path)
            self.width = self.pixmap.width() // (TILEWIDTH//2)
            self.height = self.pixmap.height() // (TILEHEIGHT//2)

    # tileset = FakeTileset('app/map_maker/rainlash_fields1_river.png')
    tileset = FakeTileset('app/map_maker/rainlash_fields1_sea.png')
    companion_tileset, column_idxs = maker.run(tileset, True)
    print("Column Idxs")
    print(column_idxs)
    full_path = tileset.full_path
    new_full_path = full_path[:-4] + '_autotiles.png'
    fn = os.path.abspath(new_full_path)
    companion_tileset.save(fn)
    tileset.autotiles = column_idxs
    tileset.autotile_full_path = fn
    pix = QPixmap(companion_tileset)
    tileset.autotile_pixmap = pix
    print("Done!")
