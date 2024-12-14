from __future__ import annotations

import os
from typing import List, Optional

from PyQt5.QtCore import QAbstractTableModel, QDir, QModelIndex, Qt
from PyQt5.QtWidgets import (QFileDialog, QHBoxLayout, QHeaderView,
                             QMessageBox, QPushButton, QTableView, QVBoxLayout)

from app.editor.file_manager.project_initializer import ProjectInitializer
from app.editor.settings.main_settings_controller import MainSettingsController
from app.editor.settings.project_history_controller import ProjectHistoryEntry
from app.extensions.custom_gui import SimpleDialog

class RecentProjectsModel(QAbstractTableModel):
    def __init__(self, data: List[ProjectHistoryEntry]):
        super(RecentProjectsModel, self).__init__()
        self._data = data
        self.header_labels = ["Project Title",
                              "Project Full Path", "Last Opened on..."]

    def data(self, index, role):
        if role == Qt.DisplayRole:
            return self.getColumnProperty(index, self._data[index.row()])
        return None

    def rowCount(self, index: QModelIndex):
        return len(self._data)

    def columnCount(self, index: QModelIndex):
        return 3

    def getColumnProperty(self, index: QModelIndex, entry: ProjectHistoryEntry) -> str:
        if index.column() == 0:
            return entry.name
        elif index.column() == 1:
            return entry.path
        elif index.column() == 2:
            return entry.get_last_open_time().strftime('%Y-%m-%d %a %H:%M:%S') if entry.get_last_open_time() else "Unknown"

    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role == Qt.DisplayRole and orientation == Qt.Horizontal:
            return self.header_labels[section]
        return QAbstractTableModel.headerData(self, section, orientation, role)


class RecentProjectDialog(SimpleDialog):
    def __init__(self, recent_projects: List[ProjectHistoryEntry], load_only):
        super().__init__()
        self.setWindowTitle("Recent Projects")
        # recent_projects guaranteed to not be empty
        self.projects = recent_projects
        self._selected_path: Optional[str] = None

        layout = QVBoxLayout()
        self.setLayout(layout)

        self.project_table = QTableView()
        self.project_table.horizontalHeader().setStretchLastSection(True)
        self.project_table.verticalHeader().hide()
        self.project_table.setShowGrid(False)
        self.model = RecentProjectsModel(self.projects)
        self.project_table.setModel(self.model)
        self.project_table.horizontalHeader().setSectionResizeMode(
            0, QHeaderView.ResizeToContents)
        self.project_table.horizontalHeader().setSectionResizeMode(
            1, QHeaderView.ResizeToContents)
        self.project_table.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.project_table.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.project_table.resizeColumnsToContents()
        self.project_table.setSelectionBehavior(QTableView.SelectRows)
        self.project_table.clicked.connect(self.on_select_project)
        self.project_table.doubleClicked.connect(self.on_double_click_project)
        self.project_table.selectRow(0)
        # select initial project
        self._selected_path = self.projects[0].path

        button_layout = QHBoxLayout()
        self.confirm_button = QPushButton("Open selected project", self)
        self.confirm_button.clicked.connect(self.on_click_confirm)
        button_layout.addWidget(self.confirm_button)
        self.open_other_button = QPushButton("Open other...", self)
        self.open_other_button.clicked.connect(self.on_click_open)
        button_layout.addWidget(self.open_other_button)
        if not load_only:
            self.new_button = QPushButton("Create New...", self)
            self.new_button.clicked.connect(self.on_click_new)
            button_layout.addWidget(self.new_button)

        layout.addWidget(self.project_table)
        layout.addLayout(button_layout)

        dialogWidth = self.project_table.horizontalHeader().length() + 24
        dialogHeight = self.project_table.verticalHeader().length() + 104
        self.setFixedSize(dialogWidth, dialogHeight)

    def on_select_project(self, index: QModelIndex):
        row = index.row()
        project = self.projects[row]
        self._selected_path = project.path

    def on_double_click_project(self, index: QModelIndex):
        row = index.row()
        project = self.projects[row]
        self._selected_path = project.path
        self.accept()

    def on_click_confirm(self):
        self.accept()

    def on_click_open(self):
        if self.projects:
            starting_path = os.path.join(self.projects[0].path, '..')
        else:
            starting_path = QDir.currentPath()
        fn = QFileDialog.getExistingDirectory(
            self, "Open Project Directory", starting_path)
        if fn:
            if not fn.endswith('.ltproj'):
                QMessageBox.warning(self, "Incorrect directory type",
                                    "%s is not an .ltproj." % fn)
            else:
                self._selected_path = fn
                self.accept()

    def on_click_new(self):
        project_initializer = ProjectInitializer()
        result = project_initializer.full_create_new_project()
        if result:
            _, _, path = result
            self._selected_path = path
            self.accept()

    def closeEvent(self, event):
        self._selected_path = None
        super().closeEvent(event)

    def get_selected(self) -> Optional[str]:
        return self._selected_path


def choose_recent_project(load_only: bool = False, allow_auto_open: bool = False) -> Optional[str]:
    """
    # str means go open that project at that path
    # None means don't do anything (When you press X or close on the dialog)
    """
    settings = MainSettingsController()
    recent_projects = settings.get_last_ten_projects()
    if not recent_projects:
        # default.ltproj is always an available choice
        recent_projects = [ProjectHistoryEntry("default.ltproj", "default.ltproj")]
    if allow_auto_open and settings.get_auto_open():
        return settings.get_current_project(fallback="default.ltproj")
    dialog = RecentProjectDialog(recent_projects, load_only)
    dialog.exec_()
    selected_path: Optional[str] = \
        dialog.get_selected()
    return selected_path
