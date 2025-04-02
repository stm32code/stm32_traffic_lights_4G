#include "git.h"

Data_TypeDef Data_init;						  // 设备数据结构体
Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
Device_Satte_Typedef device_state_init;		  // 设备状态
extern HMC_XYZ hmc_xyz_init; // 初始化结构体

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data)
{

	return MY_SUCCESSFUL;
}
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{
	
	// 读
	R_Test();
	// 状态重置
	device_state->Update_Time = 3;

	return MY_SUCCESSFUL;
}
// 更新OLED显示屏中内容
mySta Update_oled_massage()
{
#if OLED // 是否打开
	char str[50];

	sprintf(str, "车流量: %03d    ", Data_init.car );
	OLED_ShowCH(16, 0, (unsigned char *)str);
	sprintf(str, "当前剩余时间   ");
	OLED_ShowCH(16, 2, (unsigned char *)str);
	sprintf(str, " %02d S ", device_state_init.Update_Time);
	OLED_ShowCH(45, 5, (unsigned char *)str);
//	sprintf(str, " %05d", hmc_xyz_init.HMC_X);
//	OLED_ShowCH(30, 6, (unsigned char *)str);
#endif

	return MY_SUCCESSFUL;
}

// 更新设备状态
mySta Update_device_massage()
{
	// 自动模式
	HMC5883_Get_Angle();// 读数据
	if(hmc_xyz_init.HMC_X < 0 && device_state_init.car_state == 0){
		Data_init.car ++;
		device_state_init.car_state =1 ;
	}else{
		device_state_init.car_state =0;
	}
		
	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			// 发布消息
			Mqtt_Pub(1);
			break;
		case 2:
	
			break;
		}
		Data_init.App = 0;
	}


	return MY_SUCCESSFUL;
}

// 定时器
void Automation_Close(void)
{
	
	
	if (device_state_init.open)
	{
		if (device_state_init.Update_Time > 0 && LED1_State)
		{
			if (device_state_init.light_V == 1)
			{
				device_state_init.light_V = 0;
			}
			device_state_init.Update_Time--;
		}
		else if (device_state_init.Update_Time > 0 && LED3_State)
		{
			if (device_state_init.light_V == 1)
			{
				device_state_init.light_V = 2;
			}
			device_state_init.Update_Time--;
		}
		else if (device_state_init.Update_Time > 0 && LED2_State)
		{

			device_state_init.Update_Time--;
		}
		else if (device_state_init.Update_Time == 0)
		{
			if (device_state_init.light_V == 0 || device_state_init.light_V == 2)
			{
				JR6001_SongControl(2, 0); // 语音报警
				device_state_init.light_V_Copy = device_state_init.light_V;
				device_state_init.light_V = 1;
				LED_YELLOW;
				device_state_init.Update_Time = 3;
			}
			else if (device_state_init.light_V == 1)
			{
				switch (device_state_init.light_V_Copy)
				{
				case 0:
					LED_GREEN;
					JR6001_SongControl(3, 0);

					device_state_init.Update_Time = device_state_init.light_G_Timr;
					break;
				case 2:
					LED_RED;
					JR6001_SongControl(1, 0); // 语音报警
					device_state_init.Update_Time = device_state_init.light_R_Timr;
					break;
				default:
					break;
				}
			}
		}
	}
	

}
// 检测按键是否按下
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// 与上一次的键值比较 如果不相等，表明有键值的变化，开始计时
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // 时间记录器
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // 短按
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Short\n");
			if (device_state_init.open)
			{
				device_state_init.open = 0;
			}
			else
			{
				device_state_init.open = 1;
			}

			break;
		case KEY2_PRES:
			printf("Key2_Short\n");
			if (Data_init.car > 0)
			{
				Data_init.car = 0;
			}
			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // 长按
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Long\n");

			break;
		case KEY2_PRES:
			printf("Key2_Long\n");

			break;

		default:
			break;
		}
		num_on = 0;
	}
}
