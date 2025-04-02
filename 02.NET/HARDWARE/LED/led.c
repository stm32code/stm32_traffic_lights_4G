#include "delay.h"
#include "led.h"


// 初始化PB5和PE5为输出口.并使能这两个口的时钟
// LED IO初始化
void LED_Init(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使用PB3和PB4需要关闭JTAN调试
	// GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd(State_GPIO_CLK | LED1_GPIO_CLK, ENABLE);
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = State_GPIO_PIN;
	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	/*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化GPIO*/
	GPIO_Init(State_GPIO_PORT, &GPIO_InitStructure);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;

	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	/*调用库函数，初始化GPIOF*/
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;
	/*调用库函数，初始化GPIOF*/
	GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;
	/*调用库函数，初始化GPIOF*/
	GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
	/*调用库函数，初始化GPIOF*/
	GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure);

	/* 关闭所有led灯	*/
	GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

/*********************************************************************************
 * @Function	:	系统设置PB3和PB4为普通IO口
 * @Input		:	deviceSta,设备状态
 * @Output		: 	None
 * @Return		: 	None
 * @Others		: 	JTAG调试方式会受影响
 * @Date			:  2020-04-22
 **********************************************************************************/
void System_PB34_setIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 打开AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
