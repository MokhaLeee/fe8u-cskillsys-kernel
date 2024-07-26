.section .rodata
.global SpritAnim_NinianPrayfx
SpritAnim_NinianPrayfx:

motion:
	.2byte (frame_list - motion), (anim_list - motion) @ header

frame_list: @ +$4
	.2byte (frame_0 - frame_list)
	.2byte (frame_1 - frame_list)
	.2byte (frame_2 - frame_list)
	.2byte (frame_3 - frame_list)
	.2byte (frame_4 - frame_list)
	.2byte (frame_5 - frame_list)
	.2byte (frame_6 - frame_list)
	.2byte (frame_7 - frame_list)
	.2byte (frame_8 - frame_list)
	.2byte (frame_9 - frame_list)
	.2byte (frame_10 - frame_list)
	.2byte (frame_11 - frame_list)
	.2byte (frame_12 - frame_list)
	.2byte (frame_13 - frame_list)
	.2byte (frame_14 - frame_list)
	.2byte (frame_15 - frame_list)
	.2byte (frame_16 - frame_list)
	.2byte (frame_17 - frame_list)
	.2byte (frame_18 - frame_list)
	.2byte (frame_19 - frame_list)
	.2byte (frame_20 - frame_list)

anim_list: @ +$2E
	.2byte (anim_0 - anim_list)
	.2byte (anim_1 - anim_list)

frame_0: @ +$32
	.2byte 1 @ oam entries
	.2byte 0xF8, 0x41F8, 0xA @ OAM Data #0

frame_1: @ +$3A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0xE6, 0xE6
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_2: @ +$4A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0xCC, 0xCC
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_3: @ +$5A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0xB3, 0xB3
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_4: @ +$6A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x99, 0x99
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_5: @ +$7A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x80, 0x80
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_6: @ +$8A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x66, 0x66
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_7: @ +$9A
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x4C, 0x4C
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_8: @ +$AA
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x33, 0x33
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0xA @ OAM Data #0

frame_9: @ +$BA
	.2byte 1 @ oam entries
	.2byte 0xF9, 0x0, 0x1F @ OAM Data #0

frame_10: @ +$C2
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x33, 0x33
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0x0 @ OAM Data #0

frame_11: @ +$D2
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x66, 0x66
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0x0 @ OAM Data #0

frame_12: @ +$E2
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0x99, 0x99
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0x0 @ OAM Data #0

frame_13: @ +$F2
	.2byte (1 | 0x8000) @ rotscale entries
	.2byte 0x100, 0xCC, 0xCC
	.2byte 1 @ oam entries
	.2byte 0x1F8, 0x41F8, 0x0 @ OAM Data #0

frame_14: @ +$102
	.2byte 1 @ oam entries
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #0

frame_15: @ +$10A
	.2byte 4 @ oam entries
	.2byte 0xFA, 0x1FD, 0x9 @ OAM Data #0
	.2byte 0x0, 0x1FB, 0x9 @ OAM Data #1
	.2byte 0xFA, 0x1F9, 0x9 @ OAM Data #2
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #3

frame_16: @ +$124
	.2byte 5 @ oam entries
	.2byte 0xF7, 0x1FE, 0x9 @ OAM Data #0
	.2byte 0x3, 0x1FA, 0x9 @ OAM Data #1
	.2byte 0xFB, 0x1F6, 0x9 @ OAM Data #2
	.2byte 0xFE, 0x1FF, 0x28 @ OAM Data #3
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #4

frame_17: @ +$144
	.2byte 7 @ oam entries
	.2byte 0xF5, 0x1FF, 0x9 @ OAM Data #0
	.2byte 0x6, 0x1F9, 0x9 @ OAM Data #1
	.2byte 0xFA, 0x1F4, 0x9 @ OAM Data #2
	.2byte 0xFF, 0x2, 0x28 @ OAM Data #3
	.2byte 0xFB, 0x1FA, 0x29 @ OAM Data #4
	.2byte 0x0, 0x1FD, 0x29 @ OAM Data #5
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #6

frame_18: @ +$170
	.2byte 7 @ oam entries
	.2byte 0xF2, 0x0, 0x9 @ OAM Data #0
	.2byte 0x9, 0x1F8, 0x9 @ OAM Data #1
	.2byte 0xF9, 0x1F3, 0x28 @ OAM Data #2
	.2byte 0x0, 0x5, 0x28 @ OAM Data #3
	.2byte 0xF9, 0x1FA, 0x29 @ OAM Data #4
	.2byte 0x2, 0x1FE, 0x29 @ OAM Data #5
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #6

frame_19: @ +$19C
	.2byte 8 @ oam entries
	.2byte 0xEF, 0x1, 0x29 @ OAM Data #0
	.2byte 0xC, 0x1F7, 0x29 @ OAM Data #1
	.2byte 0xF8, 0x1F1, 0x28 @ OAM Data #2
	.2byte 0x1, 0x7, 0x28 @ OAM Data #3
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #4
	.2byte 0xF6, 0x1F9, 0x29 @ OAM Data #5
	.2byte 0x4, 0x1FF, 0x29 @ OAM Data #6
	.2byte 0xFB, 0x1, 0x29 @ OAM Data #7

frame_20: @ +$1CE
	.2byte 5 @ oam entries
	.2byte 0xF8, 0x41F8, 0x0 @ OAM Data #0
	.2byte 0xF7, 0x1EE, 0x29 @ OAM Data #1
	.2byte 0x3, 0x9, 0x29 @ OAM Data #2
	.2byte 0xF9, 0x4, 0x29 @ OAM Data #3
	.2byte 0x9, 0x1FF, 0x29 @ OAM Data #4

anim_0: @ +$1EE
	.2byte  1,  0
	.2byte  1, 10
	.2byte  1,  9
	.2byte  1,  1
	.2byte  1, 11
	.2byte  1,  9
	.2byte  1,  2
	.2byte  1, 12
	.2byte  1,  9
	.2byte  1,  3
	.2byte  1, 13
	.2byte  1,  9
	.2byte  1,  4
	.2byte  1, 14
	.2byte  1,  9
	.2byte  1,  5
	.2byte  1, 15
	.2byte  1,  9
	.2byte  1,  6
	.2byte  1, 16
	.2byte  1,  9
	.2byte  1,  7
	.2byte  1, 17
	.2byte  1,  9
	.2byte  1,  8
	.2byte  1, 18
	.2byte  2,  9
	.2byte  1, 19
	.2byte  2,  9
	.2byte  1, 20
	.2byte  2,  9
	.2byte  1, 14

	.2byte 0, 1 @ kill animated object

anim_1: @ +$276
	.2byte  4,  0

	.2byte 0, (-1) @ loop back to start
