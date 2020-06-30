
int gboot_main()
{ 	
     int num;
     //unsigned char buf[1024];
     #ifdef MMU_ON  //如果定义了MMU_ON
     mmu_init();
     #endif

     led_init();
       
     button_init();
     
     irq_init();
     
     uart_init();
          
     dma_init();
    
     dma_start();
     
     lcd_init(); 
     
     point(100,100, 0XFCF);
 
     clearSrc();
      
     while(1)
     {
       	printf("\n***************************************\n\r");
    	printf("\n*****************GBOOT*****************\n\r");
    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
    	printf("2:Boot Linux from RAM!\n\r");
    	printf("3:Boor Linux from Nand Flash!\n\r");  //换行，并回到行首
    	printf("\nPlese Select:");
    	
    	
    	scanf("%d", &num);
    	
    	switch (num)
    	{
         case 1:
       
        
         break;
         
         case 2:
     
         break;
         
         case 3:
      
         break;	
         
         default:
         printf("Error: wrong selection!\n\r");
         break;	
    	}
     }
     
     return 0;        
}
