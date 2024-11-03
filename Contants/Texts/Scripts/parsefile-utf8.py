# TPC CHANGELOG:
# W.I.P.

# 1.0:
# - Initial Release

import os, sys

INPUT_ENCODING = "utf-8"
OUTPUT_ENCODING = "utf-8"

def Parse(line):

	res = line.strip('\n')
	res = bytes(res,OUTPUT_ENCODING)
	res = res.replace(b'[X]',	b'\x00')
	res = res.replace(b'[N]',	b'\x01')
	res = res.replace(b'[NL]',	b'\x01')
	res = res.replace(b'[2NL]',	b'\x02')
	res = res.replace(b'[A]',	b'\x03')
	res = res.replace(b'[....]',	b'\x04')
	res = res.replace(b'[.....]',	b'\x05')
	res = res.replace(b'[......]',	b'\x06')
	res = res.replace(b'[.......]',	b'\x07')
	res = res.replace(b'[FarLeft]',	b'\x08')
	res = res.replace(b'[OpenFarLeft]',	b'\x08')
	res = res.replace(b'[MidLeft]',	b'\x09')
	res = res.replace(b'[OpenMidLeft]',	b'\x09')
	res = res.replace(b'[OpenLeft]',	b'\x0A')
	res = res.replace(b'[Right]',	b'\x0B')
	res = res.replace(b'[OpenRight]',	b'\x0B')
	res = res.replace(b'[OpenMidRight]',	b'\x0C')
	res = res.replace(b'[OpenFarRight]',	b'\x0D')
	res = res.replace(b'[OpenFarFarLeft]',	b'\x0E')
	res = res.replace(b'[OpenFarFarRight]',	b'\x0F')

	res = res.replace(b'[LoadFace]', b'\x10')
	res = res.replace(b'[ClearFace]', b'\x11')
	res = res.replace(b'[NormalPrint]', b'\x12')
	res = res.replace(b'[FastPrint]', b'\x13')
	res = res.replace(b'[CloseSpeechFast]', b'\x14')
	res = res.replace(b'[CloseSpeechSlow]', b'\x15')
	res = res.replace(b'[ToggleMouthMove]', b'\x16')
	res = res.replace(b'[ToggleSmile]', b'\x17')
	res = res.replace(b'[Yes]', b'\x18')
	res = res.replace(b'[No]', b'\x19')
	res = res.replace(b'[Buy/Sell]', b'\x1A')
	res = res.replace(b'[ShopContinue]', b'\x1B')
	res = res.replace(b'[SendToBack]', b'\x1C')
	res = res.replace(b'[FastPrint]', b'\x1D')
	res = res.replace(b'[.]', b'\x1F')

	res = res.replace(b'[LoadOverworldFaces]', b'\x80\x04')
	res = res.replace(b'[G]', b'\x80\x05')
	res = res.replace(b'[MoveFarLeft]', b'\x80\x0A')
	res = res.replace(b'[MoveMidLeft]', b'\x80\x0B')
	res = res.replace(b'[MoveLeft]', b'\x80\x0C')
	res = res.replace(b'[MoveRight]', b'\x80\x0D')
	res = res.replace(b'[MoveMidRight]', b'\x80\x0E')
	res = res.replace(b'[MoveFarRight]', b'\x80\x0F')

	res = res.replace(b'[MoveFarFarLeft]', b'\x80\x10')
	res = res.replace(b'[MoveFarFarRight]', b'\x80\x11')
	res = res.replace(b'[EnableBlinking]', b'\x80\x16')
	res = res.replace(b'[DelayBlinking]', b'\x80\x18')
	res = res.replace(b'[PauseBlinking]', b'\x80\x19')
	res = res.replace(b'[DisableBlinking]', b'\x80\x1B')
	res = res.replace(b'[OpenEyes]', b'\x80\x1C')
	res = res.replace(b'[CloseEyes]', b'\x80\x1D')
	res = res.replace(b'[HalfCloseEyes]', b'\x80\x1E')
	res = res.replace(b'[Wink]', b'\x80\x1F')

	res = res.replace(b'[Tact]', b'\x80\x20')
	res = res.replace(b'[ToggleRed]', b'\x80\x21')
	res = res.replace(b'[Item]', b'\x80\x22')
	res = res.replace(b'[SetName]', b'\x80\x23')
	res = res.replace(b'[ToggleColorInvert]', b'\x80\x25')

	res = res.replace(b'[Font_Regular]', b'\x80\x26\x01')
	res = res.replace(b'[Font_Bold]', b'\x80\x26\x02')
	res = res.replace(b'[Font_Italic]', b'\x80\x26\x03')
	res = res.replace(b'[Font_NarrowTalk]', b'\x80\x26\x04')
	res = res.replace(b'[Font_NarrowMenu]', b'\x80\x26\x05')

	# The colors inbetween these ones are a mess, so I'm not using them
	# Presumably there's 255, someone else can test the rest
	res = res.replace(b'[BoxBackground]', b'\x80\x29')
	res = res.replace(b'[BoxBackground_Black]', b'\x80\x29\x02')
	res = res.replace(b'[BoxBackground_Blue]', b'\x80\x29\x03')
	res = res.replace(b'[BoxBackground_Red]', b'\x80\x29\x04')
	res = res.replace(b'[BoxBackground_DarkGreen]', b'\x80\x29\x09')
	res = res.replace(b'[BoxBackground_Brown]', b'\x80\x29\x0B')
	res = res.replace(b'[BoxBackground_DarkBlue]', b'\x80\x29\x0C')

	res = res.replace(b'[BoxType_Dramatic]', b'\x80\x2A\x02')
	res = res.replace(b'[BoxType_InnerMonologue]', b'\x80\x2A\x03')

	res = res.replace(b'[BoxHeight_1]', b'\x80\x2B\x01')
	res = res.replace(b'[BoxHeight_2]', b'\x80\x2B\x02')
	res = res.replace(b'[BoxHeight_3]', b'\x80\x2B\x03')

	res = res.replace(b'[TextSpeed_Slow]', b'\x80\x38\x08')
	res = res.replace(b'[TextSpeed_Normal]', b'\x80\x38\x04')
	res = res.replace(b'[TextSpeed_Fast]', b'\x80\x38\x01')
	res = res.replace(b'[TextSpeed_Instant]', b'\x80\x38\x00')

	# These need to be used in conjunction with [LoadFaceFancy] to work
	res = res.replace(b'[FlipRight]', b'\x81\x01\x02\x01\x01\x0D')
	res = res.replace(b'[FlipLeft]', b'\x80\x01\x02\x01\x01\x0D')

	res = res.replace(b'[0xFF]', b'\xFF\xFF')

	res = res.replace(b'[LoadFaceFancy]', b'\x80\x2F')
	res = res.replace(b'[LoadFaceFancy_Eirika]', b'\x80\x2F\x02\x01')
	res = res.replace(b'[LoadFaceFancy_EirikaClosed]', b'\x80\x2F\x03\x01')
	res = res.replace(b'[LoadFaceFancy_Seth]', b'\x80\x2F\x04\x01')
	res = res.replace(b'[LoadFaceFancy_Gilliam]', b'\x80\x2F\x05\x01')
	res = res.replace(b'[LoadFaceFancy_Franz]', b'\x80\x2F\x06\x01')
	res = res.replace(b'[LoadFaceFancy_Moulder]', b'\x80\x2F\x07\x01')
	res = res.replace(b'[LoadFaceFancy_Vanessa]', b'\x80\x2F\x08\x01')
	res = res.replace(b'[LoadFaceFancy_Ross]', b'\x80\x2F\x09\x01')
	res = res.replace(b'[LoadFaceFancy_Neimi]', b'\x80\x2F\x0A\x01')
	res = res.replace(b'[LoadFaceFancy_NeimiClosed]', b'\x80\x2F\x0B\x01')
	res = res.replace(b'[LoadFaceFancy_Colm]', b'\x80\x2F\x0C\x01')
	res = res.replace(b'[LoadFaceFancy_ColmClosed]', b'\x80\x2F\x0D\x01')
	res = res.replace(b'[LoadFaceFancy_Garcia]', b'\x80\x2F\x0E\x01')
	res = res.replace(b'[LoadFaceFancy_Innes]', b'\x80\x2F\x0F\x01')
	res = res.replace(b'[LoadFaceFancy_Lute]', b'\x80\x2F\x10\x01')
	res = res.replace(b'[LoadFaceFancy_Natasha]', b'\x80\x2F\x11\x01')
	res = res.replace(b'[LoadFaceFancy_NatashaClosed]', b'\x80\x2F\x12\x01')
	res = res.replace(b'[LoadFaceFancy_Cormag]', b'\x80\x2F\x13\x01')
	res = res.replace(b'[LoadFaceFancy_Ephraim]', b'\x80\x2F\x14\x01')
	res = res.replace(b'[LoadFaceFancy_EphraimClosed]', b'\x80\x2F\x15\x01')
	res = res.replace(b'[LoadFaceFancy_Forde]', b'\x80\x2F\x16\x01')
	res = res.replace(b'[LoadFaceFancy_FordeClosed]', b'\x80\x2F\x17\x01')
	res = res.replace(b'[LoadFaceFancy_Kyle]', b'\x80\x2F\x18\x01')
	res = res.replace(b'[LoadFaceFancy_Amelia]', b'\x80\x2F\x19\x01')
	res = res.replace(b'[LoadFaceFancy_Artur]', b'\x80\x2F\x1A\x01')
	res = res.replace(b'[LoadFaceFancy_Gerik]', b'\x80\x2F\x1B\x01')
	res = res.replace(b'[LoadFaceFancy_Tethys]', b'\x80\x2F\x1C\x01')
	res = res.replace(b'[LoadFaceFancy_TethysClosed]', b'\x80\x2F\x1D\x01')
	res = res.replace(b'[LoadFaceFancy_Marisa]', b'\x80\x2F\x1E\x01')
	res = res.replace(b'[LoadFaceFancy_MarisaClosed]', b'\x80\x2F\x1F\x01')
	res = res.replace(b'[LoadFaceFancy_Saleh]', b'\x80\x2F\x20\x01')
	res = res.replace(b'[LoadFaceFancy_Ewan]', b'\x80\x2F\x21\x01')
	res = res.replace(b'[LoadFaceFancy_Larachel]', b'\x80\x2F\x22\x01')
	res = res.replace(b'[LoadFaceFancy_Dozla]', b'\x80\x2F\x23\x01')
	res = res.replace(b'[LoadFaceFancy_Rennac]', b'\x80\x2F\x24\x01')
	res = res.replace(b'[LoadFaceFancy_Duessel]', b'\x80\x2F\x25\x01')
	res = res.replace(b'[LoadFaceFancy_Myrrh]', b'\x80\x2F\x26\x01')
	res = res.replace(b'[LoadFaceFancy_MyrrhClosed]', b'\x80\x2F\x27\x01')
	res = res.replace(b'[LoadFaceFancy_MyrrhWings]', b'\x80\x2F\x28\x01')
	res = res.replace(b'[LoadFaceFancy_Knoll]', b'\x80\x2F\x29\x01')
	res = res.replace(b'[LoadFaceFancy_Joshua]', b'\x80\x2F\x2A\x01')
	res = res.replace(b'[LoadFaceFancy_Syrene]', b'\x80\x2F\x2B\x01')
	res = res.replace(b'[LoadFaceFancy_Tana]', b'\x80\x2F\x2C\x01')
	res = res.replace(b'[LoadFaceFancy_EirikaPast]', b'\x80\x2F\x2D\x01')
	res = res.replace(b'[LoadFaceFancy_EphraimPast]', b'\x80\x2F\x2E\x01')
	res = res.replace(b'[LoadFaceFancy_KnollPast]', b'\x80\x2F\x2F\x01')
	res = res.replace(b'[LoadFaceFancy_ONeill]', b'\x80\x2F\x30\x01')
	res = res.replace(b'[LoadFaceFancy_Breguet]', b'\x80\x2F\x31\x01')
	res = res.replace(b'[LoadFaceFancy_Bone]', b'\x80\x2F\x32\x01')
	res = res.replace(b'[LoadFaceFancy_Bazba]', b'\x80\x2F\x33\x01')
	res = res.replace(b'[LoadFaceFancy_Saar]', b'\x80\x2F\x34\x01')
	res = res.replace(b'[LoadFaceFancy_Zonta]', b'\x80\x2F\x35\x01')
	res = res.replace(b'[LoadFaceFancy_Novala]', b'\x80\x2F\x36\x01')
	res = res.replace(b'[LoadFaceFancy_Murray]', b'\x80\x2F\x37\x01')
	res = res.replace(b'[LoadFaceFancy_Tirado]', b'\x80\x2F\x38\x01')
	res = res.replace(b'[LoadFaceFancy_Binks]', b'\x80\x2F\x39\x01')
	res = res.replace(b'[LoadFaceFancy_Pablo]', b'\x80\x2F\x3A\x01')
	res = res.replace(b'[LoadFaceFancy_Aias]', b'\x80\x2F\x3B\x01')
	res = res.replace(b'[LoadFaceFancy_Carlyle]', b'\x80\x2F\x3C\x01')
	res = res.replace(b'[LoadFaceFancy_Gheb]', b'\x80\x2F\x3D\x01')
	res = res.replace(b'[LoadFaceFancy_Beran]', b'\x80\x2F\x3E\x01')
	res = res.replace(b'[LoadFaceFancy_Selena]', b'\x80\x2F\x3F\x01')
	res = res.replace(b'[LoadFaceFancy_VigardeUndead]', b'\x80\x2F\x40\x01')
	res = res.replace(b'[LoadFaceFancy_VigardeHealthy]', b'\x80\x2F\x41\x01')
	res = res.replace(b'[LoadFaceFancy_Valter]', b'\x80\x2F\x42\x01')
	res = res.replace(b'[LoadFaceFancy_Caellach]', b'\x80\x2F\x43\x01')
	res = res.replace(b'[LoadFaceFancy_OrsonUnwell]', b'\x80\x2F\x44\x01')
	res = res.replace(b'[LoadFaceFancy_OrsonHealthy]', b'\x80\x2F\x45\x01')
	res = res.replace(b'[LoadFaceFancy_Lyon]', b'\x80\x2F\x46\x01')
	res = res.replace(b'[LoadFaceFancy_LyonClosed]', b'\x80\x2F\x47\x01')
	res = res.replace(b'[LoadFaceFancy_Riev]', b'\x80\x2F\x48\x01')
	res = res.replace(b'[LoadFaceFancy_Morva]', b'\x80\x2F\x49\x01')
	res = res.replace(b'[LoadFaceFancy_LyonDemonKing]', b'\x80\x2F\x4A\x01')
	res = res.replace(b'[LoadFaceFancy_Glen]', b'\x80\x2F\x4B\x01')
	res = res.replace(b'[LoadFaceFancy_BanditPirate]', b'\x80\x2F\x4C\x01')
	res = res.replace(b'[LoadFaceFancy_BanditHeadband]', b'\x80\x2F\x4D\x01')
	res = res.replace(b'[LoadFaceFancy_VigardePast]', b'\x80\x2F\x4E\x01')
	res = res.replace(b'[LoadFaceFancy_VigardeClosedPast]', b'\x80\x2F\x4F\x01')
	res = res.replace(b'[LoadFaceFancy_LyonPast]', b'\x80\x2F\x50\x01')
	res = res.replace(b'[LoadFaceFancy_Fado]', b'\x80\x2F\x51\x01')
	res = res.replace(b'[LoadFaceFancy_Hayden]', b'\x80\x2F\x52\x01')
	res = res.replace(b'[LoadFaceFancy_Mansel]', b'\x80\x2F\x53\x01')
	res = res.replace(b'[LoadFaceFancy_Klimt]', b'\x80\x2F\x54\x01')
	res = res.replace(b'[LoadFaceFancy_Dara]', b'\x80\x2F\x55\x01')
	res = res.replace(b'[LoadFaceFancy_Ismaire]', b'\x80\x2F\x56\x01')
	res = res.replace(b'[LoadFaceFancy_IsmaireClosed]', b'\x80\x2F\x57\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianMan1]', b'\x80\x2F\x58\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianManOld]', b'\x80\x2F\x59\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianMan2]', b'\x80\x2F\x5A\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianMan3]', b'\x80\x2F\x5B\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianMan4]', b'\x80\x2F\x5C\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianBoyTeen]', b'\x80\x2F\x5D\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianBoy]', b'\x80\x2F\x5E\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianWomanOld]', b'\x80\x2F\x5F\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianWoman1]', b'\x80\x2F\x60\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianWoman2]', b'\x80\x2F\x61\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianWoman3]', b'\x80\x2F\x62\x01')
	res = res.replace(b'[LoadFaceFancy_CivilianWomanGirl]', b'\x80\x2F\x63\x01')
	res = res.replace(b'[LoadFaceFancy_Anna]', b'\x80\x2F\x64\x01')
	res = res.replace(b'[LoadFaceFancy_Armory]', b'\x80\x2F\x65\x01')
	res = res.replace(b'[LoadFaceFancy_Shop]', b'\x80\x2F\x66\x01')
	res = res.replace(b'[LoadFaceFancy_Arena]', b'\x80\x2F\x67\x01')
	res = res.replace(b'[LoadFaceFancy_LinkArena]', b'\x80\x2F\x68\x01')
	res = res.replace(b'[LoadFaceFancy_Messenger]', b'\x80\x2F\x69\x01')
	res = res.replace(b'[LoadFaceFancy_SoldierBlue]', b'\x80\x2F\x6A\x01')
	res = res.replace(b'[LoadFaceFancy_SoldierBrown]', b'\x80\x2F\x6B\x01')
	res = res.replace(b'[LoadFaceFancy_FighterSilver]', b'\x80\x2F\x6C\x01')
	res = res.replace(b'[LoadFaceFancy_FighterBrown]', b'\x80\x2F\x6D\x01')
	res = res.replace(b'[LoadFaceFancy_NovalaClone]', b'\x80\x2F\x6E\x01')
	res = res.replace(b'[LoadFaceFancy_SoldierGreen]', b'\x80\x2F\x6F\x01')
	res = res.replace(b'[LoadFaceFancy_SoldierPurple]', b'\x80\x2F\x70\x01')
	res = res.replace(b'[LoadFaceFancy_SoldierSilver]', b'\x80\x2F\x71\x01')
	res = res.replace(b'[LoadFaceFancy_ActiveUnit]', b'\x80\x2F\xFF\xFF')

	res = res.replace(b'[LoadFace_Eirika]', b'\x10\x02\x01')
	res = res.replace(b'[LoadFace_EirikaClosed]', b'\x10\x03\x01')
	res = res.replace(b'[LoadFace_Seth]', b'\x10\x04\x01')
	res = res.replace(b'[LoadFace_Gilliam]', b'\x10\x05\x01')
	res = res.replace(b'[LoadFace_Franz]', b'\x10\x06\x01')
	res = res.replace(b'[LoadFace_Moulder]', b'\x10\x07\x01')
	res = res.replace(b'[LoadFace_Vanessa]', b'\x10\x08\x01')
	res = res.replace(b'[LoadFace_Ross]', b'\x10\x09\x01')
	res = res.replace(b'[LoadFace_Neimi]', b'\x10\x0A\x01')
	res = res.replace(b'[LoadFace_NeimiClosed]', b'\x10\x0B\x01')
	res = res.replace(b'[LoadFace_Colm]', b'\x10\x0C\x01')
	res = res.replace(b'[LoadFace_ColmClosed]', b'\x10\x0D\x01')
	res = res.replace(b'[LoadFace_Garcia]', b'\x10\x0E\x01')
	res = res.replace(b'[LoadFace_Innes]', b'\x10\x0F\x01')
	res = res.replace(b'[LoadFace_Lute]', b'\x10\x10\x01')
	res = res.replace(b'[LoadFace_Natasha]', b'\x10\x11\x01')
	res = res.replace(b'[LoadFace_NatashaClosed]', b'\x10\x12\x01')
	res = res.replace(b'[LoadFace_Cormag]', b'\x10\x13\x01')
	res = res.replace(b'[LoadFace_Ephraim]', b'\x10\x14\x01')
	res = res.replace(b'[LoadFace_EphraimClosed]', b'\x10\x15\x01')
	res = res.replace(b'[LoadFace_Forde]', b'\x10\x16\x01')
	res = res.replace(b'[LoadFace_FordeClosed]', b'\x10\x17\x01')
	res = res.replace(b'[LoadFace_Kyle]', b'\x10\x18\x01')
	res = res.replace(b'[LoadFace_Amelia]', b'\x10\x19\x01')
	res = res.replace(b'[LoadFace_Artur]', b'\x10\x1A\x01')
	res = res.replace(b'[LoadFace_Gerik]', b'\x10\x1B\x01')
	res = res.replace(b'[LoadFace_Tethys]', b'\x10\x1C\x01')
	res = res.replace(b'[LoadFace_TethysClosed]', b'\x10\x1D\x01')
	res = res.replace(b'[LoadFace_Marisa]', b'\x10\x1E\x01')
	res = res.replace(b'[LoadFace_MarisaClosed]', b'\x10\x1F\x01')
	res = res.replace(b'[LoadFace_Saleh]', b'\x10\x20\x01')
	res = res.replace(b'[LoadFace_Ewan]', b'\x10\x21\x01')
	res = res.replace(b'[LoadFace_Larachel]', b'\x10\x22\x01')
	res = res.replace(b'[LoadFace_Dozla]', b'\x10\x23\x01')
	res = res.replace(b'[LoadFace_Rennac]', b'\x10\x24\x01')
	res = res.replace(b'[LoadFace_Duessel]', b'\x10\x25\x01')
	res = res.replace(b'[LoadFace_Myrrh]', b'\x10\x26\x01')
	res = res.replace(b'[LoadFace_MyrrhClosed]', b'\x10\x27\x01')
	res = res.replace(b'[LoadFace_MyrrhWings]', b'\x10\x28\x01')
	res = res.replace(b'[LoadFace_Knoll]', b'\x10\x29\x01')
	res = res.replace(b'[LoadFace_Joshua]', b'\x10\x2A\x01')
	res = res.replace(b'[LoadFace_Syrene]', b'\x10\x2B\x01')
	res = res.replace(b'[LoadFace_Tana]', b'\x10\x2C\x01')
	res = res.replace(b'[LoadFace_EirikaPast]', b'\x10\x2D\x01')
	res = res.replace(b'[LoadFace_EphraimPast]', b'\x10\x2E\x01')
	res = res.replace(b'[LoadFace_KnollPast]', b'\x10\x2F\x01')
	res = res.replace(b'[LoadFace_ONeill]', b'\x10\x30\x01')
	res = res.replace(b'[LoadFace_Breguet]', b'\x10\x31\x01')
	res = res.replace(b'[LoadFace_Bone]', b'\x10\x32\x01')
	res = res.replace(b'[LoadFace_Bazba]', b'\x10\x33\x01')
	res = res.replace(b'[LoadFace_Saar]', b'\x10\x34\x01')
	res = res.replace(b'[LoadFace_Zonta]', b'\x10\x35\x01')
	res = res.replace(b'[LoadFace_Novala]', b'\x10\x36\x01')
	res = res.replace(b'[LoadFace_Murray]', b'\x10\x37\x01')
	res = res.replace(b'[LoadFace_Tirado]', b'\x10\x38\x01')
	res = res.replace(b'[LoadFace_Binks]', b'\x10\x39\x01')
	res = res.replace(b'[LoadFace_Pablo]', b'\x10\x3A\x01')
	res = res.replace(b'[LoadFace_Aias]', b'\x10\x3B\x01')
	res = res.replace(b'[LoadFace_Carlyle]', b'\x10\x3C\x01')
	res = res.replace(b'[LoadFace_Gheb]', b'\x10\x3D\x01')
	res = res.replace(b'[LoadFace_Beran]', b'\x10\x03E\x01')
	res = res.replace(b'[LoadFace_Selena]', b'\x10\x3F\x01')
	res = res.replace(b'[LoadFace_VigardeUndead]', b'\x10\x40\x01')
	res = res.replace(b'[LoadFace_VigardeHealthy]', b'\x10\x41\x01')
	res = res.replace(b'[LoadFace_Valter]', b'\x10\x42\x01')
	res = res.replace(b'[LoadFace_Caellach]', b'\x10\x43\x01')
	res = res.replace(b'[LoadFace_OrsonUnwell]', b'\x10\x44\x01')
	res = res.replace(b'[LoadFace_OrsonHealthy]', b'\x10\x45\x01')
	res = res.replace(b'[LoadFace_Lyon]', b'\x10\x46\x01')
	res = res.replace(b'[LoadFace_LyonClosed]', b'\x10\x47\x01')
	res = res.replace(b'[LoadFace_Riev]', b'\x10\x48\x01')
	res = res.replace(b'[LoadFace_Morva]', b'\x10\x49\x01')
	res = res.replace(b'[LoadFace_LyonDemonKing]', b'\x10\x4A\x01')
	res = res.replace(b'[LoadFace_Glen]', b'\x10\x4B\x01')
	res = res.replace(b'[LoadFace_BanditPirate]', b'\x10\x4C\x01')
	res = res.replace(b'[LoadFace_BanditHeadband]', b'\x10\x4D\x01')
	res = res.replace(b'[LoadFace_VigardePast]', b'\x10\x4E\x01')
	res = res.replace(b'[LoadFace_VigardeClosedPast]', b'\x10\x4F\x01')
	res = res.replace(b'[LoadFace_LyonPast]', b'\x10\x50\x01')
	res = res.replace(b'[LoadFace_Fado]', b'\x10\x51\x01')
	res = res.replace(b'[LoadFace_Hayden]', b'\x10\x52\x01')
	res = res.replace(b'[LoadFace_Mansel]', b'\x10\x53\x01')
	res = res.replace(b'[LoadFace_Klimt]', b'\x10\x54\x01')
	res = res.replace(b'[LoadFace_Dara]', b'\x10\x55\x01')
	res = res.replace(b'[LoadFace_Ismaire]', b'\x10\x56\x01')
	res = res.replace(b'[LoadFace_IsmaireClosed]', b'\x10\x57\x01')
	res = res.replace(b'[LoadFace_CivilianMan1]', b'\x10\x58\x01')
	res = res.replace(b'[LoadFace_CivilianManOld]', b'\x10\x59\x01')
	res = res.replace(b'[LoadFace_CivilianMan2]', b'\x10\x5A\x01')
	res = res.replace(b'[LoadFace_CivilianMan3]', b'\x10\x5B\x01')
	res = res.replace(b'[LoadFace_CivilianMan4]', b'\x10\x5C\x01')
	res = res.replace(b'[LoadFace_CivilianBoyTeen]', b'\x10\x5D\x01')
	res = res.replace(b'[LoadFace_CivilianBoy]', b'\x10\x5E\x01')
	res = res.replace(b'[LoadFace_CivilianWomanOld]', b'\x10\x5F\x01')
	res = res.replace(b'[LoadFace_CivilianWoman1]', b'\x10\x60\x01')
	res = res.replace(b'[LoadFace_CivilianWoman2]', b'\x10\x61\x01')
	res = res.replace(b'[LoadFace_CivilianWoman3]', b'\x10\x62\x01')
	res = res.replace(b'[LoadFace_CivilianWomanGirl]', b'\x10\x63\x01')
	res = res.replace(b'[LoadFace_Anna]', b'\x10\x64\x01')
	res = res.replace(b'[LoadFace_Armory]', b'\x10\x65\x01')
	res = res.replace(b'[LoadFace_Shop]', b'\x10\x66\x01')
	res = res.replace(b'[LoadFace_Arena]', b'\x10\x67\x01')
	res = res.replace(b'[LoadFace_LinkArena]', b'\x10\x68\x01')
	res = res.replace(b'[LoadFace_Messenger]', b'\x10\x69\x01')
	res = res.replace(b'[LoadFace_SoldierBlue]', b'\x10\x6A\x01')
	res = res.replace(b'[LoadFace_SoldierBrown]', b'\x10\x6B\x01')
	res = res.replace(b'[LoadFace_FighterSilver]', b'\x10\x6C\x01')
	res = res.replace(b'[LoadFace_FighterBrown]', b'\x10\x6D\x01')
	res = res.replace(b'[LoadFace_NovalaClone]', b'\x10\x6E\x01')
	res = res.replace(b'[LoadFace_SoldierGreen]', b'\x10\x6F\x01')
	res = res.replace(b'[LoadFace_SoldierPurple]', b'\x10\x70\x01')
	res = res.replace(b'[LoadFace_SoldierSilver]', b'\x10\x71\x01')
	res = res.replace(b'[LoadFace_ActiveUnit]', b'\x10\xFF\xFF')

	res = res.replace(b'[0x1]', b'\x01')
	res = res.replace(b'[0x01]', b'\x01')
	res = res.replace(b'[0x2]', b'\x02')
	res = res.replace(b'[0x02]', b'\x02')
	res = res.replace(b'[0x3]', b'\x03')
	res = res.replace(b'[0x03]', b'\x03')
	res = res.replace(b'[0x4]', b'\x04')
	res = res.replace(b'[0x04]', b'\x04')
	res = res.replace(b'[0x5]', b'\x05')
	res = res.replace(b'[0x05]', b'\x05')
	res = res.replace(b'[0x6]', b'\x06')
	res = res.replace(b'[0x06]', b'\x06')
	res = res.replace(b'[0x7]', b'\x07')
	res = res.replace(b'[0x07]', b'\x07')
	res = res.replace(b'[0x8]', b'\x08')
	res = res.replace(b'[0x08]', b'\x08')
	res = res.replace(b'[0x9]', b'\x09')
	res = res.replace(b'[0x09]', b'\x09')
	res = res.replace(b'[0xA]', b'\x0A')
	res = res.replace(b'[0x0A]', b'\x0A')
	res = res.replace(b'[0xB]', b'\x0B')
	res = res.replace(b'[0x0B]', b'\x0B')
	res = res.replace(b'[0xC]', b'\x0C')
	res = res.replace(b'[0x0C]', b'\x0C')
	res = res.replace(b'[0xD]', b'\x0D')
	res = res.replace(b'[0x0D]', b'\x0D')
	res = res.replace(b'[0xE]', b'\x0E')
	res = res.replace(b'[0x0E]', b'\x0E')
	res = res.replace(b'[0xF]', b'\x0F')
	res = res.replace(b'[0x0F]', b'\x0F')

	res = res.replace(b'[0x10]', b'\x10')
	res = res.replace(b'[0x11]', b'\x11')
	res = res.replace(b'[0x12]', b'\x12')
	res = res.replace(b'[0x13]', b'\x13')
	res = res.replace(b'[0x14]', b'\x14')
	res = res.replace(b'[0x15]', b'\x15')
	res = res.replace(b'[0x16]', b'\x16')
	res = res.replace(b'[0x17]', b'\x17')
	res = res.replace(b'[0x18]', b'\x18')
	res = res.replace(b'[0x19]', b'\x19')
	res = res.replace(b'[0x1A]', b'\x1A')
	res = res.replace(b'[0x1B]', b'\x1B')
	res = res.replace(b'[0x1C]', b'\x1C')
	res = res.replace(b'[0x1D]', b'\x1D')
	res = res.replace(b'[0x1E]', b'\x1E')
	res = res.replace(b'[0x1F]', b'\x1F')

	res = res.replace(b'[0x20]', b'\x20')
	res = res.replace(b'[0x21]', b'\x21')
	res = res.replace(b'[0x22]', b'\x22')
	res = res.replace(b'[0x23]', b'\x23')
	res = res.replace(b'[0x24]', b'\x24')
	res = res.replace(b'[0x25]', b'\x25')
	res = res.replace(b'[0x26]', b'\x26')
	res = res.replace(b'[0x27]', b'\x27')
	res = res.replace(b'[0x28]', b'\x28')
	res = res.replace(b'[0x29]', b'\x29')
	res = res.replace(b'[0x2A]', b'\x2A')
	res = res.replace(b'[0x2B]', b'\x2B')
	res = res.replace(b'[0x2C]', b'\x2C')
	res = res.replace(b'[0x2D]', b'\x2D')
	res = res.replace(b'[0x2E]', b'\x2E')
	res = res.replace(b'[0x2F]', b'\x2F')

	res = res.replace(b'[0x30]', b'\x30')
	res = res.replace(b'[0x31]', b'\x31')
	res = res.replace(b'[0x32]', b'\x32')
	res = res.replace(b'[0x33]', b'\x33')
	res = res.replace(b'[0x34]', b'\x34')
	res = res.replace(b'[0x35]', b'\x35')
	res = res.replace(b'[0x36]', b'\x36')
	res = res.replace(b'[0x37]', b'\x37')
	res = res.replace(b'[0x38]', b'\x38')
	res = res.replace(b'[0x39]', b'\x39')
	res = res.replace(b'[0x3A]', b'\x3A')
	res = res.replace(b'[0x3B]', b'\x3B')
	res = res.replace(b'[0x3C]', b'\x3C')
	res = res.replace(b'[0x3D]', b'\x3D')
	res = res.replace(b'[0x3E]', b'\x3E')
	res = res.replace(b'[0x3F]', b'\x3F')

	res = res.replace(b'[0x40]', b'\x40')
	res = res.replace(b'[0x41]', b'\x41')
	res = res.replace(b'[0x42]', b'\x42')
	res = res.replace(b'[0x43]', b'\x43')
	res = res.replace(b'[0x44]', b'\x44')
	res = res.replace(b'[0x45]', b'\x45')
	res = res.replace(b'[0x46]', b'\x46')
	res = res.replace(b'[0x47]', b'\x47')
	res = res.replace(b'[0x48]', b'\x48')
	res = res.replace(b'[0x49]', b'\x49')
	res = res.replace(b'[0x4A]', b'\x4A')
	res = res.replace(b'[0x4B]', b'\x4B')
	res = res.replace(b'[0x4C]', b'\x4C')
	res = res.replace(b'[0x4D]', b'\x4D')
	res = res.replace(b'[0x4E]', b'\x4E')
	res = res.replace(b'[0x4F]', b'\x4F')

	res = res.replace(b'[0x50]', b'\x50')
	res = res.replace(b'[0x51]', b'\x51')
	res = res.replace(b'[0x52]', b'\x52')
	res = res.replace(b'[0x53]', b'\x53')
	res = res.replace(b'[0x54]', b'\x54')
	res = res.replace(b'[0x55]', b'\x55')
	res = res.replace(b'[0x56]', b'\x56')
	res = res.replace(b'[0x57]', b'\x57')
	res = res.replace(b'[0x58]', b'\x58')
	res = res.replace(b'[0x59]', b'\x59')
	res = res.replace(b'[0x5A]', b'\x5A')
	res = res.replace(b'[0x5B]', b'\x5B')
	res = res.replace(b'[0x5C]', b'\x5C')
	res = res.replace(b'[0x5D]', b'\x5D')
	res = res.replace(b'[0x5E]', b'\x5E')
	res = res.replace(b'[0x5F]', b'\x5F')

	res = res.replace(b'[0x60]', b'\x60')
	res = res.replace(b'[0x61]', b'\x61')
	res = res.replace(b'[0x62]', b'\x62')
	res = res.replace(b'[0x63]', b'\x63')
	res = res.replace(b'[0x64]', b'\x64')
	res = res.replace(b'[0x65]', b'\x65')
	res = res.replace(b'[0x66]', b'\x66')
	res = res.replace(b'[0x67]', b'\x67')
	res = res.replace(b'[0x68]', b'\x68')
	res = res.replace(b'[0x69]', b'\x69')
	res = res.replace(b'[0x6A]', b'\x6A')
	res = res.replace(b'[0x6B]', b'\x6B')
	res = res.replace(b'[0x6C]', b'\x6C')
	res = res.replace(b'[0x6D]', b'\x6D')
	res = res.replace(b'[0x6E]', b'\x6E')
	res = res.replace(b'[0x6F]', b'\x6F')

	res = res.replace(b'[0x70]', b'\x70')
	res = res.replace(b'[0x71]', b'\x71')
	res = res.replace(b'[0x72]', b'\x72')
	res = res.replace(b'[0x73]', b'\x73')
	res = res.replace(b'[0x74]', b'\x74')
	res = res.replace(b'[0x75]', b'\x75')
	res = res.replace(b'[0x76]', b'\x76')
	res = res.replace(b'[0x77]', b'\x77')
	res = res.replace(b'[0x78]', b'\x78')
	res = res.replace(b'[0x79]', b'\x79')
	res = res.replace(b'[0x7A]', b'\x7A')
	res = res.replace(b'[0x7B]', b'\x7B')
	res = res.replace(b'[0x7C]', b'\x7C')
	res = res.replace(b'[0x7D]', b'\x7D')
	res = res.replace(b'[0x7E]', b'\x7E')
	res = res.replace(b'[0x7F]', b'\x7F')

	res = res.replace(b'[0x80]', b'\x80')
	res = res.replace(b'[0x81]', b'\x81')
	res = res.replace(b'[0x82]', b'\x82')
	res = res.replace(b'[0x83]', b'\x83')
	res = res.replace(b'[0x84]', b'\x84')
	res = res.replace(b'[0x85]', b'\x85')
	res = res.replace(b'[0x86]', b'\x86')
	res = res.replace(b'[0x87]', b'\x87')
	res = res.replace(b'[0x88]', b'\x88')
	res = res.replace(b'[0x89]', b'\x89')
	res = res.replace(b'[0x8A]', b'\x8A')
	res = res.replace(b'[0x8B]', b'\x8B')
	res = res.replace(b'[0x8C]', b'\x8C')
	res = res.replace(b'[0x8D]', b'\x8D')
	res = res.replace(b'[0x8E]', b'\x8E')
	res = res.replace(b'[0x8F]', b'\x8F')

	res = res.replace(b'[0x90]', b'\x90')
	res = res.replace(b'[0x91]', b'\x91')
	res = res.replace(b'[0x92]', b'\x92')
	res = res.replace(b'[0x93]', b'\x93')
	res = res.replace(b'[0x94]', b'\x94')
	res = res.replace(b'[0x95]', b'\x95')
	res = res.replace(b'[0x96]', b'\x96')
	res = res.replace(b'[0x97]', b'\x97')
	res = res.replace(b'[0x98]', b'\x98')
	res = res.replace(b'[0x99]', b'\x99')
	res = res.replace(b'[0x9A]', b'\x9A')
	res = res.replace(b'[0x9B]', b'\x9B')
	res = res.replace(b'[0x9C]', b'\x9C')
	res = res.replace(b'[0x9D]', b'\x9D')
	res = res.replace(b'[0x9E]', b'\x9E')
	res = res.replace(b'[0x9F]', b'\x9F')

	res = res.replace(b'[0xA0]', b'\xA0')
	res = res.replace(b'[0xA1]', b'\xA1')
	res = res.replace(b'[0xA2]', b'\xA2')
	res = res.replace(b'[0xA3]', b'\xA3')
	res = res.replace(b'[0xA4]', b'\xA4')
	res = res.replace(b'[0xA5]', b'\xA5')
	res = res.replace(b'[0xA6]', b'\xA6')
	res = res.replace(b'[0xA7]', b'\xA7')
	res = res.replace(b'[0xA8]', b'\xA8')
	res = res.replace(b'[0xA9]', b'\xA9')
	res = res.replace(b'[0xAA]', b'\xAA')
	res = res.replace(b'[0xAB]', b'\xAB')
	res = res.replace(b'[0xAC]', b'\xAC')
	res = res.replace(b'[0xAD]', b'\xAD')
	res = res.replace(b'[0xAE]', b'\xAE')
	res = res.replace(b'[0xAF]', b'\xAF')
	return res

def main():
	import argparse
	argParse = argparse.ArgumentParser()
	argParse.add_argument('-i','--input', help = 'input .fetext file generated by text-process')
	argParse.add_argument('-o','--output', help = 'output .dmp file to \"#incbin\" by Event-Assembler')

	arguments = argParse.parse_args(sys.argv[1:])

	input_file = arguments.input
	output_file = arguments.output

	if not os.path.exists(input_file):
		sys.exit("`{}` doesn't exist".format(input_file))

	with open(input_file, 'r', encoding = INPUT_ENCODING) as fi:
		with open(output_file, 'wb') as fo:

			for line in fi.readlines():
				fo.write( Parse(line) )

if __name__ == '__main__':
	main()
