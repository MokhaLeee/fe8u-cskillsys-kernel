import os

try:
    import cPickle as pickle
except ImportError:
    import pickle

from PyQt5.QtWidgets import QVBoxLayout, QDialog, QTextEdit
from PyQt5.QtGui import QTextCursor
from app.extensions.custom_gui import PropertyBox, ComboBox, Dialog

import logging

class SaveViewer(Dialog):
    def __init__(self, saves, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Choose Save")
        self.window = parent

        layout = QVBoxLayout()
        self.setLayout(layout)

        # Sort saves by last modified time
        saves = sorted(saves, key=lambda s: os.path.getmtime(s), reverse=True)
        self.save_box = PropertyBox("Save", ComboBox, self)
        self.save_box.edit.addItems(saves)
        self.save_box.edit.setCurrentIndex(0)
        self.save_box.edit.activated.connect(self.save_changed)
        layout.addWidget(self.save_box)

        self.display_box = PropertyBox("Info", QTextEdit, self)
        layout.addWidget(self.display_box)

        layout.addWidget(self.buttonbox)

        self.save_changed()

    def save_changed(self):
        try:
            save_loc = self.save_box.edit.currentText()
            meta_loc = save_loc + 'meta'
            with open(save_loc, 'rb') as fp:
                s_dict = pickle.load(fp)
            with open(meta_loc, 'rb') as fp:
                meta_dict = pickle.load(fp)
        except Exception as e:
            logging.error("Can not load %s save file: %s" % (save_loc, e))
            s_dict, meta_dict = None, None

        self.display_box.edit.clear()
        if meta_dict and s_dict:
            level_nid = meta_dict['level_nid']
            level_name = meta_dict['level_title']
            time = meta_dict.get('time')
            
            text = 'Level %s: %s\n' % (level_nid, level_name)
            self.display_box.edit.insertPlainText(text)
            if time:
                text = 'Saved: %s\n' % time
                self.display_box.edit.insertPlainText(text)
            party_nid = s_dict['current_party']
            self.display_box.edit.insertPlainText("Party: %s\n" % party_nid)
            self.display_box.edit.insertPlainText("Units:\n")
            item_registry = {i['uid']: i['nid'] for i in s_dict['items']}
            for unit in s_dict['units']:
                if not unit['dead'] and unit['team'] == 'player':
                    items = ', '.join(item_registry.get(item) for item in unit['items'])
                    wlvl = ', '.join('%s: %s' % (k, v) for k, v in unit['wexp'].items() if v > 0)
                    unit_text = '%s Lv %d Exp %d Wlvl %s Items: %s\n' % (unit['nid'], unit['level'], unit['exp'], wlvl, items)

                    self.display_box.edit.insertPlainText(unit_text)
            # party = [party for party in s_dict['parties'] if party['nid'] == party_nid][0]
            # convoy_items = ', '.join(item for item in party['convoy'])
            # self.display_box.edit.insertPlainText("Convoy:\n")
            # self.display_box.edit.insertPlainText(convoy_items)
            # self.display_box.edit.insertPlainText('\n')
        else:
            self.display_box.edit.insertPlainText("Old or Corrupted save file!\nDo not use!")

        self.display_box.edit.moveCursor(QTextCursor.Start)
        self.display_box.edit.ensureCursorVisible()

    @classmethod
    def get(cls, saves, parent=None):
        dialog = cls(saves, parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            return dialog.save_box.edit.currentText()
        else:
            return None
