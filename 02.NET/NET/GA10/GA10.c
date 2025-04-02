// 网络设备
#include "usart3.h"
#include "led.h"
// 协议文件
#include "GA10.h"
#include "mqttkit.h"
#include "delay.h"
// 硬件驱动
#include "usart.h"
#include "flash.h"
//  C库
#include <string.h>
#include <stdio.h>
extern Device_Satte_Typedef device_state_init;		 // 设备状态
extern Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
extern unsigned char esp8266_buf[256];
//==========================================================
//	函数功能：	使用GA10发消息
//==========================================================
void GA10_SendData(unsigned char *data, unsigned short len)
{
	ESP8266_Clear();			  // 清空接收缓存
	Usart3_SendString(data, len); // 发送设备连接请求数据
}
//==========================================================
//	函数功能：	给GA6发指令
//==========================================================
_Bool GA10_SendCmd(char *cmd, char *res, u16 time)
{
	char str[256];
	sprintf(str, "%s\r\n", cmd);
	// printf("%s",str);
	Usart3_SendString((unsigned char *)str, strlen((const char *)str));

	while (time--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // 如果收到数据
		{
			printf("%s\n", esp8266_buf);
			if (strstr((const char *)esp8266_buf, res) != NULL) // 如果检索到关键词
			{
				ESP8266_Clear(); // 清空缓存
				return 0;
			}
		}
		delay_ms(10);
	}
	return 1;
}
//==========================================================
//	函数功能：	初始化GA10
//==========================================================
void GA10_Init(void)
{
	char str[125];
	delay_ms(250);

	ESP8266_Clear();

	printf("测试通信是否成功... ...\r\n"); // 串口输出信息

	while (GA10_SendCmd("AT", "OK", 500))
		delay_ms(300);

	printf("设置数据类型.. ...\r\n"); // 串口输出信息

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTCFG=\"cloud\",0,2,1");
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);

	printf("设置链接参数... ...\r\n"); // 串口输出信息

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"", PROID, DEVID,AUTH_INFO);
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);

	printf("连接服务器\r\n");

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTOPEN=0,\"%s\",%d", ServerIP, ServerPort);
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);
	
	printf("连接设备\r\n");

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTCONN=0,\"aliauth\" ");
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);
	
	printf("定义主题\r\n");

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTSUB=0,1,\"%s\",0",S_TOPIC_NAME);
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);

	printf("GA10 OK\r\n");
	// Sys_Restart();//软件复位
}
//==========================================================
//	函数功能：	发布消息
//==========================================================
unsigned char FillBuf(char *buf)
{
	char text[256];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"light1\":\"%d\",", device_state_init.light_V); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"car\":\"%d\",", Data_init.car); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"R_Time\":\"%d\",", device_state_init.light_R_Timr); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"G_Time\":\"%d\"", device_state_init.light_G_Timr); // Temp是数据流的一个名称，temper是温度值
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
void Mqtt_Pub(u8 cmd)
{

	char buf[256];
	char str[300];
	short body_len = 0;
	memset(buf, 0, sizeof(buf));
	switch (cmd)
	{
	case 1:
		body_len = FillBuf(buf); // 数据流
		break;

	default:
		break;
	}

		
	if (body_len)
	{
		sprintf(str, "AT+MTPUB=0,0,0,0,\"%s\",\"%s\"",P_TOPIC_NAME,buf);
		while (GA10_SendCmd(str, "OK", 500))
			delay_ms(300);
	}
	
	
}
// 解析json数据
void massage_ga10_json(char *message)
{

	cJSON *cjson_test = NULL; // 检测json格式
	cJSON *cjson_data = NULL; // 数据
	const char *massage;
	// 定义数据类型
	u8 cjson_cmd; // 指令,方向

	/* 解析整段JSO数据 */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// 解析失败
		printf("parse fail.\n");
		printf("%s\n",message);
		//return MY_FAIL;
	}

	/* 依次根据名称提取JSON数据（键值对） */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* 解析嵌套json数据 */
	cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{

	case 0x01: // 消息包
		device_state_init.light_R_Timr = cJSON_GetObjectItem(cjson_data, "r")->valueint;
		device_state_init.light_G_Timr = cJSON_GetObjectItem(cjson_data, "g")->valueint;
		Data_init.App = 1;
		W_Test();
		break;

	default:
		break;
	}

	/* 清空JSON对象(整条链表)的所有数据 */
	cJSON_Delete(cjson_test);

	//return MY_SUCCESSFUL;
}
