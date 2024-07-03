.text
.global _start
_start:
ldr r0, =0xFF709000 //gpio data
mov r1, #(1<<25) // set 25 bit pushbutton
mov r2, (1<<24) // set 24 bit led out put
str r2, [r0, #4]

loop:
ldr r3,[r0,#0x50]
and r3,r3,r1
lsr r3,r3,#1
str r3,[r0]
b loop
end: 
b
end
.end
