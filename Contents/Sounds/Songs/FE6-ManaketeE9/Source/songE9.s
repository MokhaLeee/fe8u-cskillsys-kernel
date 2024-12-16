	.include "MPlayDef.s"

	.equ	songE9_grp, ThisInstrument
	.equ	songE9_pri, 10
	.equ	songE9_rev, 128
	.equ	songE9_mvl, 127
	.equ	songE9_key, 0
	.equ	songE9_tbs, 1
	.equ	songE9_exg, 0
	.equ	songE9_cmp, 1

	.section .rodata
	.global	songE9
	.align	2


@**************** Track 1 (Midi-Chn.0) ****************@

songE9_001:
@  #01 @000   ----------------------------------------
 .byte   KEYSH , songE9_key+0
 .byte   TEMPO , 150*songE9_tbs/2
 .byte   VOICE , 1
 .byte   VOL , 127*songE9_mvl/mxv
 .byte   N72 ,Cn3 ,v127
 .byte   W24
 .byte   W24
 .byte   W24
 .byte   W24
@  #01 @001   ----------------------------------------
 .byte   W24
 .byte   FINE

@**************** Track 2 (Midi-Chn.1) ****************@

songE9_002:
@  #02 @000   ----------------------------------------
 .byte   KEYSH , songE9_key+0
 .byte   VOICE , 2
 .byte   BENDR, 11
 .byte   VOL , 36*songE9_mvl/mxv
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
@  #02 @001   ----------------------------------------
 .byte   BEND , c_v-32
 .byte   W02
 .byte   BEND , c_v-35
 .byte   W02
 .byte   BEND , c_v-38
 .byte   W02
 .byte   BEND , c_v-40
 .byte   W02
 .byte   VOL , 32*songE9_mvl/mxv
 .byte   BEND , c_v-43
 .byte   W02
 .byte   BEND , c_v-46
 .byte   W02
 .byte   VOL , 24*songE9_mvl/mxv
 .byte   BEND , c_v-48
 .byte   W02
 .byte   BEND , c_v-51
 .byte   W02
 .byte   VOL , 16*songE9_mvl/mxv
 .byte   BEND , c_v-54
 .byte   W02
 .byte   BEND , c_v-56
 .byte   W02
 .byte   VOL , 8*songE9_mvl/mxv
 .byte   BEND , c_v-59
 .byte   W02
 .byte   BEND , c_v-62
 .byte   W02
 .byte   EOT
 .byte   FINE

@******************************************************@
	.align	2

songE9:
	.byte	2	@ NumTrks
	.byte	0	@ NumBlks
	.byte	songE9_pri	@ Priority
	.byte	songE9_rev	@ Reverb.
    
	.word	songE9_grp
    
	.word	songE9_001
	.word	songE9_002

	.end
