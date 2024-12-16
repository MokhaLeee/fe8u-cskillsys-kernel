	.include "MPlayDef.s"

	.equ	song91_grp, ThisInstrument
	.equ	song91_pri, 0
	.equ	song91_rev, 0
	.equ	song91_mvl, 127
	.equ	song91_key, 0
	.equ	song91_tbs, 1
	.equ	song91_exg, 0
	.equ	song91_cmp, 1

	.section .rodata
	.global	song91
	.align	2


@**************** Track 1 (Midi-Chn.0) ****************@

song91_001:
@  #01 @000   ----------------------------------------
 .byte   VOL , 127*song91_mvl/mxv
 .byte   KEYSH , song91_key+0
 .byte   VOICE , 0
 .byte   TIE ,Cn3 ,v127
 .byte   W96
@  #01 @001   ----------------------------------------
 .byte   W96
@  #01 @002   ----------------------------------------
 .byte   W96
@  #01 @003   ----------------------------------------
 .byte   EOT
 .byte   FINE

@******************************************************@
	.align	2

song91:
	.byte	1	@ NumTrks
	.byte	0	@ NumBlks
	.byte	song91_pri	@ Priority
	.byte	song91_rev	@ Reverb.
    
	.word	song91_grp
    
	.word	song91_001

	.end
