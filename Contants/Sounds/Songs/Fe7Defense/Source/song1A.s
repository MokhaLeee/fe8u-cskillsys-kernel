	.include "MPlayDef.s"

	.equ	song1A_grp, ThisInstrument
	.equ	song1A_pri, 10
	.equ	song1A_rev, 128
	.equ	song1A_mvl, 127
	.equ	song1A_key, 0
	.equ	song1A_tbs, 1
	.equ	song1A_exg, 0
	.equ	song1A_cmp, 1

	.section .rodata
	.global	song1A
	.align	2


@**************** Track 1 (Midi-Chn.0) ****************@

song1A_001:
@  #01 @000   ----------------------------------------
 .byte   KEYSH , song1A_key+0
 .byte   TEMPO , 146*song1A_tbs/2
 .byte   VOICE , 0
 .byte   VOL , 40*song1A_mvl/mxv
 .byte   W24
Label_0100DF41:
 .byte   N11 ,CsM2 ,v104
 .byte   N11 ,DnM2 ,v096
 .byte   W12
 .byte   DnM2 ,v100
 .byte   W12
 .byte   DsM2 ,v084
 .byte   W12
 .byte   N22 ,EnM2 ,v104
 .byte   W24
 .byte   N21 ,EnM2 ,v100
 .byte   W24
@  #01 @001   ----------------------------------------
 .byte   DsM2 ,v088
 .byte   W12
 .byte   W12
 .byte   N22 ,DnM2 ,v100
 .byte   W24
 .byte   N21 ,EnM2 ,v096
 .byte   W24
 .byte   N11 ,EnM2 ,v108
 .byte   W12
 .byte   DsM2 ,v092
 .byte   W12
@  #01 @002   ----------------------------------------
 .byte   CsM2 ,v072
 .byte   N11 ,DnM2 ,v084
 .byte   W12
 .byte   CsM2
 .byte   N11 ,DnM2 ,v104
 .byte   W12
 .byte   N22 ,DnM2 ,v096
 .byte   W24
 .byte   EnM2 ,v100
 .byte   W24
 .byte   DsM2 ,v088
 .byte   W24
@  #01 @003   ----------------------------------------
 .byte   N11 ,DnM2 ,v084
 .byte   W12
 .byte   DnM2 ,v100
 .byte   W12
 .byte   N22 ,DnM2 ,v096
 .byte   W24
 .byte   N11 ,DsM2 ,v080
 .byte   W12
 .byte   EnM2 ,v100
 .byte   W12
 .byte   N21 ,DsM2 ,v080
 .byte   W24
@  #01 @004   ----------------------------------------
 .byte   N11 ,CsM2 ,v068
 .byte   N11 ,DnM2 ,v084
 .byte   W12
 .byte   CsM2 ,v112
 .byte   N11 ,DnM2 ,v100
 .byte   W12
 .byte   N22 ,DnM2 ,v104
 .byte   W24
 .byte   N21 ,DsM2 ,v096
 .byte   W24
 .byte   N22 ,EnM2 ,v112
 .byte   W24
@  #01 @005   ----------------------------------------
 .byte   N11 ,DsM2 ,v084
 .byte   W12
 .byte   DnM2
 .byte   W12
 .byte   N21 ,DnM2 ,v088
 .byte   W24
 .byte   DsM2 ,v072
 .byte   W24
 .byte   N22 ,EnM2 ,v112
 .byte   W24
@  #01 @006   ----------------------------------------
 .byte   N11 ,CsM2 ,v092
 .byte   N11 ,DsM2 ,v088
 .byte   W12
 .byte   N20 ,CsM2 ,v108
 .byte   N21 ,DnM2 ,v104
 .byte   W24
 .byte   N11 ,DsM2 ,v096
 .byte   W12
 .byte   N21 ,DsM2 ,v092
 .byte   W24
 .byte   N20 ,EnM2 ,v088
 .byte   W30
@  #01 @007   ----------------------------------------
 .byte   N05 ,DsM2 ,v032
 .byte   W06
 .byte   N11 ,DsM2 ,v080
 .byte   W12
 .byte   DsM2 ,v088
 .byte   W12
 .byte   DnM2
 .byte   W12
 .byte   N21
 .byte   W24
 .byte   N11 ,DnM2 ,v084
 .byte   W12
 .byte   EnM2 ,v104
 .byte   W12
 .byte   DsM2 ,v076
 .byte   W12
@  #01 @008   ----------------------------------------
 .byte   N20 ,DnM2 ,v088
 .byte   W96
@  #01 @009   ----------------------------------------
 .byte   W96
@  #01 @010   ----------------------------------------
 .byte   W96
@  #01 @011   ----------------------------------------
 .byte   W96
@  #01 @012   ----------------------------------------
 .byte   W96
@  #01 @013   ----------------------------------------
 .byte   W96
@  #01 @014   ----------------------------------------
 .byte   W96
@  #01 @015   ----------------------------------------
 .byte   W96
@  #01 @016   ----------------------------------------
 .byte   GOTO
  .word Label_0100DF41
 .byte   FINE

@**************** Track 2 (Midi-Chn.1) ****************@

song1A_002:
@  #02 @000   ----------------------------------------
 .byte   KEYSH , song1A_key+0
 .byte   VOICE , 5
 .byte   VOL , 41*song1A_mvl/mxv
 .byte   PAN , c_v+9
 .byte   W24
Label_0100E00B:
 .byte   W24
 .byte   N05 ,Gs2 ,v092
 .byte   N06 ,As2
 .byte   N07 ,Cs3 ,v112
 .byte   W12
 .byte   N05 ,Gs2 ,v028
 .byte   N06 ,As2
 .byte   N07 ,Cs3 ,v032
 .byte   W12
 .byte   N90 ,Fs2 ,v092
 .byte   N90 ,Gs2
 .byte   N92 ,Cn3 ,v112
 .byte   W48
@  #02 @001   ----------------------------------------
 .byte   W48
 .byte   N40 ,Fn2 ,v092
 .byte   N40 ,Gn2
 .byte   N42 ,Bn2 ,v112
 .byte   W48
@  #02 @002   ----------------------------------------
 .byte   N09 ,Fs2 ,v092
 .byte   N09 ,Gs2
 .byte   N11 ,Cn3 ,v112
 .byte   W12
 .byte   N05 ,Fs2 ,v028
 .byte   N06 ,Gs2
 .byte   N07 ,Cn3 ,v032
 .byte   W84
@  #02 @003   ----------------------------------------
 .byte   W48
 .byte   N11 ,Cn3 ,v100
 .byte   W12
 .byte   Gn3 ,v092
 .byte   W12
 .byte   Cn4 ,v096
 .byte   W12
 .byte   Gn4 ,v076
 .byte   W12
@  #02 @004   ----------------------------------------
 .byte   N07 ,Gn4 ,v032
 .byte   W24
 .byte   N04 ,Gs2 ,v092
 .byte   N04 ,As2
 .byte   N05 ,Cs3 ,v112
 .byte   W06
 .byte   Gs2 ,v028
 .byte   N06 ,As2
 .byte   N07 ,Cs3 ,v032
 .byte   W18
 .byte   N84 ,Fs2 ,v092
 .byte   N84 ,Gs2
 .byte   N88 ,Cn3 ,v112
 .byte   W48
@  #02 @005   ----------------------------------------
 .byte   W48
 .byte   N11 ,Fn2 ,v092
 .byte   N08 ,Gs2
 .byte   N11 ,Bn2 ,v112
 .byte   W12
 .byte   N05 ,Fn2 ,v028
 .byte   N05 ,Gs2
 .byte   N05 ,Bn2 ,v032
 .byte   W12
 .byte   N08 ,Fs2 ,v092
 .byte   N09 ,Gs2
 .byte   N09 ,Cn3 ,v112
 .byte   W12
 .byte   N05 ,Fs2 ,v028
 .byte   N05 ,Gs2
 .byte   N05 ,Cn3 ,v032
 .byte   W12
@  #02 @006   ----------------------------------------
 .byte   N64 ,Gs2 ,v092
 .byte   N64 ,As2
 .byte   N64 ,Ds3 ,v112
 .byte   W66
 .byte   N05 ,Gs2 ,v028
 .byte   N05 ,As2
 .byte   N05 ,Ds3 ,v032
 .byte   W06
 .byte   N18 ,Fs2 ,v092
 .byte   N16 ,As2
 .byte   N17 ,Cs3 ,v112
 .byte   W24
@  #02 @007   ----------------------------------------
 .byte   N68 ,Fn2 ,v092
 .byte   N68 ,Gs2
 .byte   N68 ,Cn3 ,v112
 .byte   W72
 .byte   N05 ,Fn2 ,v028
 .byte   N06 ,An2
 .byte   N07 ,Cn3 ,v032
 .byte   W24
@  #02 @008   ----------------------------------------
 .byte   W96
@  #02 @009   ----------------------------------------
 .byte   W96
@  #02 @010   ----------------------------------------
 .byte   W96
@  #02 @011   ----------------------------------------
 .byte   W96
@  #02 @012   ----------------------------------------
 .byte   W96
@  #02 @013   ----------------------------------------
 .byte   W96
@  #02 @014   ----------------------------------------
 .byte   W96
@  #02 @015   ----------------------------------------
 .byte   W96
@  #02 @016   ----------------------------------------
 .byte   GOTO
  .word Label_0100E00B
 .byte   FINE

@**************** Track 3 (Midi-Chn.2) ****************@

song1A_003:
@  #03 @000   ----------------------------------------
 .byte   KEYSH , song1A_key+0
 .byte   VOICE , 6
 .byte   VOL , 41*song1A_mvl/mxv
 .byte   PAN , c_v+0
 .byte   W24
Label_0100E0D8:
 .byte   N10 ,Cn1 ,v104
 .byte   W12
 .byte   Gn1 ,v100
 .byte   W12
 .byte   N11 ,Cn1 ,v112
 .byte   W12
 .byte   N08 ,As1 ,v100
 .byte   W12
 .byte   N10 ,Cn1 ,v108
 .byte   W12
 .byte   N08 ,Gn1 ,v096
 .byte   W12
@  #03 @001   ----------------------------------------
 .byte   Cn1 ,v116
 .byte   W12
 .byte   N05 ,As1 ,v112
 .byte   W12
 .byte   PEND 
Label_0100E0F7:
 .byte   N08 ,Cn1 ,v116
 .byte   W12
 .byte   N04 ,Gn1 ,v100
 .byte   W12
 .byte   N08 ,Cn1 ,v112
 .byte   W12
 .byte   N06 ,As1 ,v120
 .byte   W12
 .byte   N11 ,Cn1 ,v116
 .byte   W12
 .byte   N06 ,Gn1 ,v100
 .byte   W12
@  #03 @002   ----------------------------------------
 .byte   N10 ,Cn1 ,v116
 .byte   W12
 .byte   N04 ,As1 ,v112
 .byte   W12
 .byte   PEND 
Label_0100E118:
 .byte   N10 ,Cn1 ,v127
 .byte   W12
 .byte   N05 ,Gn1 ,v104
 .byte   W12
 .byte   N10 ,Cn1 ,v124
 .byte   W12
 .byte   N04 ,As1 ,v108
 .byte   W12
 .byte   N11 ,Cn1 ,v120
 .byte   W12
 .byte   N04 ,Gn1 ,v100
 .byte   W12
@  #03 @003   ----------------------------------------
 .byte   N11 ,Cn1 ,v120
 .byte   W12
 .byte   N04 ,As1 ,v104
 .byte   W12
 .byte   PEND 
Label_0100E139:
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,Gn1 ,v100
 .byte   W12
 .byte   N11 ,Cn1 ,v124
 .byte   W12
 .byte   N05 ,As1 ,v116
 .byte   W12
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,Gn1 ,v104
 .byte   W12
@  #03 @004   ----------------------------------------
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,As1 ,v112
 .byte   W12
 .byte   PEND 
Label_0100E15A:
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,Gn1 ,v104
 .byte   W12
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,As1 ,v112
 .byte   W12
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,Gn1 ,v104
 .byte   W12
@  #03 @005   ----------------------------------------
 .byte   N11 ,Cn1 ,v124
 .byte   W12
 .byte   N04 ,As1 ,v116
 .byte   W12
 .byte   PEND 
Label_0100E17B:
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N04 ,Gn1 ,v104
 .byte   W12
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N05 ,As1 ,v112
 .byte   W12
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N03 ,Gn1 ,v100
 .byte   W12
@  #03 @006   ----------------------------------------
 .byte   N11 ,Cn1 ,v127
 .byte   W12
 .byte   N05 ,As1 ,v120
 .byte   W12
 .byte   PEND 
 .byte   TIE ,Cs1 ,v108
 .byte   W96
@  #03 @007   ----------------------------------------
 .byte   W92
 .byte   EOT
 .byte   W04
@  #03 @008   ----------------------------------------
 .byte   PATT
  .word Label_0100E0D8
@  #03 @009   ----------------------------------------
 .byte   PATT
  .word Label_0100E0F7
@  #03 @010   ----------------------------------------
 .byte   PATT
  .word Label_0100E118
@  #03 @011   ----------------------------------------
 .byte   PATT
  .word Label_0100E139
@  #03 @012   ----------------------------------------
 .byte   PATT
  .word Label_0100E15A
@  #03 @013   ----------------------------------------
 .byte   PATT
  .word Label_0100E17B
@  #03 @014   ----------------------------------------
 .byte   TIE ,Cs1 ,v108
 .byte   W96
@  #03 @015   ----------------------------------------
 .byte   W92
 .byte   EOT
 .byte   W04
@  #03 @016   ----------------------------------------
 .byte   GOTO
  .word Label_0100E0D8
 .byte   FINE

@**************** Track 4 (Midi-Chn.3) ****************@

song1A_004:
@  #04 @000   ----------------------------------------
 .byte   KEYSH , song1A_key+0
 .byte   VOICE , 7
 .byte   VOL , 37*song1A_mvl/mxv
 .byte   PAN , c_v-3
 .byte   N05 ,Gs2 ,v108
 .byte   W06
 .byte   En2 ,v088
 .byte   W06
 .byte   Gs2 ,v100
 .byte   W06
 .byte   En2 ,v108
 .byte   W06
Label_0100E1E3:
 .byte   N11 ,Gs2 ,v127
 .byte   W12
 .byte   N23 ,En2 ,v088
 .byte   W24
 .byte   N11 ,En2 ,v100
 .byte   W12
 .byte   Gs2
 .byte   W12
 .byte   N23 ,En2 ,v088
 .byte   W24
@  #04 @001   ----------------------------------------
 .byte   N11 ,En2 ,v100
 .byte   W12
 .byte   PEND 
Label_0100E1FA:
 .byte   N11 ,Gs2 ,v104
 .byte   W12
 .byte   N23 ,En2 ,v100
 .byte   W24
 .byte   N11 ,En2 ,v104
 .byte   W12
 .byte   Gs2 ,v108
 .byte   W12
 .byte   N23 ,En2 ,v092
 .byte   W24
@  #04 @002   ----------------------------------------
 .byte   N11 ,En2 ,v104
 .byte   W12
 .byte   PEND 
Label_0100E212:
 .byte   N11 ,Gs2 ,v112
 .byte   W12
 .byte   N23 ,En2 ,v084
 .byte   W24
 .byte   N11 ,En2 ,v100
 .byte   W12
 .byte   Gs2 ,v112
 .byte   W12
 .byte   N23 ,En2 ,v088
 .byte   W24
@  #04 @003   ----------------------------------------
 .byte   N11 ,En2 ,v092
 .byte   W12
 .byte   PEND 
Label_0100E22A:
 .byte   N11 ,Gs2 ,v116
 .byte   W12
 .byte   N23 ,En2 ,v084
 .byte   W24
 .byte   N11 ,En2 ,v104
 .byte   W12
 .byte   Gs2 ,v108
 .byte   W12
 .byte   En2 ,v092
 .byte   W12
 .byte   N05 ,Gs2 ,v104
 .byte   W06
 .byte   En2 ,v092
 .byte   W06
@  #04 @004   ----------------------------------------
 .byte   Gs2 ,v096
 .byte   W06
 .byte   En2 ,v104
 .byte   W06
 .byte   PEND 
Label_0100E24A:
 .byte   N11 ,Gs2 ,v127
 .byte   W12
 .byte   N23 ,En2 ,v068
 .byte   W24
 .byte   N11 ,En2 ,v096
 .byte   W12
 .byte   Gs2 ,v112
 .byte   W12
 .byte   N23 ,En2 ,v076
 .byte   W24
@  #04 @005   ----------------------------------------
 .byte   N11 ,En2 ,v100
 .byte   W12
 .byte   PEND 
Label_0100E262:
 .byte   N11 ,Gs2 ,v108
 .byte   W12
 .byte   N23 ,En2 ,v080
 .byte   W24
 .byte   N11 ,En2 ,v096
 .byte   W12
 .byte   Gs2 ,v108
 .byte   W12
 .byte   N23 ,En2 ,v084
 .byte   W24
@  #04 @006   ----------------------------------------
 .byte   N11 ,En2 ,v092
 .byte   W12
 .byte   PEND 
 .byte   N44 ,Gs2 ,v116
 .byte   W96
@  #04 @007   ----------------------------------------
 .byte   W60
 .byte   N11 ,En2 ,v108
 .byte   W12
 .byte   Gs2 ,v084
 .byte   W12
 .byte   En2 ,v112
 .byte   W12
@  #04 @008   ----------------------------------------
 .byte   W72
 .byte   N05 ,Gs2 ,v108
 .byte   W06
 .byte   En2 ,v088
 .byte   W06
 .byte   Gs2 ,v100
 .byte   W06
 .byte   En2 ,v108
 .byte   W06
@  #04 @009   ----------------------------------------
 .byte   PATT
  .word Label_0100E1E3
@  #04 @010   ----------------------------------------
 .byte   PATT
  .word Label_0100E1FA
@  #04 @011   ----------------------------------------
 .byte   PATT
  .word Label_0100E212
@  #04 @012   ----------------------------------------
 .byte   PATT
  .word Label_0100E22A
@  #04 @013   ----------------------------------------
 .byte   PATT
  .word Label_0100E24A
@  #04 @014   ----------------------------------------
 .byte   PATT
  .word Label_0100E262
@  #04 @015   ----------------------------------------
 .byte   N44 ,Gs2 ,v116
 .byte   W96
@  #04 @016   ----------------------------------------
 .byte   GOTO
  .word Label_0100E1E3
 .byte   FINE

@**************** Track 5 (Midi-Chn.4) ****************@

song1A_005:
@  #05 @000   ----------------------------------------
 .byte   KEYSH , song1A_key+0
 .byte   VOICE , 8
 .byte   VOL , 41*song1A_mvl/mxv
 .byte   PAN , c_v-22
 .byte   N05 ,Cn1 ,v120
 .byte   N05 ,Fn2
 .byte   W06
 .byte   Gn1 ,v116
 .byte   N05 ,Cn3
 .byte   W06
 .byte   Cn2 ,v100
 .byte   N05 ,Fn3
 .byte   W06
 .byte   Fn2 ,v112
 .byte   N05 ,As3
 .byte   W06
Label_0100E2DC:
 .byte   N08 ,Gn2 ,v116
 .byte   N08 ,Cn4 ,v108
 .byte   W12
 .byte   N10 ,Gn2 ,v028
 .byte   N09 ,Cn4 ,v024
 .byte   W84
@  #05 @001   ----------------------------------------
 .byte   W96
@  #05 @002   ----------------------------------------
 .byte   W48
 .byte   N11 ,Cn2 ,v096
 .byte   W12
 .byte   Gn2 ,v092
 .byte   W12
 .byte   Cn3 ,v076
 .byte   W12
 .byte   N09 ,Gn3 ,v088
 .byte   W12
@  #05 @003   ----------------------------------------
 .byte   N11
 .byte   W12
 .byte   N07 ,Cn4 ,v100
 .byte   W12
 .byte   N11
 .byte   W12
 .byte   N10 ,Gn4 ,v096
 .byte   W12
 .byte   N11 ,Gn4 ,v108
 .byte   W12
 .byte   Cn4 ,v084
 .byte   W12
 .byte   Gn3 ,v092
 .byte   W12
 .byte   N10 ,Cn3 ,v084
 .byte   W12
@  #05 @004   ----------------------------------------
 .byte   W96
@  #05 @005   ----------------------------------------
 .byte   W96
@  #05 @006   ----------------------------------------
 .byte   W24
 .byte   N05 ,Gs2 ,v112
 .byte   W06
 .byte   As2 ,v084
 .byte   W06
 .byte   Gs2 ,v088
 .byte   W06
 .byte   As2 ,v084
 .byte   W06
 .byte   Cs3 ,v064
 .byte   W06
 .byte   Ds3 ,v080
 .byte   W06
 .byte   Cs3 ,v064
 .byte   W06
 .byte   Ds3 ,v080
 .byte   W06
 .byte   Gs3 ,v088
 .byte   W06
 .byte   As3
 .byte   W06
 .byte   Gs3 ,v084
 .byte   W06
 .byte   N06 ,As3 ,v092
 .byte   W06
@  #05 @007   ----------------------------------------
 .byte   W24
 .byte   N05 ,Gs2 ,v100
 .byte   W06
 .byte   As2 ,v080
 .byte   W06
 .byte   Gs2 ,v088
 .byte   W06
 .byte   As2 ,v084
 .byte   W06
 .byte   Cs3 ,v080
 .byte   W06
 .byte   Ds3 ,v088
 .byte   W06
 .byte   Cs3 ,v068
 .byte   W06
 .byte   Ds3 ,v084
 .byte   W06
 .byte   Gs3
 .byte   W06
 .byte   As3 ,v068
 .byte   W06
 .byte   Gs3 ,v072
 .byte   W06
 .byte   As3 ,v076
 .byte   W06
@  #05 @008   ----------------------------------------
 .byte   N23 ,Ds4 ,v072
 .byte   W24
 .byte   N03 ,Ds4 ,v020
 .byte   W24
 .byte   N11 ,Cn2 ,v088
 .byte   W12
 .byte   Ds2 ,v076
 .byte   W12
 .byte   Gn2 ,v064
 .byte   W12
 .byte   Cn3 ,v088
 .byte   W12
@  #05 @009   ----------------------------------------
 .byte   N05 ,Ds3 ,v080
 .byte   W06
 .byte   Gn2 ,v088
 .byte   W06
 .byte   Cn3 ,v068
 .byte   W06
 .byte   Ds3 ,v084
 .byte   W06
 .byte   Gn3
 .byte   W06
 .byte   Gn3 ,v068
 .byte   W06
 .byte   Cn4 ,v072
 .byte   W06
 .byte   Ds4 ,v076
 .byte   W06
 .byte   Gn4 ,v080
 .byte   W06
 .byte   Cn4 ,v088
 .byte   W06
 .byte   Cn4 ,v068
 .byte   W06
 .byte   Gn3 ,v084
 .byte   W06
 .byte   N05
 .byte   W06
 .byte   Cn3 ,v068
 .byte   W06
 .byte   Gn2 ,v072
 .byte   W06
 .byte   Cn2 ,v076
 .byte   W06
@  #05 @010   ----------------------------------------
 .byte   W24
 .byte   Cn2 ,v084
 .byte   W06
 .byte   Ds2 ,v072
 .byte   W06
 .byte   Gn2 ,v080
 .byte   W06
 .byte   Cn3 ,v100
 .byte   W06
 .byte   Cn3 ,v076
 .byte   W06
 .byte   Gn3 ,v080
 .byte   W06
 .byte   Gn3 ,v092
 .byte   W06
 .byte   Cn4
 .byte   W06
 .byte   Cn4 ,v080
 .byte   W06
 .byte   Gn3 ,v092
 .byte   W06
 .byte   N05
 .byte   W06
 .byte   Cn3 ,v076
 .byte   W06
@  #05 @011   ----------------------------------------
 .byte   Cn3 ,v084
 .byte   W06
 .byte   N06 ,Gs2
 .byte   W18
 .byte   N05 ,Cs2
 .byte   W06
 .byte   En2 ,v072
 .byte   W06
 .byte   Gs2 ,v080
 .byte   W06
 .byte   Cs3 ,v100
 .byte   W06
 .byte   Cs3 ,v076
 .byte   W06
 .byte   Gs3 ,v080
 .byte   W06
 .byte   Gs3 ,v092
 .byte   W06
 .byte   Cs4
 .byte   W06
 .byte   Cs4 ,v080
 .byte   W06
 .byte   Gs3 ,v092
 .byte   W06
 .byte   N05
 .byte   W06
 .byte   Cs3 ,v084
 .byte   W06
@  #05 @012   ----------------------------------------
 .byte   W24
 .byte   N44 ,Cs3 ,v080
 .byte   W48
 .byte   N23 ,Bn2 ,v072
 .byte   W24
@  #05 @013   ----------------------------------------
 .byte   N10 ,Gn2 ,v104
 .byte   N09 ,Cn3 ,v100
 .byte   W12
 .byte   Gn2 ,v028
 .byte   N09 ,Cn3
 .byte   W36
 .byte   N23 ,As2 ,v088
 .byte   N23 ,Ds3 ,v092
 .byte   W24
 .byte   N20 ,Gs2 ,v080
 .byte   N20 ,Cs3 ,v092
 .byte   W24
@  #05 @014   ----------------------------------------
 .byte   N44 ,Gn2
 .byte   N44 ,Cn3 ,v104
 .byte   W48
 .byte   N36 ,Fn2 ,v092
 .byte   N40 ,As2 ,v084
 .byte   W42
 .byte   N10 ,Fn2 ,v028
 .byte   N09 ,As2 ,v024
 .byte   W06
@  #05 @015   ----------------------------------------
 .byte   W48
 .byte   N36 ,Fn2 ,v092
 .byte   N40 ,As2 ,v084
 .byte   W48
@  #05 @016   ----------------------------------------
 .byte   GOTO
  .word Label_0100E2DC
 .byte   FINE

@**************** Track 6 (Midi-Chn.5) ****************@

song1A_006:
@  #06 @000   ----------------------------------------
 .byte   KEYSH , song1A_key+0
 .byte   W24
Label_0100E438:
 .byte   W96
@  #06 @001   ----------------------------------------
 .byte   W96
@  #06 @002   ----------------------------------------
 .byte   W96
@  #06 @003   ----------------------------------------
 .byte   W96
@  #06 @004   ----------------------------------------
 .byte   W96
@  #06 @005   ----------------------------------------
 .byte   W96
@  #06 @006   ----------------------------------------
 .byte   W96
@  #06 @007   ----------------------------------------
 .byte   W96
@  #06 @008   ----------------------------------------
 .byte   VOICE , 8
 .byte   VOL , 30*song1A_mvl/mxv
 .byte   PAN , c_v+16
 .byte   BEND , c_v+0
 .byte   W48
 .byte   W03
 .byte   N11 ,Cn2 ,v084
 .byte   W12
 .byte   Ds2 ,v072
 .byte   W12
 .byte   Gn2 ,v060
 .byte   W12
 .byte   Cn3 ,v084
 .byte   W09
@  #06 @009   ----------------------------------------
 .byte   W03
 .byte   N05 ,Ds3 ,v076
 .byte   W06
 .byte   Gn2 ,v084
 .byte   W06
 .byte   Cn3 ,v064
 .byte   W06
 .byte   Ds3 ,v080
 .byte   W06
 .byte   Gn3
 .byte   W06
 .byte   Gn3 ,v064
 .byte   W06
 .byte   Cn4 ,v068
 .byte   W06
 .byte   Ds4 ,v072
 .byte   W06
 .byte   Gn4 ,v076
 .byte   W06
 .byte   Cn4 ,v084
 .byte   W06
 .byte   Cn4 ,v064
 .byte   W06
 .byte   Gn3 ,v080
 .byte   W06
 .byte   N05
 .byte   W06
 .byte   Cn3 ,v064
 .byte   W06
 .byte   Gn2 ,v068
 .byte   W06
 .byte   Cn2 ,v072
 .byte   W03
@  #06 @010   ----------------------------------------
 .byte   W24
 .byte   W03
 .byte   Cn2 ,v080
 .byte   W06
 .byte   Ds2 ,v068
 .byte   W06
 .byte   Gn2 ,v076
 .byte   W06
 .byte   Cn3 ,v096
 .byte   W06
 .byte   Cn3 ,v072
 .byte   W06
 .byte   Gn3 ,v076
 .byte   W06
 .byte   Gn3 ,v088
 .byte   W06
 .byte   Cn4
 .byte   W06
 .byte   Cn4 ,v076
 .byte   W06
 .byte   Gn3 ,v088
 .byte   W06
 .byte   N05
 .byte   W06
 .byte   Cn3 ,v072
 .byte   W03
@  #06 @011   ----------------------------------------
 .byte   W03
 .byte   Cn3 ,v080
 .byte   W06
 .byte   N06 ,Gs2
 .byte   W18
 .byte   N05 ,Cs2
 .byte   W06
 .byte   En2 ,v068
 .byte   W06
 .byte   Gs2 ,v076
 .byte   W06
 .byte   Cs3 ,v096
 .byte   W06
 .byte   Cs3 ,v072
 .byte   W06
 .byte   Gs3 ,v076
 .byte   W06
 .byte   Gs3 ,v088
 .byte   W06
 .byte   Cs4
 .byte   W06
 .byte   Cs4 ,v076
 .byte   W06
 .byte   Gs3 ,v088
 .byte   W06
 .byte   N05
 .byte   W06
 .byte   Cs3 ,v080
 .byte   W03
@  #06 @012   ----------------------------------------
 .byte   W24
 .byte   N44
 .byte   W48
 .byte   N23 ,Bn2 ,v072
 .byte   W24
@  #06 @013   ----------------------------------------
 .byte   N09 ,Cn4 ,v100
 .byte   W12
 .byte   Cn4 ,v028
 .byte   W36
 .byte   N23 ,Ds4 ,v092
 .byte   W24
 .byte   N20 ,Cs4
 .byte   W24
@  #06 @014   ----------------------------------------
 .byte   N44 ,Cn4 ,v104
 .byte   W48
 .byte   N40 ,As3 ,v084
 .byte   W42
 .byte   N09 ,As3 ,v024
 .byte   W06
@  #06 @015   ----------------------------------------
 .byte   W48
 .byte   N40 ,As3 ,v084
 .byte   W48
@  #06 @016   ----------------------------------------
 .byte   GOTO
  .word Label_0100E438
 .byte   FINE

@******************************************************@
	.align	2

song1A:
	.byte	6	@ NumTrks
	.byte	0	@ NumBlks
	.byte	song1A_pri	@ Priority
	.byte	song1A_rev	@ Reverb.
    
	.word	song1A_grp
    
	.word	song1A_001
	.word	song1A_002
	.word	song1A_003
	.word	song1A_004
	.word	song1A_005
	.word	song1A_006

	.end
