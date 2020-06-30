#define GPBCON 0x56000010
#define GPBDAT 0x56000014	

int gboot_main()
{
	__asm__(
	 "ldr r1, =0x15400\n"
	 "str r1, [%0]\n"
	 "ldr r1, =0x6BF\n"
	 "str r1, [%1]\n"
	 :
	 :"r"(GPBCON),"r"(GPBDAT)
	 :"r1"
 	);
         return 0;
         
}
