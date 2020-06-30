#include "string.h"
#include "arp.h"

u8 sendbuf[1024];
u8* tftp_down_addr = 0x31000000;
u16 serverport = 0;
u16 curblock = 1;

#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

u16 checksum(u8 *ptr, int len)/*IPchek的校验和计算*/
{
    u32 sum = 0;
    u16 *p = (u16 *)ptr;

    while (len > 1)
    {
        sum += *p++;
        len -= 2;
    }
    
    if(len == 1)
        sum += *(u8 *)p;
    
    while(sum>>16)
        sum = (sum&0xffff) + (sum>>16);
    
    return (u16)((~sum)&0xffff);
}

void tftp_send_request(const char *filename)  //tftp发送请求包
{
    u8 *ptftp = &sendbuf[200];  //空出前面200个字节
    u32 tftp_len = 0;
    UDP_HDR *udphdr;
    u8 *iphdr;
    /*操作码，两个字节，需要反序*/ 
    ptftp[0] = 0x00;  
    ptftp[1] = 0x01;
    tftp_len += 2 ;
    
    sprintf(&ptftp[tftp_len],"%s",filename); //把文件名写入到数组中
    tftp_len += strlen(filename); 
    ptftp[tftp_len] = '\0';
    tftp_len += 1;
    
    sprintf(&ptftp[tftp_len],"%s","octet"); /*写入固定值*/
    tftp_len += strlen("octet");
    ptftp[tftp_len] = '\0';
    tftp_len += 1;
    
    /*tftp 包构建完了，tftp包含在UDP包中*/
    
    udphdr = ptftp-sizeof(UDP_HDR);
    iphdr =  ptftp-sizeof(UDP_HDR)+ sizeof(ETH_HDR);
    
    /*UDP帧头信息*/
    udphdr->sport = HON(48915);
    udphdr->dport = HON(69);
    udphdr->len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(IP_HDR));
    udphdr->udpchksum = 0x00;
    
    /*IP帧头信息*/
    udphdr->iphdr.vhl = 0x45;
    udphdr->iphdr.tos = 0x00;
    udphdr->iphdr.len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(ETH_HDR));
    udphdr->iphdr.ipid = HON(0x00);
    udphdr->iphdr.ipoffset = HON(0x4000);
    udphdr->iphdr.ttl = 0xff;
    udphdr->iphdr.proto = 17;
    memcpy(udphdr->iphdr.srcipaddr,ip_addr,4);
    memcpy(udphdr->iphdr.destipaddr,host_ip_addr,4);
    udphdr->iphdr.ipchksum = 0;
    udphdr->iphdr.ipchksum = checksum(iphdr,20);
    
    memcpy(udphdr->iphdr.ethhdr.s_mac,mac_addr,6);
    memcpy(udphdr->iphdr.ethhdr.d_mac,host_mac_addr,6);
    udphdr->iphdr.ethhdr.type = HON(PROTO_IP);
    
    dm9000_tx((u32 *)udphdr,sizeof(UDP_HDR)+tftp_len);
}

void tftp_send_ack(u16 blocknum)//tftp的响应包
{
    u8 *ptftp = &sendbuf[200];
    u32 tftp_len = 0;
    UDP_HDR *udphdr;
    u8 *iphdr;
    
    ptftp[0] = 0x00;
    ptftp[1] = 0x04;
    tftp_len += 2 ;
    
    ptftp[2] = (blocknum&0xff00)>>8;
    ptftp[3] = (blocknum&0xff);
    tftp_len += 2 ;
    
    udphdr = ptftp-sizeof(UDP_HDR);
    iphdr =  ptftp-sizeof(UDP_HDR)+ sizeof(ETH_HDR);/*注意类型*/
    
    /*UDP帧头信息*/
    udphdr->sport = HON(48915);
    udphdr->dport = HON(serverport);
    udphdr->len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(IP_HDR)); /*实际的长度，包含了tftp包，还要减去伪首部*/
    udphdr->udpchksum = 0x00;
    
    /*IP帧头信息*/
    udphdr->iphdr.vhl = 0x45;
    udphdr->iphdr.tos = 0x00;
    udphdr->iphdr.len = HON(tftp_len+sizeof(UDP_HDR)-sizeof(ETH_HDR));/*只需要减去多余的以太网的大小*/
    udphdr->iphdr.ipid = HON(0x00);
    udphdr->iphdr.ipoffset = HON(0x4000);
    udphdr->iphdr.ttl = 0xff;
    udphdr->iphdr.proto = 17;
    memcpy(udphdr->iphdr.srcipaddr,ip_addr,4);
    memcpy(udphdr->iphdr.destipaddr,host_ip_addr,4);
    udphdr->iphdr.ipchksum = 0;
    udphdr->iphdr.ipchksum = checksum(iphdr,20);
    
    memcpy(udphdr->iphdr.ethhdr.s_mac,mac_addr,6);
    memcpy(udphdr->iphdr.ethhdr.d_mac,host_mac_addr,6);
    udphdr->iphdr.ethhdr.type = HON(PROTO_IP);
    
    dm9000_tx((u32 *)udphdr,sizeof(UDP_HDR)+tftp_len);
}

/*tftp执行函数*/
void tftp_process(u8 *buf, u32 len, u16 port)
{
     u32 i;
     u32 tftp_len;
     
     serverport = port;
     
     TFTP_PAK *ptftp = buf + sizeof(UDP_HDR);
     
     tftp_len = len - sizeof(UDP_HDR);

     if(HON(ptftp->opcode) == 3)
     {
     	 if (HON(ptftp->blocknum) == curblock)/*加上重复存储判断*/
     	 {
              for (i = 0;i<(tftp_len-4);i++)/*只要数据，所以减去4个字节*/
              {
                  *(tftp_down_addr) = *(ptftp->data+i);
              }	
         
              tftp_send_ack(HON(ptftp->blocknum));
              
              curblock++;
              /*如果是最后一个包的话，肯定长度不是512字节*/
              if ((tftp_len-4)<512)
                  curblock = 1;
         }
    }
}
