.text
.global _start
_start: 
ldr r0, =AVECTOR 
ldr r1, =BVECTOR
ldr r2, N	
mov r3, #0

loop:
ldr r4,[r0],#4
ldr r5,[r1],#4
mla r3, r4, r5, r3 

subs r2,r2,#1
bgt loop
str r3, DOTP
stop:
b stop

N: 	.word 6
AVECTOR: .word 5,3,-6,19,8,12
BVECTOR: .word 2,14,-3,2,-5,36
DOTP:	.space 4
.end
