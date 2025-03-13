#!/usr/bin/python3

import sys, re

def main(args):
    for line in sys.stdin.readlines():
        line = line.strip()

        # Fasten debug
        if line.find("GFX_CombatArtIcon") != -1:
             continue
        elif line.find("GFX_SkillIcon") != -1:
             continue
        elif line.find("CompressedText") != -1:
            continue
        elif line.find(" unit_icon_move_") != -1:
            continue
        elif line.find(" unit_icon_wait_") != -1:
            continue
        elif line.find("DirectSoundData_") != -1:
            continue
        elif line.find("_agbfe3_bgm_") != -1:
            continue
        elif line.find(" gUnknown_08") != -1:
            continue
        elif line.find(" AnimSprite_") != -1:
            continue
        elif line.find(" AnimConf_") != -1:
            continue
        elif line.find(" UnitDef_") != -1:
            continue
        elif line.find(" EventListScr_") != -1:
            continue
        elif line.find(" EventScr_") != -1:
            continue
        elif line.find(" TrapData_") != -1:
            continue
        elif line.find(" REDA_") != -1:
            continue
        elif line.find(" _08") != -1:
            continue
        elif line.find(" gImg_") != -1:
            continue
        elif line.find(" gPal_") != -1:
            continue
        elif line.find(" gTsa_") != -1:
            continue
        elif line.find(" Img_") != -1:
            continue
        elif line.find(" Pal_") != -1:
            continue
        elif line.find(" Tsa_") != -1:
            continue
        elif line.find(" img_") != -1:
            continue
        elif line.find(" pal_") != -1:
            continue
        elif line.find(" tsa_") != -1:
            continue
        elif line.find(" banim_") != -1:
            continue
        elif line.find(" song") != -1:
            continue
        elif line.find(" voice") != -1:
            continue
        elif line.find(" item_icon") != -1:
            continue
        elif line.find(" bg_") != -1:
            continue
        elif line.find(" portrait_") != -1:
            continue

        # Rmove thumb bit, so easily on debugging in NO$GBA
        line = re.sub('1 ', '0 ', line)
        line = re.sub('3 ', '2 ', line)
        line = re.sub('5 ', '4 ', line)
        line = re.sub('7 ', '6 ', line)
        line = re.sub('9 ', '8 ', line)
        line = re.sub('B ', 'A ', line)
        line = re.sub('D ', 'C ', line)
        line = re.sub('F ', 'E ', line)

        print(f"{line}")

if __name__ == '__main__':
    main(sys.argv)
