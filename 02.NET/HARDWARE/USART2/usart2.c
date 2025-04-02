#include "delay.h"
#include "usart2.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

char rxdatabufer;
u16 point1 = 0;

_SaveData Save_Data;

#if EN_USART2_RX // ���ʹ���˽���
// ����1�жϷ������
// ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
char USART_RX2_BUF[USART2_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.
// ����״̬
// bit15��	������ɱ�־
// bit14��	���յ�0x0d
// bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA = 0; // ����״̬���

void Usart2_Init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ����3ʱ��ʹ��

	USART_DeInit(USART2);					  // ��λ����3
											  // USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			// ��ʼ��PB10

	// USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // ��ʼ��PB11

	USART_InitStructure.USART_BaudRate = bound;										// ������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); // ��ʼ������	3

	USART_Cmd(USART2, ENABLE); // ʹ�ܴ���

	// ʹ�ܽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // �����ж�

	// �����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ���

	CLR_Buf(); // ��ջ���
}

void USART2_IRQHandler(void) // ����1�жϷ������
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC // ���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART2); //(USART1->DR);	//��ȡ���յ�������

		if (Res == '$')
		{
			point1 = 0;
		}

		USART_RX2_BUF[point1++] = Res;

		if (USART_RX2_BUF[0] == '$' && USART_RX2_BUF[4] == 'M' && USART_RX2_BUF[5] == 'C') // ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
		{
			if (Res == '\n')
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);	 // ���
				memcpy(Save_Data.GPS_Buffer, USART_RX2_BUF, point1); // ��������
				Save_Data.isGetData = true;
				point1 = 0;
				memset(USART_RX2_BUF, 0, USART2_REC_LEN); // ���
			}
		}

		if (point1 >= USART2_REC_LEN)
		{
			point1 = USART2_REC_LEN;
		}
	}
#ifdef OS_TICKS_PER_SEC // ���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();
#endif
}

u8 Hand(char *a) // ��������ʶ����
{
	if (strstr(USART_RX2_BUF, a) != NULL)
		return 1;
	else
		return 0;
}

void CLR_Buf(void) // ���ڻ�������
{
	memset(USART_RX2_BUF, 0, USART2_REC_LEN); // ���
	point1 = 0;
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length); // ���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
}

#endif
