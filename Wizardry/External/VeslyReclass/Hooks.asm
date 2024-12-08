
.thumb

.global CallSomeProc
.type CallSomeProc, %function 
CallSomeProc: 
push  {r14}
pop {r3} 
bx r3 
.ltorg 

