
#ifndef    _CMDCODE_H_
#define    _CMDCODE_H_

/*通讯命令定义*/
#define CMD_RD_DEVICE_INFO              0x01 /*查询模块型号*/  
#define CMD_SET_SLOT_ADDRESS            0x42           // 设置盘管理器地址号
#define CMD_RD_CURRENT            	 	0x03 /*查询当前状态*/  
#define CMD_RD_LABEL					0x04 /*读取标签数据*/
#define CMD_WR_LABEL            	 	0x44 /*写标签*/ 
#define CMD_UPDATE						0x45 /*升级文件下载*/


#endif
