// �����豸
#include "usart3.h"
#include "led.h"
// Э���ļ�
#include "GA10.h"
#include "mqttkit.h"
#include "delay.h"
// Ӳ������
#include "usart.h"
#include "flash.h"
//  C��
#include <string.h>
#include <stdio.h>
extern Device_Satte_Typedef device_state_init;		 // �豸״̬
extern Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
extern unsigned char esp8266_buf[256];
//==========================================================
//	�������ܣ�	ʹ��GA10����Ϣ
//==========================================================
void GA10_SendData(unsigned char *data, unsigned short len)
{
	ESP8266_Clear();			  // ��ս��ջ���
	Usart3_SendString(data, len); // �����豸������������
}
//==========================================================
//	�������ܣ�	��GA6��ָ��
//==========================================================
_Bool GA10_SendCmd(char *cmd, char *res, u16 time)
{
	char str[256];
	sprintf(str, "%s\r\n", cmd);
	// printf("%s",str);
	Usart3_SendString((unsigned char *)str, strlen((const char *)str));

	while (time--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // ����յ�����
		{
			printf("%s\n", esp8266_buf);
			if (strstr((const char *)esp8266_buf, res) != NULL) // ����������ؼ���
			{
				ESP8266_Clear(); // ��ջ���
				return 0;
			}
		}
		delay_ms(10);
	}
	return 1;
}
//==========================================================
//	�������ܣ�	��ʼ��GA10
//==========================================================
void GA10_Init(void)
{
	char str[125];
	delay_ms(250);

	ESP8266_Clear();

	printf("����ͨ���Ƿ�ɹ�... ...\r\n"); // ���������Ϣ

	while (GA10_SendCmd("AT", "OK", 500))
		delay_ms(300);

	printf("������������.. ...\r\n"); // ���������Ϣ

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTCFG=\"cloud\",0,2,1");
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);

	printf("�������Ӳ���... ...\r\n"); // ���������Ϣ

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"", PROID, DEVID,AUTH_INFO);
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);

	printf("���ӷ�����\r\n");

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTOPEN=0,\"%s\",%d", ServerIP, ServerPort);
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);
	
	printf("�����豸\r\n");

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTCONN=0,\"aliauth\" ");
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);
	
	printf("��������\r\n");

	memset(str, 0, sizeof(str));
	sprintf(str, "AT+MTSUB=0,1,\"%s\",0",S_TOPIC_NAME);
	while (GA10_SendCmd(str, "OK", 500))
		delay_ms(300);

	printf("GA10 OK\r\n");
	// Sys_Restart();//�����λ
}
//==========================================================
//	�������ܣ�	������Ϣ
//==========================================================
unsigned char FillBuf(char *buf)
{
	char text[256];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"light1\":\"%d\",", device_state_init.light_V); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"car\":\"%d\",", Data_init.car); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"R_Time\":\"%d\",", device_state_init.light_R_Timr); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"G_Time\":\"%d\"", device_state_init.light_G_Timr); // Temp����������һ�����ƣ�temper���¶�ֵ
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
		body_len = FillBuf(buf); // ������
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
// ����json����
void massage_ga10_json(char *message)
{

	cJSON *cjson_test = NULL; // ���json��ʽ
	cJSON *cjson_data = NULL; // ����
	const char *massage;
	// ������������
	u8 cjson_cmd; // ָ��,����

	/* ��������JSO���� */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// ����ʧ��
		printf("parse fail.\n");
		printf("%s\n",message);
		//return MY_FAIL;
	}

	/* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* ����Ƕ��json���� */
	cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{

	case 0x01: // ��Ϣ��
		device_state_init.light_R_Timr = cJSON_GetObjectItem(cjson_data, "r")->valueint;
		device_state_init.light_G_Timr = cJSON_GetObjectItem(cjson_data, "g")->valueint;
		Data_init.App = 1;
		W_Test();
		break;

	default:
		break;
	}

	/* ���JSON����(��������)���������� */
	cJSON_Delete(cjson_test);

	//return MY_SUCCESSFUL;
}
