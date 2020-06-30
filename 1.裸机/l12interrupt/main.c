
int gboot_main()
{ 	
     #ifdef MMU_ON  //如果定义了MMU_ON
     mmu_init();
     #endif
     int  num =0;
     led_init();
       
     button_init();
     
     port_init();
     
     irq_init();
     
     
     
     while(1);
    // humen_interrpt();   	
     return 0;        
}
