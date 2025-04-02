#include "git.h"

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
extern HMC_XYZ hmc_xyz_init; // ��ʼ���ṹ��

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{

	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{
	
	// ��
	R_Test();
	// ״̬����
	device_state->Update_Time = 3;

	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];

	sprintf(str, "������: %03d    ", Data_init.car );
	OLED_ShowCH(16, 0, (unsigned char *)str);
	sprintf(str, "��ǰʣ��ʱ��   ");
	OLED_ShowCH(16, 2, (unsigned char *)str);
	sprintf(str, " %02d S ", device_state_init.Update_Time);
	OLED_ShowCH(45, 5, (unsigned char *)str);
//	sprintf(str, " %05d", hmc_xyz_init.HMC_X);
//	OLED_ShowCH(30, 6, (unsigned char *)str);
#endif

	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{
	// �Զ�ģʽ
	HMC5883_Get_Angle();// ������
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
			// ������Ϣ
			Mqtt_Pub(1);
			break;
		case 2:
	
			break;
		}
		Data_init.App = 0;
	}


	return MY_SUCCESSFUL;
}

// ��ʱ��
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
				JR6001_SongControl(2, 0); // ��������
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
					JR6001_SongControl(1, 0); // ��������
					device_state_init.Update_Time = device_state_init.light_R_Timr;
					break;
				default:
					break;
				}
			}
		}
	}
	

}
// ��ⰴ���Ƿ���
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
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
	else if (key == 0 && num_on >= Key_Scan_Time) // ����
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
