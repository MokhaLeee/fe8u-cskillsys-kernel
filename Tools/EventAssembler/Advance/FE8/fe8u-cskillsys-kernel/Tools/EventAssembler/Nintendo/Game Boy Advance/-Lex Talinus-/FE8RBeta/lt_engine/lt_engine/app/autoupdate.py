import shutil, sys, os
import urllib.request
from zipfile import ZipFile

remote_repo = r"https://gitlab.com/rainlash/lt-maker/-/releases/permalink/latest/downloads/lex_talionis_maker"
remote_latest = r"https://gitlab.com/rainlash/lt-maker/-/releases/permalink/latest"
metadata = "version.txt"

def check_version(a: str, b: str) -> bool:
    """
    Returns True if a > b, False otherwise
    """
    a = a.replace('.', '').replace('-', '')
    b = b.replace('.', '').replace('-', '')
    return a != b

def download_url(url, save_path):
    with urllib.request.urlopen(url) as dl_file:
        with open(save_path, 'wb') as out_file:
            out_file.write(dl_file.read())

def get_redirected_url(url): # https://stackoverflow.com/questions/5538280/determining-redirected-url-in-python
    opener = urllib.request.build_opener(urllib.request.HTTPRedirectHandler)
    request = opener.open(url)
    return request.url

def check_for_update() -> bool:
    version_url = get_redirected_url(remote_latest)
    version_num = version_url.index("releases/") + len("releases/") # Will point to start of release ID
    new_version = version_url[version_num:]
    print(new_version, version_url)

    if os.path.exists(metadata):
        with open(metadata) as fp:
            lines = [line.strip() for line in fp.readlines()]
            cur_version = lines[0]

        print(new_version)
        print(cur_version)
        if check_version(new_version, cur_version):
            print("Needs update! %s %s" % (new_version, cur_version))
            return True
        else:
            print("Does not need update! %s %s" % (new_version, cur_version))
            return False
    else:
        print("Cannot find version.txt, so needs update: %s!" % metadata)
        return True

def copy_and_overwrite(src, dst):
    if os.path.exists(dst):
        shutil.rmtree(dst)
    shutil.copytree(src, dst)

# Update all files -- probably buggy as hell
def update() -> bool:
    print("Executable: %s" % sys.executable)
    local = os.path.dirname(sys.executable)
    print("Local: %s %s" % (local, os.path.abspath(local)))
    cwd = os.path.abspath(os.getcwd())
    print("Current working directory: %s" % cwd)
    # Make backup of current working directory
    print("Making backup of current working directory...")
    current_backup = cwd + '.tmp'
    shutil.copytree(cwd, current_backup)

    print("Starting Process! %s" % remote_repo)
    remote_zip = r"remote_tmp.zip"
    download_url(remote_repo, remote_zip)
    print(os.path.abspath(remote_zip))
    remote_dir = os.path.abspath(remote_zip.replace('.zip', '/'))
    print(remote_dir)
    try:
        with ZipFile(remote_zip, 'r') as z:
            print("Extracting...")
            z.extractall(remote_dir)
        print("Done extracting to %s" % os.path.abspath(remote_dir))
    except OSError as e:
        print("Failed to fully unzip remote %s to %s! %s" % (remote_zip, remote_dir, e))
        return False

    print("Deleting zip")
    try:
        os.remove(remote_zip)
    except OSError as e:
        print("Failed to delete zip %s! %s" % (remote_zip, e))
        return False

    true_remote_dir = os.path.join(remote_dir, 'lt_editor', 'lt_editor')
    potential_changes = [
        'lt_editor.exe',
        'VCRunTIME.dll',
        'base_library.zip',
        'app/',
        'default.ltproj/',
        'icons/',
        'locale/',
        'resources/',
        'sprites/'
    ]

    try:
        # diff current_backup and remote_dir
        # import filecmp
        # new = os.path.join(remote_dir, 'lt_editor', 'lt_editor')
        # print(new)
        # diff = filecmp.dircmp(new, current_backup)
        # print("*** Diff Report ***")
        # diff.report()
        # print("*** Diff Full Report ***")
        # diff.report_full_closure()
        
        for fn in potential_changes:
            zip_path = os.path.join(true_remote_dir, fn)
            old_path = os.path.join(cwd, fn)
            print("Copying %s to %s..." % (zip_path, old_path))
            if os.path.isdir(old_path):
                copy_and_overwrite(zip_path, old_path)
            else:
                shutil.copy(zip_path, old_path)

    except OSError as e:
        print("Failed to completely upgrade files when copying %s to %s! %s" % (true_remote_dir, cwd, e))
        print("Replacing with backup...")
        for fn in potential_changes:
            backup_path = os.path.join(current_backup, fn)
            your_path = os.path.join(cwd, fn)
            print("Copying %s to %s..." % (backup_path, your_path))
            if os.path.isdir(your_path):
                copy_and_overwrite(backup_path, your_path)
            else:
                shutil.copy(backup_path, your_path)
        return False

    finally:
        shutil.rmtree(remote_dir)
        shutil.rmtree(current_backup)
    return True
    
if __name__ == '__main__':
    check_for_update()
