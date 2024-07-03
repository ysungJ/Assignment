#include<stdio.h>

int main(void)
{

volatile int *LED_ptr = (int *) 0xFF200000;
volatile int *SW_swithc_ptr = (int *) 0xFF200040;
volatile int *KEY_ptr = (int *) 0xFF200050;
int SW_value;
volatile int delay_count;

SW_value = *SW_switch_ptr;
while(1){
if(*KEY_ptr != 0){
SW_value = *(SW_switch_ptr):
while(*KEY_ptr != 0);
}
*LED_ptr = SW_value;
SW_value = (SW_value << 1) | ((SW_value & 0x200) != 0);
for(delay_count = 500000; delay_count != 0; --delay_count)
;
}
}