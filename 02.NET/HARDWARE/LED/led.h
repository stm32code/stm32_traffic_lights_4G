#ifndef __LED_H
#define __LED_H
#include "sys.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define State_GPIO_PORT GPIOC               /* GPIO端口 */
#define State_GPIO_CLK RCC_APB2Periph_GPIOC /* GPIO端口时钟 */
#define State_GPIO_PIN GPIO_Pin_13
/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
// R-红色
#define LED1_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED1_GPIO_PIN GPIO_Pin_4           /* 连接到SCL时钟线的GPIO */

// G-绿色
#define LED2_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED2_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED2_GPIO_PIN GPIO_Pin_5           /* 连接到SCL时钟线的GPIO */

// B-蓝色
#define LED3_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED3_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED3_GPIO_PIN GPIO_Pin_6           /* 连接到SCL时钟线的GPIO */

// R-红色
#define LED4_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED4_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED4_GPIO_PIN GPIO_Pin_12          /* 连接到SCL时钟线的GPIO */

// G-绿色
#define LED5_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED5_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED5_GPIO_PIN GPIO_Pin_13          /* 连接到SCL时钟线的GPIO */

// B-蓝色
#define LED6_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED6_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED6_GPIO_PIN GPIO_Pin_14          /* 连接到SCL时钟线的GPIO */

#define State PCout(13)

/** the macro definition to trigger the led on or off
 * 1 - off
 *0 - on
 */
#define ON 0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define LED1(a)                                  \
  if (a)                                         \
    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); \
  else                                           \
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN)

#define LED2(a)                                  \
  if (a)                                         \
    GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN); \
  else                                           \
    GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN)

#define LED3(a)                                  \
  if (a)                                         \
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN); \
  else                                           \
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN)

#define Relay(a)                                   \
  if (a)                                           \
    GPIO_SetBits(State_GPIO_PORT, State_GPIO_PIN); \
  else                                             \
    GPIO_ResetBits(State_GPIO_PORT, State_GPIO_PIN)

/* 定义控制IO的宏 */
#define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF digitalLo(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_ON digitalHi(LED1_GPIO_PORT, LED1_GPIO_PIN)

#define LED2_TOGGLE digitalToggle(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF digitalLo(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_ON digitalHi(LED2_GPIO_PORT, LED2_GPIO_PIN)

#define LED3_TOGGLE digitalToggle(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_OFF digitalLo(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_ON digitalHi(LED3_GPIO_PORT, LED3_GPIO_PIN)

#define LED4_TOGGLE digitalToggle(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_OFF digitalLo(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_ON digitalHi(LED4_GPIO_PORT, LED4_GPIO_PIN)

#define LED5_TOGGLE digitalToggle(LED5_GPIO_PORT, LED5_GPIO_PIN)
#define LED5_OFF digitalLo(LED5_GPIO_PORT, LED5_GPIO_PIN)
#define LED5_ON digitalHi(LED5_GPIO_PORT, LED5_GPIO_PIN)

#define LED6_TOGGLE digitalToggle(LED6_GPIO_PORT, LED6_GPIO_PIN)
#define LED6_OFF digitalLo(LED6_GPIO_PORT, LED6_GPIO_PIN)
#define LED6_ON digitalHi(LED6_GPIO_PORT, LED6_GPIO_PIN)

#define State_TOGGLE digitalToggle(State_GPIO_PORT, State_GPIO_PIN)
#define State_OFF digitalLo(State_GPIO_PORT, State_GPIO_PIN)
#define State_ON digitalHi(State_GPIO_PORT, State_GPIO_PIN)

#define Read_State GPIO_ReadInputDataBit(State_GPIO_PORT, State_GPIO_PIN)
#define LED1_State GPIO_ReadInputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED2_State GPIO_ReadInputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED3_State GPIO_ReadInputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED4_State GPIO_ReadInputDataBit(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED5_State GPIO_ReadInputDataBit(LED5_GPIO_PORT, LED5_GPIO_PIN)
#define LED6_State GPIO_ReadInputDataBit(LED6_GPIO_PORT, LED6_GPIO_PIN)

// #define LED PCout(13)	// LED接口
#define OFF 1
#define ON 0

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

// 红
#define LED_RED \
  LED1_ON;      \
  LED2_OFF      \
  LED3_OFF      \
  LED4_OFF      \
  LED5_OFF      \
  LED6_ON
// 绿
#define LED_GREEN \
  LED1_OFF;       \
  LED2_OFF        \
  LED3_ON         \
  LED4_ON         \
  LED5_OFF        \
  LED6_OFF

// 黄
#define LED_YELLOW \
  LED1_OFF;        \
  LED2_ON          \
  LED3_OFF         \
  LED4_OFF         \
  LED5_ON          \
  LED6_OFF

#define LED_ALL_OFF \
  LED1_OFF;         \
  LED2_OFF          \
  LED3_OFF          \
  LED4_OFF          \
  LED5_OFF          \
  LED6_OFF

// 初始化
void LED_Init(void);
/*********************************************************************************
 * @Function	:	系统设置PB3和PB4为普通IO口
 * @Input		:	deviceSta,设备状态
 * @Output		: 	None
 * @Return		: 	None
 * @Others		: 	JTAG调试方式会受影响
 * @Date			:  2020-04-22
 **********************************************************************************/
void System_PB34_setIO(void);
#endif
