void default_int_handler(void)
{
	volatile unsigned char i = 1;

	while(i)
		;
}
