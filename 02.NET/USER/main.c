#include "git.h"


// 软件定时器设定
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;
extern u8 time25ms;
u8 Connect_Net;
// 获取全局变量
const char *topics[] = {S_TOPIC_NAME};

// 硬件初始化
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    HZ = GB16_NUM();                                // 字数
    delay_init();                                   // 延时函数初始化
    GENERAL_TIM_Init(TIM_4, 0, 1);
    
    OLED_ColorTurn(0);   // 0正常显示，1 反色显示
    OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示
#endif
	
#if OLED // OLED文件存在
    OLED_Clear();
#endif
}
// 网络初始化
void Net_Init()
{

#if OLED // OLED文件存在
    char str[50];
    OLED_Clear();
    // 写OLED内容
		sprintf(str, "---------------");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "---Net Break---");
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "---------------");
		OLED_ShowCH(0, 4, (unsigned char *)str);
#endif
    GA10_Init(); // 初始化GPRS模块

    Connect_Net = 60; // 入网成功
#if OLED              // OLED文件存在
    OLED_Clear();
#endif
}

// 任务1
void task1(void)
{
	//1秒计算器
 	Automation_Close();

	 
    Update_device_massage(); // 更新设备
                             // BEEP= ~BEEP;
    State = ~State;
}
// 任务3
void task3(void)
{

	if (Data_init.App == 0) {
		Data_init.App = 1;
	}
}
// 软件初始化
void SoftWare_Init(void)
{
    // 定时器初始化
    timer_init(&task1_id, task1, 1000, 1); // 1s执行一次
    timer_init(&task2_id, task2, 400, 1);  // 400ms执行一次
    timer_init(&task3_id, task3, 3500, 1); // 3.5s执行一次
	
}
// 主函数
int main(void)
{

    char *dataPtr = NULL;
    SoftWare_Init(); // 软件初始化
    Hardware_Init(); // 硬件初始化

    Net_Init(); // 网络初始
    TIM_Cmd(TIM4, ENABLE); // 使能计数器
    while (1) {

        // 线程
        timer_loop(); // 定时器执行
        // 串口接收判断
        dataPtr = (char*)ESP8266_GetIPD(0);
        if (dataPtr != NULL) {
            massage_ga10_json(dataPtr); // 接收命令
        }
#if KEY_OPEN
			 
#endif
    }
}
