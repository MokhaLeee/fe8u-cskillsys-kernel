# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

"""
Converts a TMX file into an installable EA event, for use in buildfiles.
This is an alternative interface to the functionality of TMX2EA.
It trades the ability to batch process folders and files for the ability to customize the output paths.
"""

import os.path
import sys, argparse

# HACK: we need TMX2EA to see modules in its own directory, since it imports lzss locally
# If anybody knows a better way of doing this please tell me.
sys.path.append(os.path.join(os.path.split(__file__)[0], "TMX2EA"))
import TMX2EA.tmx2ea as core

def main():
    sys.excepthook = core.showExceptionAndExit

    parser = argparse.ArgumentParser(description='Convert a TMX file into an installable EA event. This only processes a single file and does not create an installer.')

    parser.add_argument("tmx_file", help="path to tmx file to process")
    parser.add_argument("--output-event", help='output event (default: TMX_FILE:.tmx=.event)')
    parser.add_argument("--output-data", help='output dmp (default: TMX_FILE:.tmx=_data.dmp)')
    parser.add_argument("--omit-header", action="store_true", help="do not add in the tmx2ea header in generated event file")

    args = parser.parse_args()

    tmx_path: str = args.tmx_file
    evt_path: str = args.output_event if args.output_event != None else f"{os.path.splitext(tmx_path)[0]}.event"
    dmp_path: str = args.output_data if args.output_data != None else f"{os.path.splitext(tmx_path)[0]}_data.dmp"
    emit_header: bool = not args.omit_header

    try:
        core.process(tmx_path, evt_path, dmp_path, emit_header)

    except core.Tmx2EaError as e:
        sys.exit(f"ERROR: {tmx_path}: {e.message}")

if __name__ == '__main__':
    main()
