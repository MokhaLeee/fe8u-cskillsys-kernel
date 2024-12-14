#! /usr/bin/env python3
# -*- coding: utf-8 -*-
from __future__ import annotations

from enum import Enum
from typing import Dict, Optional

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QPalette

from app.editor.settings.main_settings_controller import MainSettingsController
from app.engine.item_system import text_color

WHITE = QColor(255, 255, 255)
BLACK = QColor(0, 0, 0)
RED = QColor(255, 0, 0)
PRIMARY = QColor(53, 53, 53, 232)
SECONDARY = QColor(35, 35, 35, 232)
TERTIARY = QColor(42, 130, 218, 232)
HIGHLIGHT = QColor(84, 85, 81, 232)
DISABLED = QColor(128, 128, 128)

DISPRIMARY = QColor(54, 57, 63)
DISSECONDARY = QColor(47, 49, 54)
#DISTERTIARY = QColor(57, 60, 67)
DISTERTIARY = QColor(78, 82, 92)
DISHIGHLIGHT = QColor(32, 34, 37)
GRAY = QColor(230, 230, 225)
DARKGRAY = QColor(188, 188, 188)

TRANSPRIMARY = QColor(53, 53, 53, 232)
TRANSSECONDARY = QColor(35, 35, 35, 232)
TRANSTERTIARY = QColor(42, 130, 218, 232)
TRANSHIGHLIGHT = QColor(84, 85, 81, 232)

BLUEPRIMARY = QColor(81, 95, 107, 232)
BLUESECONDARY = QColor(69, 66, 89, 232)
BLUETERTIARY = QColor(128, 143, 137, 232)
BLUEHIGHLIGHT = QColor(248, 248, 240, 232)

# SKYPRIMARY = QColor(50, 150, 200, 232)
SKYPRIMARY = QColor(44, 44, 119, 232)
SKYSECONDARY = QColor(75, 125, 200, 232)
SKYTERTIARY = QColor(25, 25, 100, 245)
SKYHIGHLIGHT = QColor(100, 175, 255, 232)

PURPLEPRIMARY = QColor(50, 50, 100, 232)
PURPLESECONDARY = QColor(100, 100, 125, 232)
PURPLETERTIARY = QColor(28, 12, 28, 245)
PURPLEHIGHLIGHT = QColor(175, 175, 200, 232)

base_palette = QPalette()

def css_rgb(color, a=False):
    """Get a CSS `rgb` or `rgba` string from a `QtGui.QColor`."""
    return ("rgba({}, {}, {}, {})" if a else "rgb({}, {}, {})").format(*color.getRgb())

class EventSyntaxHighlightingPalette():
    line_number_color: QColor
    func_color: QColor
    comment_color: QColor
    error_underline_color: QColor
    text_color: QColor
    special_text_color: QColor
    special_func_color: QColor

class PythonSyntaxHighlightingPalette():
    keyword: QColor
    alt_keyword: QColor
    brace: QColor
    deffunc: QColor
    defclass: QColor
    string: QColor
    string2: QColor
    comment: QColor
    numbers: QColor
    command: QColor
    flag: QColor

class QLightPalette(QPalette):
    """Dark palette for a Qt application meant to be used with the Fusion theme."""
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = DARKGRAY

    def set_stylesheet(self, app):
        """Static method to set the tooltip stylesheet to a `QtWidgets.QApplication`."""
        app.custom_style_sheet = ""
        tooltip = ("QToolTip {{"
                   "color: {white};"
                   "background-color: {tertiary};"
                   "border: 1px solid {white};"
                   "}}".format(white=css_rgb(WHITE), tertiary=css_rgb(TERTIARY)))
        scrollbar = (
            "QScrollBar:vertical {{"
            "    border: 0px solid {secondary};"
            "    background: transparent;"
            "    width: 8px;    "
            "    margin: 0px 0px 0px 0px;"
            "}}"
            "QScrollBar:horizontal {{"
            "    border: 0px solid {secondary};"
            "    background: transparent;"
            "    height: 8px;    "
            "    margin: 0px 0px 0px 0px;"
            "}}"
            "QScrollBar::handle {{"
            "    background: {secondary};"
            "    border: 1px solid {secondary};"
            "    border-radius: 4px;"
            "}}"
            "QScrollBar::add-line:vertical {{"
            "    height: 0px;"
            "    subcontrol-position: bottom;"
            "    subcontrol-origin: margin;"
            "}}"
            "QScrollBar::sub-line:vertical {{"
            "    height: 0 px;"
            "    subcontrol-position: top;"
            "    subcontrol-origin: margin;"
            "}}"
            "QScrollBar::add-line:horizontal {{"
            "    width: 0px;"
            "    subcontrol-position: right;"
            "    subcontrol-origin: margin;"
            "}}"
            "QScrollBar::sub-line:horizontal {{"
            "    width: 0 px;"
            "    subcontrol-position: left;"
            "    subcontrol-origin: margin;"
            "}}".format(white=css_rgb(WHITE), secondary=css_rgb(self.highlight_color)))

        # app.custom_style_sheet += tooltip
        app.custom_style_sheet += scrollbar

    def set_app(self, app):
        """Set the Fusion theme and this palette to a `QtWidgets.QApplication`."""
        app.setStyle("Fusion")
        # app.setPalette(self)
        app.setPalette(app.style().standardPalette())
        self.set_stylesheet(app)

    def background_png(self) -> Optional[str]:
        return None

    def icon_dir(self) -> str:
        return 'icons/icons'

    def event_syntax_highlighting(self) -> EventSyntaxHighlightingPalette:
        palette = EventSyntaxHighlightingPalette()
        palette.line_number_color = Qt.darkGray
        palette.func_color = QColor(52, 103, 174)
        palette.comment_color = Qt.darkGray
        palette.error_underline_color = Qt.red
        palette.text_color = QColor(63, 109, 58)
        palette.special_text_color = Qt.darkMagenta
        palette.special_func_color = Qt.red
        return palette

    def python_syntax_highlighting(self) -> PythonSyntaxHighlightingPalette:
        palette = PythonSyntaxHighlightingPalette()
        # from https://github.com/enkia/tokyo-night-vscode-theme
        # Tokyo Night Light
        palette.keyword = QColor('#8c4351')
        palette.alt_keyword = QColor('#5a4a78')
        palette.brace = QColor('#8f5e15')
        palette.deffunc = QColor('#34548a')
        palette.defclass = QColor('#33635c')
        palette.string = QColor('#485e30')
        palette.string2 = QColor('#485e30')
        palette.comment = QColor('#9699a3')
        palette.numbers = QColor('#965027')
        palette.command = QColor('#166775')
        palette.flag = QColor("#634f30")
        return palette

class QDarkPalette(QLightPalette):
    """Dark palette for a Qt application meant to be used with the Fusion theme."""
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = HIGHLIGHT

        # Set all the colors based on the constants in globals
        self.setColor(QPalette.Window, PRIMARY)
        self.setColor(QPalette.WindowText, WHITE)
        self.setColor(QPalette.Base, SECONDARY)
        self.setColor(QPalette.AlternateBase, PRIMARY)
        self.setColor(QPalette.ToolTipBase, WHITE)
        self.setColor(QPalette.ToolTipText, WHITE)
        self.setColor(QPalette.Text, WHITE)
        self.setColor(QPalette.Button, PRIMARY)
        self.setColor(QPalette.ButtonText, WHITE)
        self.setColor(QPalette.BrightText, RED)
        self.setColor(QPalette.Link, TERTIARY)
        self.setColor(QPalette.Highlight, TERTIARY)
        self.setColor(QPalette.HighlightedText, BLACK)

        self.setColor(QPalette.Disabled, QPalette.WindowText, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.Text, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.ButtonText, DISABLED)

    def set_app(self, app):
        """Set the Fusion theme and this palette to a `QtWidgets.QApplication`."""
        app.setStyle("Fusion")
        app.setPalette(self)
        self.set_stylesheet(app)

    def icon_dir(self) -> str:
        return 'icons/dark_icons'

    def event_syntax_highlighting(self) -> EventSyntaxHighlightingPalette:
        palette = EventSyntaxHighlightingPalette()
        palette.line_number_color = QColor(144, 144, 138)
        palette.func_color = QColor(102, 217, 239)
        palette.comment_color = QColor(117, 113, 94)
        palette.error_underline_color = QColor(249, 38, 114)
        palette.text_color = QColor(230, 219, 116)
        palette.special_text_color = QColor(174, 129, 255)
        palette.special_func_color = (249, 38, 114)
        return palette

    def python_syntax_highlighting(self) -> PythonSyntaxHighlightingPalette:
        palette = PythonSyntaxHighlightingPalette()
        # from https://github.com/enkia/tokyo-night-vscode-theme
        # Tokyo Night
        palette.keyword = QColor('#f7768e')
        palette.alt_keyword = QColor('#bb9af7')
        palette.brace = QColor('#e0af68')
        palette.deffunc = QColor('#7aa2f7')
        palette.defclass = QColor('#c0caf5')
        palette.string = QColor('#9ece6a')
        palette.string2 = QColor('#9ece6a')
        palette.comment = QColor('#565f89')
        palette.numbers = QColor('#ff9e64')
        palette.command = QColor('#2ac3de')
        palette.flag = QColor('#a9b1d6')
        return palette

class QDiscordPalette(QDarkPalette):
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = DISHIGHLIGHT

        # Set all the colors based on the constants in globals
        self.setColor(QPalette.Window, DISPRIMARY)
        self.setColor(QPalette.WindowText, GRAY)
        self.setColor(QPalette.Base, DISSECONDARY)
        self.setColor(QPalette.AlternateBase, DISPRIMARY)
        self.setColor(QPalette.ToolTipBase, WHITE)
        self.setColor(QPalette.ToolTipText, GRAY)
        self.setColor(QPalette.Text, GRAY)
        self.setColor(QPalette.Button, DISPRIMARY)
        self.setColor(QPalette.ButtonText, WHITE)
        self.setColor(QPalette.BrightText, RED)
        self.setColor(QPalette.Link, TERTIARY)
        self.setColor(QPalette.Highlight, DISTERTIARY)
        self.setColor(QPalette.HighlightedText, WHITE)

        self.setColor(QPalette.Disabled, QPalette.WindowText, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.Text, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.ButtonText, DISABLED)

class QSiderealPalette(QDarkPalette):
    """Dark palette for a Qt application meant to be used with the Fusion theme."""
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = TRANSHIGHLIGHT

        # Set all the colors based on the constants in globals
        self.setColor(QPalette.Window, TRANSPRIMARY)
        self.setColor(QPalette.WindowText, WHITE)
        self.setColor(QPalette.Base, TRANSSECONDARY)
        self.setColor(QPalette.AlternateBase, TRANSPRIMARY)
        self.setColor(QPalette.ToolTipBase, WHITE)
        self.setColor(QPalette.ToolTipText, WHITE)
        self.setColor(QPalette.Text, WHITE)
        self.setColor(QPalette.Button, TRANSPRIMARY)
        self.setColor(QPalette.ButtonText, WHITE)
        self.setColor(QPalette.BrightText, RED)
        self.setColor(QPalette.Link, TRANSTERTIARY)
        self.setColor(QPalette.Highlight, TRANSTERTIARY)
        self.setColor(QPalette.HighlightedText, BLACK)

        self.setColor(QPalette.Disabled, QPalette.WindowText, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.Text, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.ButtonText, DISABLED)

    def background_png(self) -> Optional[str]:
        return 'icons/bg.png'

class QMistPalette(QDarkPalette):
    """Dark palette for a Qt application meant to be used with the Fusion theme."""
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = BLUEHIGHLIGHT

        # Set all the colors based on the constants in globals
        self.setColor(QPalette.Window, BLUEPRIMARY)
        self.setColor(QPalette.WindowText, WHITE)
        self.setColor(QPalette.Base, BLUESECONDARY)
        self.setColor(QPalette.AlternateBase, BLUEPRIMARY)
        self.setColor(QPalette.ToolTipBase, WHITE)
        self.setColor(QPalette.ToolTipText, WHITE)
        self.setColor(QPalette.Text, WHITE)
        self.setColor(QPalette.Button, BLUEPRIMARY)
        self.setColor(QPalette.ButtonText, WHITE)
        self.setColor(QPalette.BrightText, RED)
        self.setColor(QPalette.Link, BLUETERTIARY)
        self.setColor(QPalette.Highlight, BLUETERTIARY)
        self.setColor(QPalette.HighlightedText, BLACK)

        self.setColor(QPalette.Disabled, QPalette.WindowText, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.Text, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.ButtonText, DISABLED)

    def background_png(self) -> Optional[str]:
        return 'icons/bg2.png'

class QSkyPalette(QLightPalette):
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = SKYHIGHLIGHT

        # Set all the colors based on the constants in globals
        self.setColor(QPalette.Window, SKYSECONDARY)
        self.setColor(QPalette.WindowText, WHITE)
        self.setColor(QPalette.Base, SKYTERTIARY)
        self.setColor(QPalette.AlternateBase, SKYPRIMARY)
        self.setColor(QPalette.ToolTipBase, WHITE)
        self.setColor(QPalette.ToolTipText, WHITE)
        self.setColor(QPalette.Text, WHITE)
        self.setColor(QPalette.Button, SKYSECONDARY)
        self.setColor(QPalette.ButtonText, WHITE)
        self.setColor(QPalette.BrightText, RED)
        self.setColor(QPalette.Link, TERTIARY)
        self.setColor(QPalette.Highlight, SKYHIGHLIGHT)
        self.setColor(QPalette.HighlightedText, WHITE)

        self.setColor(QPalette.Disabled, QPalette.WindowText, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.Text, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.ButtonText, DISABLED)

    def set_app(self, app):
        """Set the Fusion theme and this palette to a `QtWidgets.QApplication`."""
        app.setStyle("Fusion")
        app.setPalette(self)
        self.set_stylesheet(app)

    def background_png(self) -> Optional[str]:
        return 'icons/bg3.png'

class QPurplePalette(QLightPalette):
    def __init__(self, *__args):
        super().__init__(*__args)
        self.highlight_color = PURPLEHIGHLIGHT

        # Set all the colors based on the constants in globals
        self.setColor(QPalette.Window, PURPLESECONDARY)
        self.setColor(QPalette.WindowText, WHITE)
        self.setColor(QPalette.Base, PURPLETERTIARY)
        self.setColor(QPalette.AlternateBase, PURPLESECONDARY)
        self.setColor(QPalette.ToolTipBase, WHITE)
        self.setColor(QPalette.ToolTipText, WHITE)
        self.setColor(QPalette.Text, WHITE)
        self.setColor(QPalette.Button, PURPLEPRIMARY)
        self.setColor(QPalette.ButtonText, WHITE)
        self.setColor(QPalette.BrightText, RED)
        self.setColor(QPalette.Link, TERTIARY)
        self.setColor(QPalette.Highlight, PURPLEHIGHLIGHT)
        self.setColor(QPalette.HighlightedText, WHITE)

        self.setColor(QPalette.Disabled, QPalette.WindowText, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.Text, DISABLED)
        self.setColor(QPalette.Disabled, QPalette.ButtonText, DISABLED)

    def set_app(self, app):
        """Set the Fusion theme and this palette to a `QtWidgets.QApplication`."""
        app.setStyle("Fusion")
        app.setPalette(self)
        self.set_stylesheet(app)

    def background_png(self) -> Optional[str]:
        return 'icons/bg4.png'

class ThemeType(Enum):
    Light = 0
    Dark = 1
    Discord = 2
    Sidereal = 3
    Mist = 4
    Sky = 5
    Purple = 6

THEMES: Dict[ThemeType, QPalette] = {
    ThemeType.Light: QLightPalette(),
    ThemeType.Dark: QDarkPalette(),
    ThemeType.Discord: QDiscordPalette(),
    ThemeType.Sidereal: QSiderealPalette(),
    ThemeType.Mist: QMistPalette(),
    ThemeType.Sky: QSkyPalette(),
    ThemeType.Purple: QPurplePalette(),
}

def get_theme(theme: Optional[ThemeType] = None) -> QLightPalette:
    if not theme:
        settings = MainSettingsController()
        theme_idx = settings.get_theme(0)
        return THEMES[ThemeType(theme_idx)]
    return THEMES[theme]

def set(app, theme: ThemeType | QPalette):
    """
    Unfortunately for now, icon colors don't change until restart
    """
    if isinstance(theme, ThemeType):
        theme = get_theme(theme)
    theme.set_app(app)
    bg = theme.background_png()
    if bg:
        app.custom_style_sheet += "QDialog {background-image: url(%s)};" % bg
    else:
        app.custom_style_sheet += "QDialog {background-image: none};"

    app.setStyleSheet(app.custom_style_sheet)
