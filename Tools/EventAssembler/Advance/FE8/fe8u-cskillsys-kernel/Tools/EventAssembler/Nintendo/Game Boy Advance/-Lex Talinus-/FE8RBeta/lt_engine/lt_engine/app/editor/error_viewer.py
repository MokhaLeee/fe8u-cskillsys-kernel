import sys
from typing import Callable

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QTextEdit, QVBoxLayout, QWidget, QPushButton
from app.data.database.database import DB
from app.data.resources.resources import RESOURCES

from app.data.validation.db_validation import DBChecker, ValidationResult
from app.editor.settings.main_settings_controller import MainSettingsController

MAX_NUM_CHARS = 100000

def generate_header(title: str):
    bars = '=' * len(title)
    return f"{bars}\n{title}\n{bars}\n"

class ErrorViewer(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle("Error report")
        self.resize(800, 800)

        self.settings = MainSettingsController()
        self.textEdit = QTextEdit()
        self.textEdit.setReadOnly(True)
        self.textEdit.setFontFamily(self.settings.get_code_font())
        self.textEdit.setFontPointSize(12)
        self.textEdit.setLineWrapMode(QTextEdit.LineWrapMode.WidgetWidth)

        self.refresh_errors_button = QPushButton("Regenerate Error Report")
        self.refresh_errors_button.clicked.connect(self.regenerate_errors)

        layout = QVBoxLayout()
        layout.addWidget(self.textEdit)
        layout.addWidget(self.refresh_errors_button)
        self.setLayout(layout)

        self.regenerate_errors()

    def regenerate_errors(self):
        text_body = ""

        dbchecker = DBChecker(DB, RESOURCES)

        def create_error_log(category_name, validation_func: Callable[[], ValidationResult]):
            errors = validation_func()
            log = ''
            if errors.errors:
                errors_as_str = '\n\n'.join(str(error) for error in errors.errors)
                log += generate_header(f"{category_name} ERRORS") + errors_as_str + '\n\n'
            if errors.warnings:
                warnings_as_str = '\n\n'.join(str(warn) for warn in errors.warnings)
                log += generate_header(f"{category_name} WARNINGS") + warnings_as_str + '\n\n'
            return log

        text_body += create_error_log('EVENT', dbchecker.validate_events)
        text_body += create_error_log('UNIT', dbchecker.validate_units)
        text_body += create_error_log('SKILL AND ITEM', dbchecker.validate_items_and_skills)
        text_body += create_error_log('LEVEL', dbchecker.validate_levels)

        if not text_body:
            text_body = generate_header("No errors in project!")
        self.textEdit.setText(text_body)

def show_error_report():
    dlg = ErrorViewer()
    dlg.show()
    return dlg

# python -m app.editor.error_viewer
if __name__ == '__main__':
    app = QApplication(sys.argv)
    win = ErrorViewer()
    win.show()
    sys.exit(app.exec_())