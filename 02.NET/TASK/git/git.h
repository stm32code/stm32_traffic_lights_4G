#ifndef __GIT__H
#define __GIT__H


//  �豸ʹ�����趨��
#define OLED 1			// �Ƿ�ʹ��OLED
#define NET_SERVE 1		// ƽ̨ѡ��
#define NETWORK_CHAEK 1 // �Ƿ������߼��
#define KEY_OPEN 1		// �Ƿ��������Ͷ̰����
#define USART2_OPEN 0	// �Ƿ�ʹ�ô��ڶ�

// ���ݶ���
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

//  C��
#include "cjson.h"
#include <string.h>
#include <stdio.h>
// ��Ƭ��ͷ�ļ�
#include "sys.h"
#include "usart.h"	 
// ����Э���
#include "Net.h"
// �����豸
#include "usart3.h"
// ������
#include "task.h"
#include "timer.h"
// Ӳ������
#include "delay.h"
#include "usart.h"
#include "git.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "flash.h"
#include "HC_SR04.h"
#include "usart2.h"
#include "GA10.h"
#include "hmc5883l.h"
#include "JR6001.h"

#if OLED // OLED�ļ�����
#include "oled.h"
#endif


// ��������Ϣ
#define SSID "NET"		// ·����SSID����
#define PASS "12345678" // ·��������
#if NET_SERVE == 0
// ��Э���������Onenet�ɰ�֧�֣�
#define ServerIP "183.230.40.39" // ������IP��ַ
#define ServerPort 6002			 // ������IP��ַ�˿ں�
#elif NET_SERVE == 1
// ����������ƽ̨��������֧�֣�
#define ServerIP "iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com" // ������IP��ַ
#define ServerPort 1883											// ������IP��ַ�˿ں�
#elif NET_SERVE == 2
// EMQXƽ̨��������
#define ServerIP "broker.emqx.io" // ������IP��ַ
#define ServerPort 1883			  // ������IP��ַ�˿ں�
#elif NET_SERVE == 3
// ��Ϊ��ƽ̨
#define ServerIP "4a33038a2b.st1.iotda-device.cn-north-4.myhuaweicloud.com" // ������IP��ַ
#define ServerPort 1883			  // ������IP��ַ�˿ں�
#endif
// �豸��Ϣ
#define PROID "h9sjWYKPnKA"															 // ��ƷID
#define DEVID "smartdevice" // �豸ID
#define AUTH_INFO "b90496a86ca3283fab89fe9cc0f0b432"						 // ��Ȩ��Ϣ
// MQTT���� /broadcast/
#define S_TOPIC_NAME "/broadcast/h9sjWYKPnKA/test1" // ��Ҫ���ĵ�����
#define P_TOPIC_NAME "/broadcast/h9sjWYKPnKA/test2" // ��Ҫ����������

#define P_TOPIC_CMD "/sys/h9sjWYKPnKA/smartdevice/thing/event/property/post"

// �Զ��岼������
typedef enum
{
	MY_TRUE,
	MY_FALSE
} myBool;

// �Զ���ִ�н������
typedef enum
{
	MY_SUCCESSFUL = 0x01, // �ɹ�
	MY_FAIL = 0x00		  // ʧ��

} mySta; // �ɹ���־λ

typedef enum
{
	OPEN = 0x01, // ��
	CLOSE = 0x00 // �ر�

} On_or_Off_TypeDef; // �ɹ���־λ

typedef enum
{
	DERVICE_SEND = 0x00, // �豸->ƽ̨
	PLATFORM_SEND = 0x01 // ƽ̨->�豸

} Send_directino; // ���ͷ���

typedef struct
{
	U8 App;			 // ָ��ģʽ
	U8 Time;
	U8 Device_State; // ģʽ
	U8 Page;		 // ҳ��
	U8 Error_Time;
	U8 time_cut_page; // ҳ��

	F32 temperatuer; // �¶�
	F32 humiditr;	 // ʪ��
	U8 Flage;		 // ģʽѡ��
	U16 car;		 // ��
	U16 light;		 // ����
	U16 rain;		 // ���

} Data_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	U16 somg_value; // ������ֵ
	U16 humi_value; // ʪ����ֵ
	U16 temp_value; // �¶���ֵ
	U16 Distance_value; // ������ֵ
	
} Threshold_Value_TypeDef; // ���ݲ����ṹ��

typedef struct
{
	U8 check_device; // ״̬
	U8 open;	 
	U8 Key_State;	 // �������

	U8 light_V;
	U8 light_V_Copy;
	U8 light_R_Timr;
	U8 light_G_Timr;
	U8 Update_Time; // ����ʱ��
	U8 car_state; 
	
} Device_Satte_Typedef; // ״̬�����ṹ��


// ȫ������
extern Data_TypeDef Data_init;
extern Device_Satte_Typedef device_state_init; // �豸״̬

extern Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data);
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state);
// ����OLED��ʾ��������
mySta Update_oled_massage(void);
// �����豸״̬
mySta Update_device_massage(void);
// ����json����
mySta massage_parse_json(char *message);
// ����
void Check_Key_ON_OFF(void);
// ��ʱ
void Automation_Close(void);

#endif
