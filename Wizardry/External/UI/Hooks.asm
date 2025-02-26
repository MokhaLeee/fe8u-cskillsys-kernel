
.thumb

.global DecompressImg_ShopGoldBox_Hook
.type DecompressImg_ShopGoldBox_Hook, %function 
DecompressImg_ShopGoldBox_Hook: 
push  {r14}
@bl DecompressImg_ShopGoldBox
pop {r3} 
ldr r3, =0x80B4E31 
bx r3 
.ltorg 

.global Decompress_Img_PrepItemUseScreen_Hook
.type Decompress_Img_PrepItemUseScreen_Hook, %function 
Decompress_Img_PrepItemUseScreen_Hook: 
push  {r14}
bl Decompress_Img_PrepItemUseScreen
pop {r3} 
ldr r3, =0x809C661 
bx r3 
.ltorg 

.global PrepUiPalHook
.type PrepUiPalHook, %function 
PrepUiPalHook: 
push {lr} 
mov r5, r6 
bl PrepUiPalHook_2
pop {r3} 
bx r3 
.ltorg 


.global ekrGaugeMain_Hook
.type ekrGaugeMain_Hook, %function 
ekrGaugeMain_Hook: 
push  {r14}
bl ekrGaugeMain_GetImg
add r0, r8 
lsl r5, r7, #1 
mov r3, r9 
lsl r1, r3, #1 
ldr r3, =0x805137C 
ldr r3, [r3] 
add r2, r4, r3 
pop {r3} 
bx r3 
.ltorg 


.global ekrGaugeMain_Hook2
.type ekrGaugeMain_Hook2, %function 
ekrGaugeMain_Hook2: 
push  {r14}
ldr r0, [r1, #0x4C] 
cmp r0, #0 
bne Load2428 
bl ekrGaugeMain2_GetImg @ 0x8802348 
mov r3, r8 
add r2, r3, r0 
b Exit_ekrGaugeMain_Hook2
Load2428: 
@ldr r0, =0x805141C 
@ldr r0, [r0] @ 0x8802428 
bl ekrGaugeMain3_GetImg
mov r4, r8 
add r2, r4, r0 
Exit_ekrGaugeMain_Hook2: 
pop {r1} 
bx r1 
.ltorg 



.global ekrGaugeMain_Hook3
.type ekrGaugeMain_Hook3, %function 
ekrGaugeMain_Hook3: 
push  {r14}
bl ekrGaugeMain3_GetImg
mov r4, r8 
add r2, r4, r0 
lsl r5, r7, #1 
pop {r3} 
ldr r3, =0x805138D 
bx r3 
.ltorg 

@.global EfxPrepareScreenFxHook2
@.type EfxPrepareScreenFxHook2, %function 
@EfxPrepareScreenFxHook2: 
@push  {r14}
@bl ekrGaugeMain4_GetImg
@mov r6, r4 
@add r6, #0x3C 
@mov r1, #1 
@neg r1, r1 
@pop {r3} 
@ldr r3, =0x80520E5 
@bx r3 
@.ltorg 
@
@.global EfxPrepareScreenFxHook3
@.type EfxPrepareScreenFxHook3, %function 
@EfxPrepareScreenFxHook3: 
@push  {r14}
@
@ldr r3, =0x8003CB8 
@mov lr, r3 
@mov r3, #2  
@.short 0xf800 
@ldr r3, =0x8004428 
@mov lr, r3 
@.short 0xf800 
@
@bl GetImg_08801C14
@ldr r1, =0x8051F6C 
@ldr r1, [r1] @ 6001000 
@pop {r3} 
@bx r3 
@.ltorg 





