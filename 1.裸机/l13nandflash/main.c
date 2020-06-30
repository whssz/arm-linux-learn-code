
int gboot_main()
{ 	
     unsigned char buf[1024];
     #ifdef MMU_ON  //如果定义了MMU_ON
     mmu_init();
     #endif

     led_init();
       
     button_init();
     
     irq_init();
     NF_Erase(32*2+1);
     
     buf[0] = 100;
     NF_WritePage(32*2+1, buf);
      buf[0] = 50;
      NF_PageRead(32*2+1, buf);
      if( buf[0] ==100)
      led_on();
     
        
     while(1);
     
     return 0;        
}
