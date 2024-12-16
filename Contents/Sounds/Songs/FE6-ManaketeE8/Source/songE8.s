	.include "MPlayDef.s"

	.equ	songE8_grp, ThisInstrument
	.equ	songE8_pri, 10
	.equ	songE8_rev, 128
	.equ	songE8_mvl, 127
	.equ	songE8_key, 0
	.equ	songE8_tbs, 1
	.equ	songE8_exg, 0
	.equ	songE8_cmp, 1

	.section .rodata
	.global	songE8
	.align	2


@**************** Track 1 (Midi-Chn.0) ****************@

songE8_001:
@  #01 @000   ----------------------------------------
 .byte   KEYSH , songE8_key+0
 .byte   TEMPO , 150*songE8_tbs/2
 .byte   VOICE , 1
 .byte   VOL , 127*songE8_mvl/mxv
 .byte   N84 ,Cn3 ,v127
 .byte   W24
 .byte   W24
 .byte   W24
 .byte   W24
@  #01 @001   ----------------------------------------
 .byte   W24
 .byte   W24
 .byte   W24
 .byte   W24
@  #01 @002   ----------------------------------------
 .byte   W24
 .byte   FINE

@**************** Track 2 (Midi-Chn.1) ****************@

songE8_002:
@  #02 @000   ----------------------------------------
 .byte   KEYSH , songE8_key+0
 .byte   VOICE , 2
 .byte   BENDR, 11
 .byte   VOL , 36*songE8_mvl/mxv
 .byte   BEND , c_v+0
 .byte   TIE ,Cn3 ,v127
 .byte   W01
 .byte   BEND , c_v-2
 .byte   W01
 .byte   BEND , c_v-4
 .byte   W01
 .byte   BEND , c_v-6
 .byte   W01
 .byte   BEND , c_v-8
 .byte   W01
 .byte   BEND , c_v-10
 .byte   W01
 .byte   BEND , c_v-12
 .byte   W01
 .byte   BEND , c_v-14
 .byte   W01
 .byte   BEND , c_v-16
 .byte   W01
 .byte   BEND , c_v-18
 .byte   W01
 .byte   BEND , c_v-20
 .byte   W01
 .byte   BEND , c_v-22
 .byte   W01
 .byte   BEND , c_v-24
 .byte   W01
 .byte   BEND , c_v-26
 .byte   W01
 .byte   BEND , c_v-28
 .byte   W01
 .byte   BEND , c_v-30
 .byte   W01
 .byte   BEND , c_v-32
 .byte   W01
 .byte   BEND , c_v-34
 .byte   W01
 .byte   BEND , c_v-36
 .byte   W01
 .byte   BEND , c_v-38
 .byte   W01
 .byte   BEND , c_v-40
 .byte   W01
 .byte   BEND , c_v-42
 .byte   W01
 .byte   BEND , c_v-44
 .byte   W01
 .byte   BEND , c_v-46
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-50
 .byte   W01
 .byte   BEND , c_v-51
 .byte   W01
 .byte   BEND , c_v-52
 .byte   W01
 .byte   BEND , c_v-53
 .byte   W01
 .byte   BEND , c_v-54
 .byte   W01
 .byte   BEND , c_v-55
 .byte   W01
 .byte   BEND , c_v-56
 .byte   W01
 .byte   BEND , c_v-57
 .byte   W01
 .byte   BEND , c_v-58
 .byte   W01
 .byte   BEND , c_v-59
 .byte   W01
 .byte   BEND , c_v-60
 .byte   W01
 .byte   BEND , c_v-60
 .byte   W01
 .byte   BEND , c_v-61
 .byte   W01
 .byte   BEND , c_v-62
 .byte   W01
 .byte   BEND , c_v-62
 .byte   W01
 .byte   BEND , c_v-63
 .byte   W01
 .byte   BEND , c_v-63
 .byte   W01
 .byte   BEND , c_v-63
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W24
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-64
 .byte   W01
 .byte   BEND , c_v-63
 .byte   W01
 .byte   BEND , c_v-62
 .byte   W01
 .byte   BEND , c_v-62
 .byte   W01
 .byte   BEND , c_v-61
 .byte   W01
 .byte   BEND , c_v-60
 .byte   W01
 .byte   BEND , c_v-59
 .byte   W01
 .byte   BEND , c_v-58
 .byte   W01
 .byte   BEND , c_v-56
 .byte   W01
 .byte   BEND , c_v-55
 .byte   W01
 .byte   BEND , c_v-54
 .byte   W01
 .byte   BEND , c_v-52
 .byte   W01
 .byte   BEND , c_v-50
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-46
 .byte   W01
 .byte   BEND , c_v-44
 .byte   W01
 .byte   BEND , c_v-42
 .byte   W01
 .byte   BEND , c_v-40
 .byte   W01
 .byte   BEND , c_v-38
 .byte   W01
 .byte   BEND , c_v-35
 .byte   W01
@  #02 @001   ----------------------------------------
 .byte   BEND , c_v-32
 .byte   W01
 .byte   BEND , c_v-30
 .byte   W01
 .byte   BEND , c_v-28
 .byte   W01
 .byte   BEND , c_v-26
 .byte   W01
 .byte   BEND , c_v-24
 .byte   W01
 .byte   BEND , c_v-22
 .byte   W01
 .byte   BEND , c_v-21
 .byte   W01
 .byte   BEND , c_v-19
 .byte   W01
 .byte   BEND , c_v-18
 .byte   W01
 .byte   BEND , c_v-18
 .byte   W01
 .byte   BEND , c_v-17
 .byte   W01
 .byte   BEND , c_v-17
 .byte   W01
 .byte   BEND , c_v-16
 .byte   W01
 .byte   BEND , c_v-17
 .byte   W01
 .byte   BEND , c_v-17
 .byte   W01
 .byte   BEND , c_v-18
 .byte   W01
 .byte   BEND , c_v-18
 .byte   W01
 .byte   BEND , c_v-19
 .byte   W01
 .byte   BEND , c_v-21
 .byte   W01
 .byte   BEND , c_v-22
 .byte   W01
 .byte   BEND , c_v-24
 .byte   W01
 .byte   BEND , c_v-26
 .byte   W01
 .byte   BEND , c_v-28
 .byte   W01
 .byte   BEND , c_v-30
 .byte   W01
 .byte   BEND , c_v-32
 .byte   W01
 .byte   BEND , c_v-34
 .byte   W01
 .byte   BEND , c_v-35
 .byte   W01
 .byte   BEND , c_v-36
 .byte   W01
 .byte   BEND , c_v-37
 .byte   W01
 .byte   BEND , c_v-38
 .byte   W01
 .byte   BEND , c_v-39
 .byte   W01
 .byte   BEND , c_v-40
 .byte   W01
 .byte   BEND , c_v-41
 .byte   W01
 .byte   BEND , c_v-42
 .byte   W01
 .byte   BEND , c_v-43
 .byte   W01
 .byte   BEND , c_v-44
 .byte   W01
 .byte   BEND , c_v-44
 .byte   W01
 .byte   BEND , c_v-45
 .byte   W01
 .byte   BEND , c_v-46
 .byte   W01
 .byte   BEND , c_v-46
 .byte   W01
 .byte   BEND , c_v-47
 .byte   W01
 .byte   BEND , c_v-47
 .byte   W01
 .byte   BEND , c_v-47
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-48
 .byte   W01
 .byte   VOL , 32*songE8_mvl/mxv
 .byte   BEND , c_v-48
 .byte   W01
 .byte   BEND , c_v-47
 .byte   W01
 .byte   BEND , c_v-46
 .byte   W01
 .byte   BEND , c_v-45
 .byte   W01
 .byte   BEND , c_v-44
 .byte   W01
 .byte   BEND , c_v-43
 .byte   W01
 .byte   BEND , c_v-42
 .byte   W01
 .byte   BEND , c_v-40
 .byte   W01
 .byte   VOL , 32*songE8_mvl/mxv
 .byte   W01
 .byte   BEND , c_v-41
 .byte   W02
 .byte   BEND , c_v-42
 .byte   W02
 .byte   BEND , c_v-43
 .byte   W02
 .byte   BEND , c_v-44
 .byte   W01
 .byte   VOL , 28*songE8_mvl/mxv
 .byte   W01
 .byte   BEND , c_v-45
 .byte   W02
 .byte   BEND , c_v-46
 .byte   W02
 .byte   BEND , c_v-47
 .byte   W02
 .byte   BEND , c_v-48
 .byte   W01
 .byte   VOL , 24*songE8_mvl/mxv
 .byte   W01
 .byte   BEND , c_v-49
 .byte   W02
 .byte   BEND , c_v-50
 .byte   W02
 .byte   BEND , c_v-51
 .byte   W02
 .byte   BEND , c_v-52
 .byte   W01
 .byte   VOL , 20*songE8_mvl/mxv
 .byte   W01
 .byte   BEND , c_v-53
 .byte   W02
 .byte   BEND , c_v-54
 .byte   W02
 .byte   BEND , c_v-55
 .byte   W02
 .byte   BEND , c_v-56
 .byte   W01
 .byte   VOL , 16*songE8_mvl/mxv
 .byte   W01
 .byte   BEND , c_v-57
 .byte   W02
 .byte   BEND , c_v-58
 .byte   W02
 .byte   BEND , c_v-59
 .byte   W02
 .byte   BEND , c_v-60
 .byte   W01
@  #02 @002   ----------------------------------------
 .byte   VOL , 12*songE8_mvl/mxv
 .byte   W01
 .byte   BEND , c_v-61
 .byte   W02
 .byte   BEND , c_v-62
 .byte   W02
 .byte   BEND , c_v-63
 .byte   W02
 .byte   BEND , c_v-64
 .byte   W01
 .byte   VOL , 8*songE8_mvl/mxv
 .byte   W08
 .byte   VOL , 4*songE8_mvl/mxv
 .byte   W08
 .byte   EOT
 .byte   FINE

@******************************************************@
	.align	2

songE8:
	.byte	2	@ NumTrks
	.byte	0	@ NumBlks
	.byte	songE8_pri	@ Priority
	.byte	songE8_rev	@ Reverb.
    
	.word	songE8_grp
    
	.word	songE8_001
	.word	songE8_002

	.end
