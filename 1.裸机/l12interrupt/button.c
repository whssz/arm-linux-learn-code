#define GPGCON  (volatile unsigned long*)0x56000060

#define GPG0_MSK     (3<<(2*0))
#define GPG3_MSK     (3<<(2*3))
#define GPG5_MSK     (3<<(2*5))
#define GPG6_MSK     (3<<(2*6))

#define GPI0_INT     (0x2<<(2*0)) 
#define GPI3_INT     (0x2<<(2*3))     
#define GPI5_INT     (0x2<<(2*5)) 
#define GPI6_INT     (0x2<<(2*6))    

void button_init()
{
  *(GPGCON) &=  ~( GPG0_MSK | GPG3_MSK | GPG5_MSK | GPG6_MSK );
  *(GPGCON) |= GPI0_INT|GPI3_INT|GPI5_INT|GPI6_INT;
}
