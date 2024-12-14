from PyQt5.QtCore import QSettings

GEOMETRY_SETTING_PREFIX = "geometry_setting:"
STATE_SETTING_PREFIX = "state_setting:"
SORT_SETTING_PREFIX = "sort_setting:"

class ComponentSettingsController():
    """
    Provides an interface for interacting with geometry
    settings for the application.
    """
    def __init__(self, company='rainlash', product='Lex Talionis'):
        QSettings.setDefaultFormat(QSettings.IniFormat)
        self.state = QSettings(company, product)

    def set_geometry(self, editor_name, value):
        """Sets geometry settings for a specific editor

        Args:
            editor_name (str): class of object.
            value: geometry value.
        """
        self.state.setValue(GEOMETRY_SETTING_PREFIX + editor_name, value)

    def get_geometry(self, editor_name):
        """gets geometry settings for a specific editor

        Args:
            editor_name (str): class of object.
        """
        return self.state.value(GEOMETRY_SETTING_PREFIX + editor_name)

    def set_state(self, editor_name, value):
        """sets state settings for a specific editor

        Args:
            editor_name (str): class of object.
            value: value of state.
        """
        self.state.setValue(STATE_SETTING_PREFIX + editor_name, value)

    def get_state(self, editor_name):
        """gets state settings for a specific component

        Args:
            editor_name (str): class of object.
        """
        return self.state.value(STATE_SETTING_PREFIX + editor_name)

    def set_sort(self, editor_name, value):
        self.state.setValue(SORT_SETTING_PREFIX + editor_name, value)

    def get_sort(self, editor_name):
        return self.state.value(SORT_SETTING_PREFIX + editor_name)

    def set_property(self, editor_name, property_name, value):
        self.state.setValue(STATE_SETTING_PREFIX + editor_name + property_name, value)

    def get_property(self, editor_name, property_name):
        return self.state.value(STATE_SETTING_PREFIX + editor_name + property_name)