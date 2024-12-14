import math

from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, \
    QSizePolicy, QTableView, QPushButton, QDialog, QHBoxLayout, \
    QButtonGroup, QMenu, QAction, QApplication, QMessageBox, QLineEdit, QVBoxLayout
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont

from app.utilities import utils
from app.data.database.database import DB

from app.extensions.custom_gui import IntDelegate, PropertyBox, SimpleDialog
from app.extensions.list_models import VirtualListModel

import logging

class MultiEditTableView(QTableView):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.customMenuRequested)

        self.copy_action = QAction("Copy", self, shortcut="Ctrl+C", triggered=self.copy)
        self.paste_action = QAction("Paste", self, shortcut="Ctrl+V", triggered=self.paste)

    def customMenuRequested(self, pos):
        index = self.indexAt(pos)
        if not index.isValid():
            return None

        menu = QMenu(self)
        menu.addAction(self.copy_action)
        menu.addAction(self.paste_action)

        menu.popup(self.viewport().mapToGlobal(pos))

    def copy(self):
        if not (self.currentIndex() and self.currentIndex().isValid()):
            return None
        clipboard = QApplication.clipboard()
        model = self.currentIndex().model()
        all_values = []
        for index in self.selectionModel().selectedIndexes():
            value = model.data(index, Qt.DisplayRole)
            all_values.append(str(value))
        final_text = ','.join(all_values)
        clipboard.setText(final_text)

    def paste(self):
        if not (self.currentIndex() and self.currentIndex().isValid()):
            return None
        clipboard = QApplication.clipboard()
        model = self.currentIndex().model()
        final_text = clipboard.text()
        logging.debug(final_text)
        try:
            if ',' in final_text:
                all_values = [int(_) for _ in final_text.split(',')]
            else:
                all_values = [int(_) for _ in final_text.split()]
            if len(self.selectionModel().selectedIndexes()) > 1:
                current_index = self.selectionModel().selectedIndexes()[0]
            else:
                current_index = self.currentIndex()

            for idx, value in enumerate(all_values):
                model.setData(current_index, value, Qt.EditRole)
                next_column = current_index.column() + 1
                current_index = model.index(current_index.row(), next_column)

        except Exception as e:
            logging.error("Could not read from clipboard! %s" % e)
            QMessageBox.critical(None, "Import Error", "Could not read valid text from clipboard!")

    def keyPressEvent(self, event):
        super().keyPressEvent(event)
        if event.key() == Qt.Key_C and (QApplication.keyboardModifiers() & Qt.ControlModifier):
            self.copy()
        elif event.key() == Qt.Key_V and (QApplication.keyboardModifiers() & Qt.ControlModifier):
            self.paste()

    def commitData(self, editor):
        super().commitData(editor)
        model = self.currentIndex().model()
        value = model.data(self.currentIndex(), Qt.EditRole)
        for index in self.selectionModel().selectedIndexes():
            model.setData(index, value, Qt.EditRole)

class StatListWidget(QWidget):
    def __init__(self, obj, title, reset_button=False, average_button=True, parent=None):
        super().__init__(parent)
        self.window = parent
        self._obj = obj

        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        column_titles = DB.stats.keys()
        if obj:
            row_titles = obj.get_stat_titles()
            row_values = obj.get_stat_lists()
        else:
            row_titles = ['Example']
            row_values = [{}]

        self.reset_button_flag = reset_button
        self.average_button_flag = average_button

        self.setup(column_titles, row_titles, row_values, title)

    def setup(self, column_titles, row_titles, row_values, title):
        self.model = StatModel(column_titles, row_titles, row_values, self)
        self.view = MultiEditTableView(self)
        self.view.setModel(self.model)
        self.view.setSelectionMode(3)  # ExtendedSelection
        delegate = IntDelegate(self.view, range(len(column_titles)))
        self.view.setItemDelegate(delegate)
        for col in range(len(column_titles)):
            self.view.resizeColumnToContents(col)

        layout = QGridLayout(self)
        layout.setSpacing(0)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.view, 1, 0, 1, 2)
        self.setLayout(layout)

        label = QLabel(title)
        label.setAlignment(Qt.AlignBottom)
        layout.addWidget(label, 0, 0)

        hbox = QHBoxLayout()

        if self.reset_button_flag:
            self.reset_button = QPushButton("Apply Class Values")
            self.reset_button.setMaximumWidth(150)
            hbox.addWidget(self.reset_button, alignment=Qt.AlignRight)

        if self.average_button_flag:
            self.button = QPushButton("Display Averages")
            self.button.setMaximumWidth(130)
            hbox.addWidget(self.button, alignment=Qt.AlignRight)
            layout.addLayout(hbox, 0, 1, alignment=Qt.AlignRight)

    def get_stat_lists(self, obj):
        return obj.get_stat_lists()

    def set_new_obj(self, obj):
        self._obj = obj
        row_titles = obj.get_stat_titles()
        row_values = self.get_stat_lists(obj)
        self.model.set_new_data(row_titles, row_values)
        for col in range(len(row_values[0])):
            self.view.resizeColumnToContents(col)

    def update_stats(self):
        column_titles = DB.stats.keys()
        self.model.update_column_header(column_titles)
        if self._obj:
            self.set_new_obj(self._obj)

class StatModel(VirtualListModel):
    def __init__(self, columns, rows, data, parent=None):
        super().__init__(parent)
        self.window = parent
        self._columns = self._headers = columns
        self._rows = rows
        self._data: list = data  # Must be list of dicts

    def set_new_data(self, stat_titles: list, stat_lists: list):
        self._rows: list = stat_titles
        self._data: list = stat_lists
        self.layoutChanged.emit()

    def update_column_header(self, columns):
        self._columns = self._headers = columns

    def headerData(self, idx, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Vertical:
            val = self._rows[idx]
            return val
        elif orientation == Qt.Horizontal:
            return self._columns[idx]

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole or role == Qt.EditRole:
            row = self._data[index.row()]  # row is a dict
            key = self._columns[index.column()]
            attr = self._rows[index.row()]
            if attr == 'max_stats':
                val = row.get(key, DB.stats.get(key).maximum)
            else:
                val = row.get(key, 0)
            return val
        elif role == Qt.TextAlignmentRole:
            return Qt.AlignRight + Qt.AlignVCenter

    def setData(self, index, value, role):
        if not index.isValid():
            return False
        row = self._data[index.row()]  # A dict
        key = self._columns[index.column()]  # A stat key
        row[key] = value
        self.dataChanged.emit(index, index)
        return True

    def flags(self, index):
        basic_flags = Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemNeverHasChildren | Qt.ItemIsEditable
        return basic_flags

class StatAverageDialog(QDialog):
    def __init__(self, current, title, model, parent=None):
        super().__init__(parent)
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)
        self.setWindowTitle("%s Stat Averages Display" % title)
        self.window = parent
        self.current = current
        self.setMinimumWidth(400)

        column_titles = DB.stats.keys()
        self.setup(column_titles, "Average Stats", model)
        if title == 'Generic':
            self.view.verticalHeader().setFixedWidth(20)

    def setup(self, column_titles, title, model):
        self.model = model(column_titles, self.current, parent=self)
        self.view = QTableView(self)
        self.view.setModel(self.model)
        for col in range(len(column_titles)):
            self.view.resizeColumnToContents(col)

        layout = QGridLayout(self)
        layout.setSpacing(0)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.view, 1, 0, 1, 2)
        self.setLayout(layout)

        label = QLabel(title)
        label.setAlignment(Qt.AlignBottom)
        layout.addWidget(label, 0, 0)

        hbox_layout = QHBoxLayout()
        hbox_layout.setSpacing(0)
        hbox_layout.setContentsMargins(0, 0, 0, 0)
        layout.addLayout(hbox_layout, 0, 1, alignment=Qt.AlignRight)

        self.button_custom = QPushButton("Show all levels")
        hbox_layout.addWidget(self.button_custom, alignment=Qt.AlignCenter)
        self.button_custom.clicked.connect(self.show_all_levels)
        self.showing_all = False

        self.button_group = QButtonGroup(self)
        self.button_group.setExclusive(True)
        self.button_group.buttonToggled.connect(self.button_clicked)
        self.button10 = QPushButton("10%")
        self.button50 = QPushButton("50%")
        self.button90 = QPushButton("90%")
        self.buttons = [self.button10, self.button50, self.button90]
        for idx, button in enumerate(self.buttons):
            button.setMaximumWidth(50)
            button.setCheckable(True)
            hbox_layout.addWidget(button, alignment=Qt.AlignRight)
            self.button_group.addButton(button)
            self.button_group.setId(button, idx)
        self.button50.setChecked(True)

    def button_clicked(self, spec_button):
        checked = spec_button.isChecked()
        if checked:
            if spec_button == self.button10:
                self.model.average_idx = 2
            elif spec_button == self.button50:
                self.model.average_idx = 1
            elif spec_button == self.button90:
                self.model.average_idx = 3
            self.update()

    def set_current(self, current):
        self.current = current
        self.model.set_current(self.current, self.showing_all)

    def update(self):
        self.model.layoutChanged.emit()

    def show_all_levels(self):
        self.showing_all = not self.showing_all
        self.model.set_current(self.current, self.showing_all)

    def closeEvent(self, event):
        # Remove averages dialog
        self.window.averages_dialog = None

class Binomial():
    @staticmethod
    def fact(n, k):
        return math.factorial(n) / math.factorial(k) / math.factorial(n - k)

    @staticmethod
    def binom(x, n, p):
        return Binomial.fact(n, x) * p**x * (1-p)**(n-x)

    @staticmethod
    def cdf(x, n, p):
        total = 0
        for i in range(x + 1):
            total += Binomial.binom(i, n, p)
        return total

    @staticmethod
    def quantile(q, n, p):
        for x in range(n + 1):
            prob = Binomial.cdf(x, n, p)
            if prob > q:
                return x
        return n

class ClassStatAveragesModel(VirtualListModel):
    average_idx = 1

    def __init__(self, columns, current, parent=None):
        super().__init__(parent)
        self.window = parent
        self._columns = self._headers = columns
        self.current = current
        self._rows = [1] + list(range(5, current.max_level, 5)) + [current.max_level]

    def set_current(self, current, all_levels=False):
        self.current = current
        if not all_levels:
            self._rows = [1] + list(range(5, current.max_level, 5)) + [current.max_level]
        else:
            self._rows = list(range(1, current.max_level + 1))
        self.layoutChanged.emit()

    def determine_average(self, obj, stat_nid, level_ups):
        if obj.tier > 1:
            if obj.promotes_from:
                prev_klass = DB.classes.get(obj.promotes_from)
                level_ups += prev_klass.max_level
            else:
                level_ups += 0
        stat_base = obj.bases.get(stat_nid, 0)
        stat_growth = obj.growths.get(stat_nid, 0)
        stat_max = obj.max_stats.get(stat_nid, DB.stats.get(stat_nid).maximum)

        average = int(stat_base + 0.5 + (stat_growth/100) * level_ups)

        while stat_growth > 100:
            stat_growth -= 100
            stat_base += level_ups
        quantile10 = Binomial.quantile(.1, level_ups, stat_growth/100) + stat_base
        quantile90 = Binomial.quantile(.9, level_ups, stat_growth/100) + stat_base
        return stat_max, average, quantile10, quantile90

    def update_column_header(self, columns):
        self._columns = self._headers = columns

    def rowCount(self, parent=None):
        return len(self._rows)

    def headerData(self, idx, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Vertical:
            val = self._rows[idx]
            return val
        elif orientation == Qt.Horizontal:
            return self._columns[idx]

    def get_data(self, index):
        level = self._rows[index.row()]
        stat_nid = self._columns[index.column()]
        vals = self.determine_average(self.current, stat_nid, level - 1)
        avg = vals[self.average_idx]
        maxim = vals[0]
        return maxim, avg

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            maxim, avg = self.get_data(index)
            return min(maxim, avg)
        elif role == Qt.TextAlignmentRole:
            return Qt.AlignRight + Qt.AlignVCenter
        elif role == Qt.FontRole:
            maxim, avg = self.get_data(index)
            font = QFont()
            if avg >= maxim:
                font.setBold(True)
            return font

    def flags(self, index):
        basic_flags = Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemNeverHasChildren
        return basic_flags

class GenericStatAveragesModel(ClassStatAveragesModel):
    def __init__(self, columns, current, parent=None):
        VirtualListModel.__init__(self, parent)
        self.window = parent
        self._columns = self._headers = columns
        self.current = current
        self._rows = [current.level]

    def set_current(self, current, _=None):
        self.current = current
        self._rows = [current.level]
        self.layoutChanged.emit()

    def determine_average(self, obj, stat_nid, level_ups):
        klass = DB.classes.get(obj.klass)
        if klass.tier > 1:
            if klass.promotes_from:
                prev_klass = DB.classes.get(klass.promotes_from)
                level_ups += prev_klass.max_level
            else:
                level_ups += 0
        stat_base = klass.bases.get(stat_nid, 0)
        stat_growth = klass.growths.get(stat_nid, 0)
        stat_max = klass.max_stats.get(stat_nid, DB.stats.get(stat_nid).maximum)

        average = int(stat_base + 0.5 + (stat_growth/100) * level_ups)

        # average = quantile(.5, level_ups, stat_growth/100) + stat_base
        while stat_growth > 100:
            stat_growth -= 100
            stat_base += level_ups
        quantile10 = Binomial.quantile(.1, level_ups, stat_growth/100) + stat_base
        quantile90 = Binomial.quantile(.9, level_ups, stat_growth/100) + stat_base
        return stat_max, average, quantile10, quantile90

class UnitStatAveragesModel(ClassStatAveragesModel):
    def __init__(self, columns, current, parent=None):
        VirtualListModel.__init__(self, parent)
        self.window = parent
        self._columns = self._headers = columns
        self.current = current
        self.get_rows()

    def get_rows(self, all_levels=False):
        klass = DB.classes.get(self.current.klass)
        max_level = klass.max_level
        self._rows = []
        if not all_levels:
            level_range = [1] + list(range(5, max_level, 5)) + [max_level]
        else:
            level_range = list(range(1, max_level+1))
        for i in level_range:
            self._rows.append((klass.nid, i, i))
        true_levels = 0
        while klass.promotion_options(DB):
            true_levels += max_level
            klass = DB.classes.get(klass.promotion_options(DB)[0])
            if klass:
                max_level = klass.max_level
                for i in level_range:
                    self._rows.append((klass.nid, i, i + true_levels))
            else:
                return

    def set_current(self, current, all_levels=False):
        self.current = current
        self.get_rows(all_levels)
        self.layoutChanged.emit()

    def headerData(self, idx, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Vertical:
            nid, level, true_level = self._rows[idx]
            long_name = DB.classes.get(nid).name
            row = long_name + " " + str(level)
            return row
        elif orientation == Qt.Horizontal:
            return self._columns[idx]

    def determine_average(self, obj, stat_nid, level_ups):
        stat_base = obj.bases.get(stat_nid, 0)
        stat_growth = obj.growths.get(stat_nid, 0)
        stat_cap_modifier = obj.stat_cap_modifiers.get(stat_nid, 0)
        if DB.constants.value('unit_stats_as_bonus'):
            klass = DB.classes.get(obj.klass)
            stat_base += klass.bases.get(stat_nid, 0)
            stat_growth += klass.growths.get(stat_nid, 0)
        average = 0.5
        quantile10 = 0
        quantile90 = 0
        classes = [obj.klass]
        base_klass = DB.classes.get(obj.klass)
        turns_into = base_klass.promotion_options(DB)
        while turns_into:
            classes.append(turns_into[0])
            new_klass = DB.classes.get(turns_into[0])
            turns_into = new_klass.promotion_options(DB)

        for idx, klass in enumerate(classes):
            if idx != 0:
                level_ups -= 1  # Costs one level to move up a class
            klass = DB.classes.get(klass)
            stat_max = klass.max_stats.get(stat_nid, DB.stats.get(stat_nid).maximum)
            stat_max += stat_cap_modifier
            if idx == 0:
                ticks = utils.clamp(level_ups, 0, klass.max_level - obj.level)
            else:
                ticks = utils.clamp(level_ups, 0, klass.max_level - 1)
            level_ups -= ticks
            growth_bonus = klass.growth_bonus.get(stat_nid, 0)
            if idx > 0:
                promotion_bonus = klass.promotion.get(stat_nid, 0)
                if promotion_bonus in (-99, -98):
                    prev_klass = classes[idx - 1]
                    promotion_bonus = klass.bases.get(stat_nid, 0) - DB.classes.get(prev_klass).bases.get(stat_nid, 0)
            else:
                promotion_bonus = stat_base
            growth = (stat_growth + growth_bonus)/100
            average += min(stat_max, promotion_bonus + (growth * ticks))
            while growth > 1:
                growth -= 1
                promotion_bonus += ticks
            quantile10 += min(stat_max, Binomial.quantile(.1, ticks, growth) + promotion_bonus)
            quantile90 += min(stat_max, Binomial.quantile(.9, ticks, growth) + promotion_bonus)
            if level_ups <= 0:
                break
        return stat_max, int(average), quantile10, quantile90

    def get_data(self, index):
        base_level = self.current.level
        nid, level, true_level = self._rows[index.row()]
        stat_nid = self._columns[index.column()]
        vals = self.determine_average(self.current, stat_nid, max(0, true_level - base_level))
        avg = vals[self.average_idx]
        maxim = vals[0]
        return maxim, avg
