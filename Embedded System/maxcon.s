.text
.global _start
_start: 	mov r0, #54
	mov r1, #24

gcd: cmp r0,r1
	subgt r0,r0,r1
	sublt r1,r1,r0
	bne  	gcd

stop:
b stop

.end
