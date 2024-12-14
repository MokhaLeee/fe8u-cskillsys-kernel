import json
import os
from typing import List, Tuple

from PyQt5.QtCore import QSettings, QDir
from PyQt5.QtCore import Qt

from .project_history_controller import ProjectHistoryController, ProjectHistoryEntry
from .component_settings_controller import ComponentSettingsController


class MainSettingsController():
    """
    Provides an interface for interacting with editor settings.
    Contains general application-wide settings. Also contains
    specific setting controllers for more tailored settings.
    """

    def __init__(self, company='rainlash', product='Lex Talionis'):
        QSettings.setDefaultFormat(QSettings.IniFormat)
        self.state = QSettings(company, product)
        self.component_controller = ComponentSettingsController(
            company, product)
        self.project_history_controller = ProjectHistoryController(
            company, product)

    def fileName(self):
        return self.state.fileName()

    """========== General Settings =========="""

    def set_current_project(self, value):
        self.state.setValue("current_proj", value)

    def get_current_project(self, fallback=""):
        return self.state.value("current_proj", fallback, type=str)

    def set_last_open_path(self, value):
        self.state.setValue("last_open_path", value)

    def get_last_open_path(self, fallback=""):
        if not fallback:
            fallback = QDir.currentPath()
        return str(self.state.value("last_open_path", fallback, type=str))

    def append_or_bump_project(self, project_name: str, project_path: str):
        self.project_history_controller.append_or_bump_project(
            project_name, project_path)

    def get_last_ten_projects(self) -> List[ProjectHistoryEntry]:
        return self.project_history_controller.get_last_ten_projects()

    """========== General UI Settings =========="""

    def set_theme(self, value):
        self.state.setValue("theme", value)

    def get_theme(self, fallback=0):
        return self.state.value("theme", fallback, type=int)

    def set_code_font(self, value):
        self.state.setValue("code_font", value)

    def get_code_font(self, fallback='Courier New'):
        return self.state.value("code_font", fallback, type=str)

    def set_event_autocomplete(self, value):
        self.state.setValue("event_autocomplete", value)

    def get_event_autocomplete(self, fallback=True):
        return self.state.value("event_autocomplete", fallback, type=bool)

    def set_autosave_time(self, value):
        self.state.setValue("autosave_time", value)

    def get_autosave_time(self, fallback=5):
        return float(self.state.value("autosave_time", fallback, type=float))

    def set_should_display_crash_logs(self, value):
        self.state.setValue("should_display_crash_logs", value)

    def get_should_display_crash_logs(self, fallback=True):
        return self.state.value("should_display_crash_logs", fallback, type=bool)

    def set_should_make_backup_save(self, value):
        self.state.setValue("should_make_backup_save", value)

    def get_should_make_backup_save(self, fallback=False):
        return self.state.value("should_make_backup_save", fallback)

    def set_should_save_as_chunks(self, value):
        self.state.setValue("should_save_as_chunks", value)

    def get_should_save_as_chunks(self, fallback=True):
        return self.state.value("should_save_as_chunks", fallback)

    def set_auto_open(self, value):
        self.state.setValue('auto_open', value)

    def get_auto_open(self, fallback=False):
        return self.state.value('auto_open', fallback)

    """========== General Control Settings =========="""

    def set_place_button(self, value):
        self.state.setValue("place_button", value)

    def get_place_button(self, fallback=None):
        return self.state.value('place_button', fallback, type=Qt.MouseButton)

    def set_select_button(self, value):
        self.state.setValue('select_button', value)

    def get_select_button(self, fallback=None):
        return self.state.value('select_button', fallback, type=Qt.MouseButton)

    def set_autocomplete_button(self, value):
        self.state.setValue('autocomplete_button', value)

    def get_autocomplete_button(self, fallback=None):
        return self.state.value('autocomplete_button', fallback, type=Qt.Key)

    def set_editor_close_button(self, value):
        self.state.setValue('editor_close_button', value)

    def get_editor_close_button(self, fallback=Qt.Key_Escape):
        return self.state.value('editor_close_button', fallback, type=Qt.Key)
