#include "git.h"


// �����ʱ���趨
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;
extern u8 time25ms;
u8 Connect_Net;
// ��ȡȫ�ֱ���
const char *topics[] = {S_TOPIC_NAME};

// Ӳ����ʼ��
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    HZ = GB16_NUM();                                // ����
    delay_init();                                   // ��ʱ������ʼ��
    GENERAL_TIM_Init(TIM_4, 0, 1);
    
    OLED_ColorTurn(0);   // 0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0); // 0������ʾ 1 ��Ļ��ת��ʾ
#endif
	
#if OLED // OLED�ļ�����
    OLED_Clear();
#endif
}
// �����ʼ��
void Net_Init()
{

#if OLED // OLED�ļ�����
    char str[50];
    OLED_Clear();
    // дOLED����
		sprintf(str, "---------------");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "---Net Break---");
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "---------------");
		OLED_ShowCH(0, 4, (unsigned char *)str);
#endif
    GA10_Init(); // ��ʼ��GPRSģ��

    Connect_Net = 60; // �����ɹ�
#if OLED              // OLED�ļ�����
    OLED_Clear();
#endif
}

// ����1
void task1(void)
{
	//1�������
 	Automation_Close();

	 
    Update_device_massage(); // �����豸
                             // BEEP= ~BEEP;
    State = ~State;
}
// ����3
void task3(void)
{

	if (Data_init.App == 0) {
		Data_init.App = 1;
	}
}
// �����ʼ��
void SoftWare_Init(void)
{
    // ��ʱ����ʼ��
    timer_init(&task1_id, task1, 1000, 1); // 1sִ��һ��
    timer_init(&task2_id, task2, 400, 1);  // 400msִ��һ��
    timer_init(&task3_id, task3, 3500, 1); // 3.5sִ��һ��
	
}
// ������
int main(void)
{

    char *dataPtr = NULL;
    SoftWare_Init(); // �����ʼ��
    Hardware_Init(); // Ӳ����ʼ��

    Net_Init(); // �����ʼ
    TIM_Cmd(TIM4, ENABLE); // ʹ�ܼ�����
    while (1) {

        // �߳�
        timer_loop(); // ��ʱ��ִ��
        // ���ڽ����ж�
        dataPtr = (char*)ESP8266_GetIPD(0);
        if (dataPtr != NULL) {
            massage_ga10_json(dataPtr); // ��������
        }
#if KEY_OPEN
			 
#endif
    }
}
