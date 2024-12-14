from PyQt5.QtWidgets import QMenu, QToolTip
from PyQt5.QtCore import QEvent

class QHelpMenu(QMenu):
    def event(self, event):
        if event.type() == QEvent.ToolTip and self.activeAction():
            QToolTip.showText(event.globalPos(), self.activeAction().toolTip())
        else:
            QToolTip.hideText()
        return super().event(event)
