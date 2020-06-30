#ifndef   _GY_50_
#define   _GY_50_

//**********L3G4200D内部寄存器地址*********
#define WHO_AM_I 0x0f
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28  /*数据输出寄存器*/
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38

#define	SlaveAddress   0x69	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
#define I2C_RDWR   0x0707     //I2C驱动读写命令
#define MDPS_DIGT  (float)0.07
void Single_WriteL3G4200D(unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_ReadL3G4200D(unsigned char REG_Address);
unsigned char X_Y_Z_display(void);

struct i2c_msg {
	unsigned short addr;	/* slave address			*/
	unsigned short flags;   /*读写方向,0写1读*/
	unsigned short len;		/* msg length				*/
	unsigned char *buf;		/* pointer to msg data			*/
};

 
struct i2c_rdwr_ioctl_data {
	struct i2c_msg  *msgs;	/* pointers to i2c_msgs */
	unsigned int nmsgs;	/* number of i2c_msgs */
};
#endif