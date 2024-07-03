#define bit_25_pattern 0x02000000

int main(void)
{
volatile int *HPS_GPI01_Data = (int *) 0xFF709000;
volatile int *HPS_GPI01_Direction = (int *) 0xFF709004;
volatile int *HPS_GPI01_External = (int *) 0xFF709050;

*HPS_GPI01_Direction = (1<<24);

while(1)
{
int value = *HPS_GPI01_External;
value & = bit_25_pattern;
HPS_GPI01_Data = (value >> 1);

}
}