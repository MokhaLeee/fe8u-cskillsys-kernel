from PyQt5 import QtGui
from PyQt5.QtWidgets import QHBoxLayout, QLabel, QPushButton, QSizePolicy, QVBoxLayout, QApplication, QDoubleSpinBox, QCheckBox, QFontComboBox
from PyQt5.QtCore import Qt

from app import dark_theme
from app.extensions.custom_gui import ComboBox, PropertyBox, PropertyCheckBox, Dialog

from app.editor.settings import MainSettingsController

from app.editor import timer, utilities

name_to_button = {'L-click': Qt.LeftButton,
                  'R-click': Qt.RightButton}
button_to_name = {v: k for k, v in name_to_button.items()}

key_to_button = {'Tab': Qt.Key_Tab,
                 'Enter': Qt.Key_Return}
button_to_key = {v: k for k, v in key_to_button.items()}

class PreferencesDialog(Dialog):
    def __init__(self, parent):
        super().__init__(parent)
        self.window = parent
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)
        self.setWindowTitle("Preferences")

        self.awaiting_button_bind = False

        self.settings = MainSettingsController()

        self.saved_preferences = {}
        self.saved_preferences['select_button'] = self.settings.get_select_button(Qt.LeftButton)
        self.saved_preferences['place_button'] = self.settings.get_place_button(Qt.RightButton)
        self.saved_preferences['theme'] = self.settings.get_theme(0)
        self.saved_preferences['code_font'] = self.settings.get_code_font()
        self.saved_preferences['event_autocomplete'] = self.settings.get_event_autocomplete(1)
        self.saved_preferences['autocomplete_button'] = self.settings.get_autocomplete_button(Qt.Key_Tab)
        self.saved_preferences['autosave_time'] = self.settings.get_autosave_time()
        self.saved_preferences['crash_logs'] = self.settings.get_should_display_crash_logs()
        self.saved_preferences['save_backup'] = self.settings.get_should_make_backup_save()
        self.saved_preferences['editor_close_button'] = self.settings.get_editor_close_button(Qt.Key_Escape)
        self.saved_preferences['save_chunks'] = self.settings.get_should_save_as_chunks()
        self.saved_preferences['auto_open_last'] = self.settings.get_auto_open()

        self.available_options = name_to_button.keys()
        self.autocomplete_options = key_to_button.keys()

        label = QLabel("Modify mouse preferences for Unit and Tile Painter Menus")

        self.select = PropertyBox('Select', ComboBox, self)
        for option in self.available_options:
            self.select.edit.addItem(option)
        self.place = PropertyBox('Place', ComboBox, self)
        for option in self.available_options:
            self.place.edit.addItem(option)
        self.select.edit.setValue(button_to_name[self.saved_preferences['select_button']])
        self.place.edit.setValue(button_to_name[self.saved_preferences['place_button']])
        self.select.edit.currentIndexChanged.connect(self.select_changed)
        self.place.edit.currentIndexChanged.connect(self.place_changed)

        self.theme = PropertyBox('Theme', ComboBox, self)
        for option in dark_theme.ThemeType:
            self.theme.edit.addItem(option.name)
        self.theme.edit.setValue(dark_theme.ThemeType(self.saved_preferences['theme']).name)
        self.theme.edit.currentIndexChanged.connect(self.theme_changed)

        self.code_font = PropertyBox('Code Font', QFontComboBox, self)
        self.code_font.edit.setFontFilters(QFontComboBox.FontFilter.MonospacedFonts)
        self.code_font.edit.setCurrentFont(QtGui.QFont(self.saved_preferences['code_font']))

        self.autocomplete = PropertyCheckBox('Event Autocomplete', QCheckBox, self)
        self.autocomplete.edit.setChecked(bool(self.saved_preferences['event_autocomplete']))

        self.crashlog = PropertyCheckBox('Show Error Logs on Crash?', QCheckBox, self)
        self.crashlog.edit.setChecked(bool(self.saved_preferences['crash_logs']))
        self.savebackup = PropertyCheckBox('Make Additional Backup Save?', QCheckBox, self)
        self.savebackup.edit.setChecked(bool(self.saved_preferences['save_backup']))
        self.savechunks = PropertyCheckBox('Save data in chunks?', QCheckBox, self)
        self.savechunks.setToolTip("Saving data in chunks makes it easier to collaborate with others, but also makes saving slower.")
        self.savechunks.edit.setChecked(bool(self.saved_preferences['save_chunks']))
        self.autoopen = PropertyCheckBox('Automatically open most recent project?', QCheckBox, self)
        self.autoopen.setToolTip("Skips the recent project dialog.")
        self.autoopen.edit.setChecked(bool(self.saved_preferences['auto_open_last']))

        self.autocomplete_button = PropertyBox('Autocomplete Button', ComboBox, self)
        for option in self.autocomplete_options:
            self.autocomplete_button.edit.addItem(option)
        self.autocomplete_button.edit.setValue(button_to_key[self.saved_preferences['autocomplete_button']])
        self.autocomplete_button.edit.currentIndexChanged.connect(self.autocomplete_button_changed)

        self.editor_close_button = PropertyBox('Editor Close Button', QPushButton, self)
        self.editor_close_button.edit.setText(utilities.qtkey_to_string(self.saved_preferences['editor_close_button']))
        self.editor_close_button.edit.clicked.connect(self.await_button_bind)
        sp = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        sp.setHorizontalStretch(4)
        self.editor_close_button.setSizePolicy(sp)
        self.editor_close_button_unbind = PropertyBox(' ', QPushButton, self)
        self.editor_close_button_unbind.edit.setText("Unbind")
        self.editor_close_button_unbind.edit.clicked.connect(self.unbind_editor_close_button)
        sp1 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        sp1.setHorizontalStretch(1)
        self.editor_close_button_unbind.setSizePolicy(sp1)

        self.editor_close_button_layout = QHBoxLayout()
        self.editor_close_button_layout.addWidget(self.editor_close_button)
        self.editor_close_button_layout.addWidget(self.editor_close_button_unbind)

        self.autosave = PropertyBox('Autosave Time (minutes)', QDoubleSpinBox, self)
        self.autosave.edit.setRange(0.5, 99)
        self.autosave.edit.setValue(self.saved_preferences['autosave_time'])
        self.autosave.edit.valueChanged.connect(self.autosave_time_changed)

        self.layout.addWidget(label)
        self.layout.addWidget(self.select)
        self.layout.addWidget(self.place)
        self.layout.addWidget(self.theme)
        self.layout.addWidget(self.code_font)
        self.layout.addWidget(self.autocomplete_button)
        self.layout.addLayout(self.editor_close_button_layout)
        self.layout.addWidget(self.autocomplete)
        self.layout.addWidget(self.crashlog)
        self.layout.addWidget(self.savebackup)
        self.layout.addWidget(self.savechunks)
        self.layout.addWidget(self.autoopen)
        self.layout.addWidget(self.autosave)
        self.layout.addWidget(self.buttonbox)

    def select_changed(self, idx):
        choice = self.select.edit.currentText()
        if choice == 'L-click':
            self.place.edit.setValue('R-click')
        else:
            self.place.edit.setValue('L-click')

    def place_changed(self, idx):
        choice = self.place.edit.currentText()
        if choice == 'L-click':
            self.select.edit.setValue('R-click')
        else:
            self.select.edit.setValue('L-click')

    def autocomplete_button_changed(self, idx):
        choice = self.autocomplete_button.edit.currentText()
        if choice == 'Tab':
            self.autocomplete_button.edit.setValue('Tab')
        else:
            self.autocomplete_button.edit.setValue('Return')

    def await_button_bind(self):
        self.editor_close_button.edit.setText('press any key')
        self.awaiting_button_bind = True

    def keyPressEvent(self, keypress: QtGui.QKeyEvent) -> None:
        if self.awaiting_button_bind:
            self.bind_editor_close_button(keypress.key())
            self.awaiting_button_bind = False
            return
        return super().keyPressEvent(keypress)

    def bind_editor_close_button(self, key: Qt.Key):
        self.saved_preferences['editor_close_button'] = key
        self.editor_close_button.edit.setText(utilities.qtkey_to_string(key))

    def unbind_editor_close_button(self):
        self.saved_preferences['editor_close_button'] = None
        self.editor_close_button.edit.setText(None)

    def theme_changed(self, idx):
        choice = self.theme.edit.currentText()
        ap = QApplication.instance()
        theme_type = dark_theme.ThemeType[choice]
        dark_theme.set(ap, theme_type)
        self.window.set_icons(theme_type)  # Change icons of main editor

    def autosave_time_changed(self, val):
        t = timer.get_timer()
        t.autosave_timer.stop()
        t.autosave_timer.setInterval(int(val * 60 * 1000))
        t.autosave_timer.start()

    def accept(self):
        self.settings.set_select_button(name_to_button[self.select.edit.currentText()])
        self.settings.set_place_button(name_to_button[self.place.edit.currentText()])
        self.settings.set_autocomplete_button(key_to_button[self.autocomplete_button.edit.currentText()])
        self.settings.set_theme(self.theme.edit.currentIndex())
        self.settings.set_code_font(self.code_font.edit.currentText())
        # For some reason Qt doesn't save booleans correctly
        # resorting to int
        autocomplete = 1 if self.autocomplete.edit.isChecked() else 0
        self.settings.set_event_autocomplete(autocomplete)
        crash_log_setting = 1 if self.crashlog.edit.isChecked() else 0
        self.settings.set_should_display_crash_logs(crash_log_setting)
        save_chunks_setting = 1 if self.savechunks.edit.isChecked() else 0
        self.settings.set_should_save_as_chunks(save_chunks_setting)
        auto_open_setting = 1 if self.autoopen.edit.isChecked() else 0
        self.settings.set_auto_open(auto_open_setting)
        save_backup_setting = 1 if self.savebackup.edit.isChecked() else 0
        self.settings.set_should_make_backup_save(save_backup_setting)
        autosave = float(self.autosave.edit.value())
        self.settings.set_autosave_time(autosave)
        self.settings.set_editor_close_button(self.saved_preferences['editor_close_button'])
        super().accept()

    def reject(self):
        super().reject()
