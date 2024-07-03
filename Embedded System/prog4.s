.text
.global _start
_start:	
mov	r0,	#data1 //	r0,	data1위치확인
ldr	r1,	[r0] //	r1
ldr	r2,	[r0,#4] //	r2,	r0
ldr	r3,	[r0],	#4 //	r3,	r0
ldr	r4,	[r0,#4]! //	r4,	r0
mov	r8,	#src //	r8,	src위치확인
mov	r9,	#dst //	r9,	dst위치확인
ldmia	r8,	{r0-r4} //	r0-r4,	r8
stmia	r9!,{r0-r4} //	dst	메모리블록,	r9
mov	sp,	#tos //	sp
stmfd	sp!,	{r0-r4}	 //	sp,	스택메모리
ldmfd	sp!,	{r5-r9} //	sp,	r5-r9
mov	sp,	#tos
push	{r0-r4} //	sp,	스택메모리
pop	{r8-r12} //	sp,	r8-r12

stop:	
b stop

src: .word	0x1111,	0x2222,	0x3333,	0x4444,	0x5555	
dst: .space	20
data1: .word	0x1234,	0x5678
stack:	
 .space	256
