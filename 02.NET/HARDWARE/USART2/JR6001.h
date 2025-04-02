#ifndef __JR6001_H
#define __JR6001_H

#include "sys.h"

// ��æʹ��
#define JR6001_BUSY 0 // 0.ʧ�� 1.ʹ��

// JR6001��æ�������� �ߵ�ƽæ
#define JR6001_BUSY_GPIO_CLK RCC_APB2Periph_GPIOA
#define JR6001_BUSY_GPIO_PORT GPIOA
#define JR6001_BUSY_GPIO_PIN GPIO_Pin_1
#define JR6001_BUSY_GPIO_Mode GPIO_Mode_IPD

#define JR6001_BUSY_IO GPIO_ReadInputDataBit(JR6001_BUSY_GPIO_PORT, JR6001_BUSY_GPIO_PIN) // ��ȡ��æ����

// JR6001��������[���� ��ͣ ��һ�� ��һ�� ������ ������]
#define Play "A2"
#define Suspend "A3"
#define On_Song "A5"
#define Next_Song "A6"
#define Volumeadd "B0"
#define Volumesub "B1"

void JR6001_Init(void);
// ���ڷ�������
void JR6001_SendCode(u8 *str, u8 busy);
// ����ָ���
void JR6001_Instruction(u8 *str, u8 busy);
// ��Ŀѡ��
void JR6001_SongControl(u8 num, u8 busy);
// ����ѡ��
void JR6001_Volumecontrol(u8 num);
#endif
