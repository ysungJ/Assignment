.text
.global _start
_start:
LDR R0, =0xFF200000
LDR R1, =0xFF200040
LOOP:
LDR R3,[R1]
STR R3,[R0]
B LOOP
END: B END


.end