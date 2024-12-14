import os
import shutil
import filecmp
import json
from typing import List, Type, TypeVar

from app.utilities.data import Data

import logging

B = TypeVar('B')
class BaseResourceCatalog(Data[B]):
    def load(self, loc):
        for root, dirs, files in os.walk(loc):
            for name in files:
                if name.endswith(self.filetype):
                    full_path = os.path.join(root, name)
                    nid = name[:-len(self.filetype)]
                    new_resource = self.datatype(nid, full_path)
                    self.append(new_resource)

M = TypeVar('M')
class ManifestCatalog(Data[M]):
    filetype = '.png'
    manifest = None  # To be implemented
    title = ''  # To be implemented
    datatype: Type[M] = None  # To be implemented
    multi_loc = None  # Can be implemented for distributed saving

    def load(self, loc):
        resource_dict = self.read_manifest(os.path.join(loc, self.manifest))
        for s_dict in resource_dict:
            new_resource = self.datatype.restore(s_dict)
            new_resource.set_full_path(os.path.join(loc, new_resource.nid + self.filetype))
            self.append(new_resource)

    def read_manifest(self, fn: str) -> dict:
        datum = {}
        if os.path.exists(fn):
            with open(fn) as load_file:
                datum = json.load(load_file)
        return datum

    def dump(self, loc):
        save = [datum.save() for datum in self]
        save_loc = os.path.join(loc, self.manifest)
        with open(save_loc, 'w') as serialize_file:
            json.dump(save, serialize_file, indent=4)

    def save(self, loc):
        for datum in self:
            new_full_path = os.path.join(loc, datum.nid + self.filetype)
            if os.path.abspath(datum.full_path) != os.path.abspath(new_full_path):
                self.make_copy(datum.full_path, new_full_path)
                datum.set_full_path(new_full_path)
        self.dump(loc)

    def make_copy(self, old_full_path, new_full_path):
        if os.path.exists(old_full_path):
            if os.path.exists(new_full_path) and filecmp.cmp(old_full_path, new_full_path, shallow=False):
                pass  # Identical files
            else:
                shutil.copy(old_full_path, new_full_path)
        else:
            logging.warning("%s does not exist" % old_full_path)

    def valid_files(self) -> set:
        return {datum.nid + self.filetype for datum in self}

    def get_unused_files(self, loc: str) -> List[str]:
        unused_files = []
        valid_filenames = self.valid_files()
        valid_filenames.add(self.manifest)  # also include the manifest file otherwise it would be deleted
        valid_filenames.add(self.multi_loc)
        for fn in os.listdir(loc):
            if fn not in valid_filenames:
                full_fn = os.path.normpath(os.path.join(loc, fn))
                unused_files.append(full_fn)
        return unused_files

    def clean(self, bad_files: List[str]):
        for fn in bad_files:
            logging.warning("Removing %s..." % fn)
            os.remove(fn)
