#define MMU_FULL_ACCESS   (3 << 10)
#define MMU_DOMIN     (0 << 5)
#define MMU_SPECIAL   (1 << 4)
#define MMU_CACHE   (1 << 3)
#define MMU_BUFFER   (1 << 2)
#define MMU_SECTION   (2)
#define MMU_LED_VP    ( MMU_FULL_ACCESS | MMU_DOMIN | MMU_SPECIAL | MMU_SECTION )
#define MMU_DRAM_VP   ( MMU_FULL_ACCESS | MMU_DOMIN | MMU_SPECIAL | MMU_SECTION | MMU_CACHE | MMU_BUFFER)

void creat_section_table(void)
{
    unsigned long vaddr,paddr;
    unsigned long *ttb = (unsigned long *)0x30000000; //�����ַ
      
    vaddr = 0xa0000000;  // �����ַ
    paddr = 0x56000000; //  �����ַ�λ���ַ
    *(ttb + (vaddr >> 20)) = (paddr & 0xfff00000) | MMU_LED_VP;//��һ�����������������Ҫ�Ķλ���ַ�Ͷε�����
	
    vaddr = 0x30000000;
    paddr = 0x30000000;
    while(vaddr < 0x34000000)//ѭ����64M
    {
        *(ttb + (vaddr >> 20)) = (paddr & 0xfff00000) | MMU_DRAM_VP;
        vaddr += 0x100000;  //��Ϊ�εĵ�λ��1M 
	paddr += 0x100000;
    }	
}

void mmu_enable()//ʹ��mmu
{
     __asm__(
     
     "ldr r0,  =0x30000000\n"
     "mcr p15, 0, r0, c2, c0, 0\n"  
     
     "mvn r0, #0\n"
     "mcr p15, 0, r0, c3, c0, 0\n" 
     
     "mrc p15, 0, r0, c1, c0, 0\n"
     "orr r0, r0, #0x0001\n"
     "mcr p15, 0, r0, c1, c0, 0 \n"    
     :
     :    
   );		
}


void mmu_init()
{
   creat_section_table();
   
   mmu_enable();
   
}