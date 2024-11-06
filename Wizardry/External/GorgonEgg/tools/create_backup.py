
"""
Configuration backup script.

Specified files are placed into a backup .zip in a given
backup directory.
"""

from zipfile import ZipFile
from argparse import ArgumentParser
from pathlib import Path
from glob import glob
from datetime import datetime


def create_backup(out_folder: Path, files: list[Path]) -> None:
  """Create a .zip of some files and place it into a folder."""
  # First, get all of the subfiles within our files.
  zipables = []
  for file in files:

    if not file.exists():
      raise ValueError(f"Unable to find file '{file}'.")

    if file.is_file():
      zipables.append(file)

    elif file.is_dir():
      children = [
          file / Path(f) for f in glob("**", root_dir=file, recursive=True)
        ]
      zipables.extend(children)

    # If we encounter something that isn't a dir or file.
    else:
      raise ValueError(f"Unable to backup file '{file}'.")

  # Then, create our backup folder if it does not already exist.
  out_folder.mkdir(exist_ok=True)

  # Create our backup.

  backup_name = out_folder.joinpath(
      "Backup_" + datetime.now().strftime("%Y_%m_%d-%H_%M_%S") + ".zip"
    )

  with ZipFile(backup_name, "w") as out_zip:
    for pending in set(zipables):

      if pending.is_dir():
        out_zip.mkdir(str(pending))

      else:
        out_zip.write(str(pending))


def main():
  """Parse commandline arguments and create a backup."""
  parser = ArgumentParser()
  parser.add_argument(
      "backup_folder",
      type=Path,
      help="A folder to place the backup into."
    )
  parser.add_argument(
      "backup_files",
      metavar="file",
      nargs="+",
      type=Path,
      help="A file to back up."
    )

  args = parser.parse_args()

  create_backup(args.backup_folder, args.backup_files)


if __name__ == '__main__':
  main()
