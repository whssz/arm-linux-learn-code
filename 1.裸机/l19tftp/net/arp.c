#include "arp.h"

#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

/*1.发送arp请求包*/
void arp_request()
{
     /*1.构成arp请求包*/
     memcpy(arpbuf.ethhdr.d_mac,host_mac_addr,6);
     memcpy(arpbuf.ethhdr.s_mac,mac_addr,6);
     arpbuf.ethhdr.type = HON(0x0806);
       
     arpbuf.hwtype = HON(1);
     arpbuf.protocol = HON(0x0800);
     
     arpbuf.hwlen = 6;
     arpbuf.protolen = 4;
        
     arpbuf.opcode = HON(1);
 
     memcpy(arpbuf.smac,mac_addr,6);
     memcpy(arpbuf.sipaddr,ip_addr,4);
     memcpy(arpbuf.dipaddr,host_ip_addr,4);

     packet_len = 14+28;
     
     /*2.调用dm9000发送函数，发送应答包*/	
     dm9000_tx(&arpbuf,packet_len);
}


/*2.解析arp应答包，提取mac*/
u8 arp_process(u8 *buf, u32 len)
{
    u32 i;
    ARP_HDR *arp_p = (ARP_HDR *)buf;
    
    if (packet_len<28)
        return 0;
    
    switch (HON(arp_p->opcode))//判断是arp请求包还是应答包
    {
    	case 2: /*arp响应包*/
            memcpy(host_ip_addr,arp_p->sipaddr,4);
            printf("host ip is : ");
            for(i=0;i<4;i++)
            printf("%03d ",host_ip_addr[i]);
            printf("\n\r");
    
            memcpy(host_mac_addr,arp_p->smac,6);
            printf("host mac is : ");
            for(i=0;i<6;i++)
            printf("%02x ",host_mac_addr[i]);
            printf("\n\r");
    
            break;
            
        case 1: /*arp请求包*/
        /*发送arp响应包*/
            memcpy(arpbuf.ethhdr.d_mac,arp_p->smac,6);  //请求包里面提取的目的以太网地址
            memcpy(arpbuf.ethhdr.s_mac,mac_addr,6);//开发板以太网地址
            arpbuf.ethhdr.type = HON(0x0806);
       
            arpbuf.hwtype = HON(1);
            arpbuf.protocol = HON(0x0800);
     
            arpbuf.hwlen = 6;
            arpbuf.protolen = 4;
        
            arpbuf.opcode = HON(2);
 
            memcpy(arpbuf.smac,mac_addr,6); //开发板mac地址
            memcpy(arpbuf.sipaddr,ip_addr,4); //开发板IP地址
            memcpy(arpbuf.dmac,arp_p->smac,6); //目的mac地址
            memcpy(arpbuf.dipaddr,arp_p->sipaddr,4); //目的IP地址，都在请求包arp_p 里面拿到

            packet_len = 14+28;
     
         /*2.调用dm9000发送函数，发送应答包*/	
            dm9000_tx(&arpbuf,packet_len);
            
            break;
     }
}

void udp_process(u8* buf, u32 len)
{
     UDP_HDR *udphdr = (UDP_HDR *)buf;
     
     tftp_process(buf,len,HON(udphdr->sport));     	
}

void ip_process(u8 *buf, u32 len)  //IP包的处理函数
{
     IP_HDR *p = (IP_HDR *)buf;	
     
     switch(p->proto)
     {
         case PROTO_UDP:
             udp_process(buf,len);
             break;
             
         default:
             break;	
     }
}

void net_handle(u8 *buf, u32 len)  //数据处理函数
{
     ETH_HDR *p = (ETH_HDR *)buf;  //定义一个以太网结构类型的指针
     
     switch (HON(p->type)) //查看二层协议类型，注意字节序的转换
     {
          case PROTO_ARP: //如果是arp包
         	arp_process(buf,len);
         	break;
          
          case PROTO_IP: //如果是IP包
                ip_process(buf,len);
     	        break;
     	  
     	  default:
     	        break;
     }
}