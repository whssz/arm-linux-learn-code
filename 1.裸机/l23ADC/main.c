//char *buf = "hello world";

int gboot_main()
{ 
    int ret=0;
    int num;
    unsigned char humi=0; 
    unsigned char temp=0;
   
    
#ifdef MMU_ON
    mmu_init();
#endif

    //led_init();
    
    button_init();
    
    init_irq();
    
    uart_init();
    
    adc_init();
    printf("11");
    //ts_init();
    
    //lcd_init(); 
    
    //lcd_test();
    
   // dm9000_init();
   // spi_port_init();
  // printf("\n***************************************\n\r");
   //while(dht_port_init())
      // printf("dev fail!\n");
    
     
    while(1)
    {
    	
	printf("\n***************************************\n\r");
    	printf("\n*****************GBOOT*****************\n\r");
    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
    	printf("2:Boot Linux from RAM!\n\r");
    	printf("3:Boor Linux from Nand Flash!\n\r");
    	printf("\n Plese Select:");
    	
    	scanf("%d",&num);
    
        switch (num)
        {
            case 1:
               // ret = DHT11_Read_Data(&temp, &humi); 
                //if(0 == ret)
                   // printf("temp: %d, humi: %d\n\r", temp, humi);
               // else
                   // printf("get the envir fail!\n");
            //tftp_load();
            //tftp_send_request("zImage");
            break;
            
            case 2:
             adc_data();        
            //writeb(0x20);
            //ret = readb();
            //printf("%x\n", ret);
           // boot_linux();         
            break;
                
            case 3:
            //arp_request();
            break;
            
            default:
                printf("Error: wrong selection!\n\r");
            break;	
        }
    	
    
    }
    return 0;    
}


