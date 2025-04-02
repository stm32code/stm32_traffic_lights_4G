#include "delay.h"
#include "led.h"


// ��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
// LED IO��ʼ��
void LED_Init(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	// ʹ��PB3��PB4��Ҫ�ر�JTAN����
	// GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	/*����LED��ص�GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd(State_GPIO_CLK | LED1_GPIO_CLK, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = State_GPIO_PIN;
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(State_GPIO_PORT, &GPIO_InitStructure);

	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;

	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	/*���ÿ⺯������ʼ��GPIOF*/
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;
	/*���ÿ⺯������ʼ��GPIOF*/
	GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;
	/*���ÿ⺯������ʼ��GPIOF*/
	GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
	/*���ÿ⺯������ʼ��GPIOF*/
	GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure);

	/* �ر�����led��	*/
	GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
	/* �ر�����led��	*/
	GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
	/* �ر�����led��	*/
	GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

/*********************************************************************************
 * @Function	:	ϵͳ����PB3��PB4Ϊ��ͨIO��
 * @Input		:	deviceSta,�豸״̬
 * @Output		: 	None
 * @Return		: 	None
 * @Others		: 	JTAG���Է�ʽ����Ӱ��
 * @Date			:  2020-04-22
 **********************************************************************************/
void System_PB34_setIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
