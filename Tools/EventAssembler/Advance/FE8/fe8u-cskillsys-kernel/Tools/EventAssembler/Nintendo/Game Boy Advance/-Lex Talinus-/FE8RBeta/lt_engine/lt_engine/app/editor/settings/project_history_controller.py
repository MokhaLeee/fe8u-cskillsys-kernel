from datetime import datetime
from PyQt5.QtCore import QSettings
from typing import List, Optional, Tuple

import os
import json


class ProjectHistoryEntry():
    def __init__(self, name: str, path: str, last_open_time: Optional[int] = None):
        self.name = name
        self.path = path
        self._last_open_time = last_open_time

    def get_last_open_time(self) -> Optional[datetime]:
        if not self._last_open_time:
            return None
        return datetime.fromtimestamp(self._last_open_time)

    def to_serializable(self):
        return (self.name, self.path, self._last_open_time)

    def __str__(self):
        return str((self.name, self.path, self._last_open_time))

    def __repr__(self) -> str:
        return self.__str__()


class ProjectHistoryController():
    """
    Provides an interface for interacting with geometry
    settings for the application.
    """

    def __init__(self, company='rainlash', product='Lex Talionis'):
        QSettings.setDefaultFormat(QSettings.IniFormat)
        self.state = QSettings(company, product)

    def append_or_bump_project(self, project_name: str, project_path: str):
        # most_recent_projects contains a json string like so:
        # "[['PROJ_NAME', 'PATH', MillisSinceEpoch], ...]"
        # which are the args to ProjectHistoryEntry
        current_list: List[Tuple[str, str, int]] = json.loads(self.state.value(
            "most_recent_projects", "[]", type=str))
        filtered_list: List[ProjectHistoryEntry] = []
        for project_entry in current_list:
            path = project_entry[1]
            if not path == project_path:
                filtered_list.append(ProjectHistoryEntry(*project_entry))
        filtered_list.insert(0, ProjectHistoryEntry(
            project_name, project_path, datetime.now().timestamp()))
        # only stores 10 projects
        filtered_list = filtered_list[:10]
        serializable_list = [entry.to_serializable()
                             for entry in filtered_list]
        self.state.setValue('most_recent_projects',
                            json.dumps(serializable_list))

    def get_last_ten_projects(self) -> List[ProjectHistoryEntry]:
        current_list = json.loads(self.state.value(
            'most_recent_projects', "[]", type=str))
        filtered_list: List[ProjectHistoryEntry] = []
        for project_entry in current_list:
            path = project_entry[1]
            if os.path.isdir(path):
                filtered_list.append(ProjectHistoryEntry(*project_entry))
        return filtered_list
