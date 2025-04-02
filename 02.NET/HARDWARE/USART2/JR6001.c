#include "JR6001.h"

#include "usart.h"
#include "delay.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
// ��ʱ�����ض��� JR6001��ʱ�������壬�û����и���Ϊ�Լ�����ʱ����
#define JR6001_ms(ms) delay_ms(ms)
#define JR6001_us(us) delay_us(us)

void JR6001_Volumecontrol(u8 num);

void JR6001_Init(void)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;

#if JR6001_BUSY // ��æ��������

	RCC_APB2PeriphClockCmd(JR6001_BUSY_GPIO_CLK, ENABLE); // ʹ��PORʱ��
	GPIO_InitStructure.GPIO_Pin = JR6001_BUSY_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = JR6001_BUSY_GPIO_Mode;
	GPIO_Init(JR6001_BUSY_GPIO_PORT, &GPIO_InitStructure);

#endif
	// �ѹر�
			JR6001_SongControl(2,0);
	JR6001_Volumecontrol(31); // ��������
	JR6001_ms(100);
	JR6001_Volumecontrol(31); // ��������
	JR6001_ms(100);
	JR6001_Volumecontrol(31); // ��������
	JR6001_ms(100); 
}

// ���ڷ�������
void JR6001_SendCode(u8 *str, u8 busy)
{
	u16 len, i;
	len = strlen((char *)str);

	if (busy == 1 && JR6001_BUSY_IO == 0)
	{
		printf("%s\n", str);
		for (i = 0; i <= len; i++) // ѭ����������
		{
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
				; // ѭ������,ֱ���������
			USART_SendData(USART2, str[i]);
		}
	}
	else if (busy == 0)
	{
		printf("%s\n", str);
		for (i = 0; i <= len; i++) // ѭ����������
		{
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
				; // ѭ������,ֱ���������
			USART_SendData(USART2, str[i]);
		}
	}
	JR6001_ms(10);
}
// ����ָ���
void JR6001_Instruction(u8 *str, u8 busy)
{
	u8 JR6001_Buff[20];
	sprintf((char *)JR6001_Buff, "%s\r\n", str);
	JR6001_SendCode(JR6001_Buff, busy);
}
// ��Ŀѡ��
void JR6001_SongControl(u8 num, u8 busy)
{
	u8 JR6001_Buff[20];
	if (num < 10)
	{
		sprintf((char *)JR6001_Buff, "A7:0000%d\r\n", num);
		JR6001_SendCode(JR6001_Buff, busy);
	}
	else if (num < 100)
	{
		sprintf((char *)JR6001_Buff, "A7:000%d\r\n", num);
		JR6001_SendCode(JR6001_Buff, busy);
	}
	else
	{
		sprintf((char *)JR6001_Buff, "A7:00%d\r\n", num);
		JR6001_SendCode(JR6001_Buff, busy);
	}
}
// ����ѡ��
void JR6001_Volumecontrol(u8 num)
{
	u8 JR6001_Buff[20];
	sprintf((char *)JR6001_Buff, "AF:%d\r\n", num);
	JR6001_SendCode(JR6001_Buff, 0);
}

/*
	JR6001_Init();
	JR6001_SongControl(5,0);

*/
