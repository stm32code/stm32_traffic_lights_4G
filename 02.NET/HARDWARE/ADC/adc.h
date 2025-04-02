#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

#define SMOG_PIN46_R 1000 // ���������ܽ�4��6�ӳ����صĵ���ֵ

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define ADC_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADC_CLK RCC_APB2Periph_ADC1

#define ADC_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK RCC_APB2Periph_GPIOA
#define ADC_PORT GPIOA

// ת��ͨ������
#define NOFCHANEL 2

#define ADC_PIN1 GPIO_Pin_0
#define ADC_CHANNEL1 ADC_Channel_0

#define ADC_PIN2 GPIO_Pin_1
#define ADC_CHANNEL2 ADC_Channel_1

// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define ADCx ADC1
#define ADC_DMA_CHANNEL DMA1_Channel1
#define ADC_DMA_CLK RCC_AHBPeriph_DMA1

/**************************��������********************************/
void ADCx_Init(void);
u16 ADC2_value(void);
u16 ADC1_value(void);
#endif /* __ADC_H */
