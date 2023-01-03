#include "wy_headfile.h"


u16 ItemNum = 0;	  //菜单项的数目
u16 ParaNum = 0;   //参量型菜单项的数目
u16 OrderNum = 0;  //指令型菜单项的数目
item Item[ITEM_MAX];  //菜单项结构体数组
u16 CurItem = 1;   //当前菜单项的ID
u16 CurMenu = MENU;//当前菜单的ID
u8 Keyflag = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief            创建菜单项
//  @param            type: 菜单项类型   name：菜单项的名字   min：数据的最小值   max: 数据的最大值
//  @attention        非数据类型的菜单项min = 0, max = 1
//  @return           item：菜单项
//  Sample usage      CreatItem(paraF_item, "angle_kp", 0, 10)
//-------------------------------------------------------------------------------------------------------------------
item CreatItem(itemtype type, char* name, int min, int max)
{
	item Item;
	Item.item_type = type;
	strncpy(Item.item_name, name, NAME_MAX); 
	Item.data_max = max;
	Item.data_min = min;
	Item.root_ID = 0;
	Item.child_ID = 0;
	Item.list_ID = 0;
	Item.list_num = 0;
	Item.para_ID = 0;
	Item.order_ID = 0;
	Item.ID = ItemNum++;                          //MENU项的ID是0
	if(type == order_item)                        //指令型菜单项
	{
		Item.order_ID = ++OrderNum;                 // 指令菜单项数加一
	}
	else 
	{
	  if(type == paraI_item || type == paraF_item)//参量型菜单项
	  {
		  Item.para_ID = ++ParaNum;                 //指令菜单项数加一
		  Item.item_data = CAR[ParaNum];            //将小车的数据给参量型菜单项（对参量型菜单项数据进行初始化）
	  }
  }
	return Item;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief         插入菜单项
//  @param         Item：菜单项结构体数组名   root：被插入的菜单的ID（插到谁里面）  item_t: 插入的菜单项的ID（把谁插入） 
//  @attention     调用此函数前先设置ID，把大写的名字放入枚举ITEMID里，如BANLANCE = 2，以大写名字作ID     
//  @return        void
//  Sample usage   InsertItem(Item, BALANCE, ANGLE_SET) 
//-------------------------------------------------------------------------------------------------------------------
void InsertItem(item* Item, u16 root, u16 item_t)
{
	if(Item[root].child_ID == 0)
	{
		Item[root].child_ID = Item[item_t].ID;
	}
	Item[item_t].root_ID = Item[root].ID;
	Item[item_t].list_ID = ++Item[root].list_num;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief        创建菜单
//  @param        
//  @attention    菜单项创建顺序决定其ID顺序，要求使创建顺序与其在枚举中的顺序一致，保证其大写的名字表示其ID
//  @return       void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void CreatMenu(void)//**创建并插入一个新项需要增加一个新ITEMID枚举项***
{
	Item[MENU] = CreatItem(list_item, "MENU", 0, 1);
	Item[MODE] = CreatItem(paraI_item, "Mode", 0, 5);
	Item[POSITION] = CreatItem(list_item, "Position", 0, 1);
	Item[BALANCE] = CreatItem(list_item, "Balance", 0, 1);
	Item[SPEED] = CreatItem(list_item, "Speed", 0, 1);
	Item[DIR] = CreatItem(list_item, "Dir", 0, 1);
	Item[HOLD] = CreatItem(order_item, "Hold", 0, 1);//保存菜单项
	InsertItem(Item, MENU, MODE);
	InsertItem(Item, MENU, POSITION);
	InsertItem(Item, MENU, BALANCE);
	InsertItem(Item, MENU, SPEED);
	InsertItem(Item, MENU, DIR);
	InsertItem(Item, MENU, HOLD);
	Item[GYRO_X] = CreatItem(stateF_item, "gyro_x", 0, 1);
	Item[GYRO_Y] = CreatItem(stateF_item, "gyro_y", 0, 1);
	Item[GYRO_Z] = CreatItem(stateF_item, "gyro_z", 0, 1);
	Item[ACC_X] = CreatItem(stateF_item, "acc_x", 0, 1);
	Item[ACC_Y] = CreatItem(stateF_item, "acc_y", 0, 1);
	Item[ACC_Z] = CreatItem(stateF_item, "acc_z", 0, 1);
	InsertItem(Item, POSITION, GYRO_X);
	InsertItem(Item, POSITION, GYRO_Y);
	InsertItem(Item, POSITION, GYRO_Z);
	InsertItem(Item, POSITION, ACC_X);
	InsertItem(Item, POSITION, ACC_Y);
	InsertItem(Item, POSITION, ACC_Z);
	Item[ANGLE_SET] = CreatItem(paraF_item, "angle_set", -180, 180);
	Item[STEER_MID] = CreatItem(paraI_item, "steer_mid", 0, 4500);
	Item[ANGLE_CUR] = CreatItem(stateF_item, "angle_cur", -90, 90);
	Item[ANGLE_KP] = CreatItem(paraF_item, "angle_kp", -200, 200);
	Item[ANGLE_KI] = CreatItem(paraF_item, "angle_ki", -100, 100);
	Item[ANGLE_KD] = CreatItem(paraF_item, "angle_kd", -200, 200);
	Item[ANGLE_OUT] = CreatItem(stateF_item, "angle_out", -600, 600);
	Item[ANGLE_PROP] = CreatItem(paraF_item, "angle_prop", -100, 100);
	InsertItem(Item, BALANCE, ANGLE_SET);
	InsertItem(Item, BALANCE, STEER_MID);
	InsertItem(Item, BALANCE, ANGLE_CUR);
	InsertItem(Item, BALANCE, ANGLE_KP);
	InsertItem(Item, BALANCE, ANGLE_KI);
	InsertItem(Item, BALANCE, ANGLE_KD);
	InsertItem(Item, BALANCE, ANGLE_OUT);
	InsertItem(Item, BALANCE, ANGLE_PROP);
	Item[SPEED_SET] = CreatItem(paraF_item, "speed_set", -10000, 10000);
	Item[SPEED_CUR] = CreatItem(stateF_item, "speed_cur", -1000, 1000);
	Item[SPEED_KP] = CreatItem(paraF_item, "speed_kp", -500, 500);
	Item[SPEED_KI] = CreatItem(paraF_item, "speed_ki", -100, 100);
	Item[SPEED_KD] = CreatItem(paraF_item, "speed_kd", -100, 100);
	Item[SPEED_MOTOR] = CreatItem(stateF_item, "speed_mot", -10000, 10000);
	InsertItem(Item, SPEED, SPEED_SET);
	InsertItem(Item, SPEED, SPEED_CUR);
	InsertItem(Item, SPEED, SPEED_KP);
	InsertItem(Item, SPEED, SPEED_KI);
	InsertItem(Item, SPEED, SPEED_KD);
	InsertItem(Item, SPEED, SPEED_MOTOR);
	Item[KP_MID_L] = CreatItem(paraF_item, "kp_mid_l", -500, 500);
	Item[DIR_MAX_L] = CreatItem(paraF_item, "dir_max_l", -20, 20);
	Item[KP_MID_R] = CreatItem(paraF_item, "kp_mid_r", -500, 500);
	Item[DIR_MAX_R] = CreatItem(paraF_item, "dir_max_r", -20, 20);	
	Item[DIR_ANGLE] = CreatItem(stateF_item, "dir_angle", -200, 200);
	InsertItem(Item, DIR, KP_MID_L);
	InsertItem(Item, DIR, DIR_MAX_L);
	InsertItem(Item, DIR, KP_MID_R);
	InsertItem(Item, DIR, DIR_MAX_R);
	InsertItem(Item, DIR, DIR_ANGLE);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief       打印菜单
//  @param       
//  @attention   依照当前菜单ID(CurMenu)和当前菜单项ID(CurItem)打印菜单       
//  @return      void
//  Sample usage:  
//-------------------------------------------------------------------------------------------------------------------
void PrintMenu(void)
{
	u16 x, y;
	u16 start_site;
	u16 print_ID;
	u16 end_ID;
	char Num_str[10];
	start_site = (Item[CurItem].list_ID <= 3) ? 1 : Item[CurItem].list_ID - 2;		// 菜单中打印开始项的位置
	print_ID = Item[CurMenu].child_ID + (start_site - 1); 							// 打印开始项的位置的ID
	end_ID = Item[CurMenu].child_ID + (Item[CurMenu].list_num - 1);					// 该菜单最后一项的ID
	DataUpdate();																	// 首先更新一下数据
	OLED_Clear();                                            						// 清屏
	OLED_ShowString(0, 0, Item[CurMenu].item_name, 12);
	OLED_ShowString(0, 10, "---------------------", 12);
	x = 6;
	y = 2;	//菜单项的开始显示位置
	while(y <= 4 && print_ID <= end_ID)  // 
 {
	 OLED_ShowString(x, 12*y, Item[print_ID].item_name, 12);	//打印菜单项名字
	 if(Item[print_ID].item_type != list_item && Item[print_ID].item_type != order_item)//打印数据菜单项数据
	 {
		 if(Item[print_ID].item_type == stateI_item || Item[print_ID].item_type == paraI_item)  //打印整型数据
		 {
			sprintf((char*)Num_str, "%5d", Item[print_ID].item_data.intData); // （u16) 打印到(char)。
			OLED_ShowString(88, 12*y, Num_str, 12);
		 }
		 else
		 {
			sprintf((char*)Num_str, "%.2f", Item[print_ID].item_data.floatData); // （u16) 打印到(char)。
			OLED_ShowString(88, 12*y, Num_str, 12);
		 }
	 }
	 print_ID++;
	 y++;
 }
	OLED_ShowString(0, (Item[CurItem].list_ID + 1 <= 4) ? 12*(Item[CurItem].list_ID + 1) : 12*4, ">", 12);	// 打印光标
 	OLED_Refresh_Gram();	// OLED更新
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief        检测按键
//  @param     
//  @attention    同时按下左右键作ok使用，目前ok仅用于保存数据，每隔150ms检测一次按键   
//  @return       key
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
key GetKey(void)
{	
	key key_oper = undo;
	if(KEY_L == 0 && KEY_R == 0)
	{
		Keyflag = 1;
	}
	else if (KEY_U == 0 && Keyflag == 0)
	{
		Keyflag = 2;
	}
	else if (KEY_D == 0 && Keyflag == 0)
	{
		Keyflag = 3;
	}
	else if (KEY_L == 0 && Keyflag == 0)
	{
		Keyflag = 4;
	}
	else if (KEY_R == 0 && Keyflag == 0)
	{
		Keyflag = 5;
	}
	if(KEY_L == 1 && KEY_R == 1 && Keyflag == 1)
	{
		key_oper = ok;
		Keyflag = 0;
	}
	else if(KEY_U == 1 && Keyflag == 2)
	{
		key_oper = up;
		Keyflag = 0;
	}
	else if (KEY_D == 1 && Keyflag == 3)
	{
		key_oper = down;
		Keyflag = 0;
	}
	else if (KEY_L == 1 && Keyflag == 4)
	{
		key_oper = left;
		Keyflag = 0;
	}
	else if (KEY_R == 1 && Keyflag == 5)
	{
		key_oper = right;
		Keyflag = 0;
	}
	return key_oper;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief        菜单界面的按键操作
//  @param     
//  @attention    上下键进行上下移动光标并修改当前菜单项ID （CurItem），左键返回上一级菜单、右键进入子菜单并修改当前菜单ID（CurMenu)和当前菜单项ID（CurItem）
//                ****在当前菜单项（CurItem）是参量类型菜单项时，右键进入参量项数据修改界面****
//                ****在当前菜单项（CurItem）是Hold菜单项时，右键保存数据****
//  @return       void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void KeyOperation(key Key)
{
	switch (Key)
	{
		case up:
			if(Item[CurItem].list_ID > 1)
			{
				CurItem--;
			}
			break;
		case down:
			if(Item[CurItem].list_ID < Item[CurMenu].list_num)
			{
				CurItem++;
			}
			break;
		case left:
			if(CurMenu != MENU)
			{
				CurMenu = Item[CurMenu].root_ID;
				CurItem = Item[CurItem].root_ID;
			}		  
		  break;
		case right:
			if(Item[CurItem].item_type == list_item)
			{
				CurMenu = CurItem;
				CurItem = Item[CurItem].child_ID;				
			}
			else if(CurItem == HOLD)
			{
				LED = 0;
				delay_ms(500);
				LED = 1;
				STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&CAR[0].intData, PARA_MAX * sizeof(cardata));// 把参量数据存入单片机寄存器
			}
			else if(Item[CurItem].item_type == paraI_item || Item[CurItem].item_type == paraF_item)//进入修改参量界面
			{
				ModifyParaData();
			}
			break;
		default:
			break;
	}
	if (Key != undo)
		PrintMenu();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief       菜单初始化, 给状态菜单项赋初值
//  @param     
//  @param     
//  @attention   这是一个传值函数，只赋了初值，菜单项数据不会实时变动，使用DataUpdata()函数更新菜单项数据
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void MenuInit(void)
{
	Item[GYRO_X].item_data.floatData = gyrox; 
	Item[GYRO_Y].item_data.floatData = gyroy;
	Item[GYRO_Z].item_data.floatData = gyroz;
	Item[ACC_X].item_data.floatData = aacx;
	Item[ACC_Y].item_data.floatData = aacy;
	Item[ACC_Z].item_data.floatData = aacz;
	Item[ANGLE_CUR].item_data.floatData = angle_cur;
	Item[ANGLE_OUT].item_data.floatData = Balance_pidoutput;
	Item[SPEED_CUR].item_data.floatData = speed_cur;
	Item[SPEED_MOTOR].item_data.floatData = speed_motor;
	Item[DIR_ANGLE].item_data.floatData = Dir_bias;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief        进入参量菜单项界面修改参量数据
//  @param     
//  @attention         
//  @return       item
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void ModifyParaData(void)
{
	key key_t;       																					  //按键
	u16 pos = 0;   																					//改数光标
	u16 i;        																					  //循环使用
	int Data;       																					//去掉小数点 123.45->12345
	u16 digit;                                               //数据位数
	u16 dataMax;                                             //数据的最大极限值
	char dataArray[10] = {0}; 
	digit = (Item[CurItem].item_type == paraI_item) ? 0 : 2;    //浮点型数据小数一律为两位
	dataMax = Item[CurItem].data_max;
	while(dataMax != 0)                                         //获得最大数位
    {
        dataMax /= 10;
        digit++;
    }
	if(Item[CurItem].item_type == paraF_item)
	{ 
		 Data = (int)(Item[CurItem].item_data.floatData * 100); //123.45->12345浮点型数据X100去小数位方便修改
	}
	else
	{
		 Data = (int)Item[CurItem].item_data.intData;
	}
	if(Data < 0)                                                //数组首位存符号
	{
		Data = -Data;
		dataArray[0] = '-';
	}
	else
	{
		dataArray[0] = '+';
	}
	for(i = digit; i >= 1; i--)                                 //将数据逐位存进字符数组
	{
		dataArray[i] = '0' + Data % 10;
		Data = Data / 10;
	}
	PrintParaData(dataArray, pos);                              //打印参量数据
	while(1)                                                    //参量界面下的按键操作
	{
		key_t = KEY_Scan(0);                
		if(key_t != undo)
		{
			if(pos == 0 && key_t == left)                           //不保存修改，退出
			{
				PrintMenu();
				break; 
			}
			else if(pos == digit && key_t == right)
			{
				LED = 0;
				delay_ms(500);
				LED = 1;
				ArrayToData(dataArray, digit);
				PrintMenu();
				break;
			}
				pos = paraKeyOper(dataArray, key_t, pos, digit);        //移动光标或修改数据
	  }
	}
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief        打印参量菜单项界面
//  @param     
//  @attention    浮点型参量在数据尾部打印e-2表示最后两位数是小数
//  @return       void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void PrintParaData(char* DataArray, u16 Pos)
{
	OLED_Clear();                                            //清屏
	OLED_ShowString(0, 0, Item[CurItem].item_name, 12);
	OLED_ShowString(0, 12, "----------------------", 12);
	OLED_ShowString(0, 24, DataArray, 12); 
	if(Item[CurItem].item_type == paraF_item)                    //浮点型参量在数据尾部打印e-2表示最后两位数是小数
	OLED_ShowString(66, 24, "e-2", 12); 
	OLED_ShowString(Pos * 6, 36, "*", 12);
	OLED_Refresh_Gram();	// OLED更新
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief        参量菜单项界面的按键操作
//  @param        Digit: 数据的位数（由该数据的最大值决定）
//  @attention    移动光标或修改数据     
//  @return       光标位置pos
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
u16 paraKeyOper(char* DataArray, key Key, u16 Pos, u16 Digit)
{
	int j;//循环使用
	switch (Key)
	{
		case up:
			if(Pos == 0)
			{
				if(DataArray[0] == '-')                        //修改数据的符号
					DataArray[0] = '+';
				else if(DataArray[0] == '+')
					DataArray[0] = '-';
				PrintParaData(DataArray, Pos);
			}
			else if(Pos <= Digit)
			{
				if(DataArray[Pos] < '9')
				DataArray[Pos]++;
				else if(DataArray[Pos] == '9')                      //进位
				{
					j = 0;
					while(DataArray[Pos-j] == '9' && Pos-j > 0)  //数位是‘9’则变为‘0’，往前找
					{
						DataArray[Pos-j] = '0';
						j++;
					}
          if(Pos-j != 0)					
					DataArray[Pos-j]++;                          //直到数位不为‘9’，则该数位加一
				}
        PrintParaData(DataArray, Pos);				
			}
			break;
		case down:
			if(Pos == 0)
			{
				if(DataArray[0] == '-')
					DataArray[0] = '+';
				else if(DataArray[0] == '+')
					DataArray[0] = '-';
				PrintParaData(DataArray, Pos);
			}
			else if(Pos <= Digit)
			{
				if(DataArray[Pos] > '0')
				DataArray[Pos]--;
				else if(DataArray[Pos] == '0')                       //退位
				{
					j = 0;
					while(DataArray[Pos-j] == '0' && Pos-j > 0)   //数位是‘0’则变为‘9’，往前找
					{
						DataArray[Pos-j] = '9';
						j++;
					}
          if(Pos-j != 0)					
					DataArray[Pos-j]--;                           //直到数位不为‘0’，则该数位减一
				}
        PrintParaData(DataArray, Pos);				
			}
			break;
		case left:
			if(Pos > 0)
			{
				Pos--;
				PrintParaData(DataArray, Pos);
			}		
		  	break;
		case right:
			if(Pos < Digit)
			{
				Pos++;
				PrintParaData(DataArray, Pos);
			}			
			break;
		default:
			break;
	}
	return Pos;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief         重新打印菜单时对数据进行更新
//  @param     
//  @attention     此函数放在打印菜单PrintMenu()函数里，在打印菜单前更新一下菜单项数据以及把修改的参量型菜单项数据传给小车数据和实际参量数据
//  @return       
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void DataUpdate(void)
{	
	mode = Item[MODE].item_data.intData;
	angle_set = Item[ANGLE_SET].item_data.floatData;
	steer_mid = Item[STEER_MID].item_data.intData;
	Balance_pid.kp = Item[ANGLE_KP].item_data.floatData;
	Balance_pid.ki = Item[ANGLE_KI].item_data.floatData;
	Balance_pid.kd = Item[ANGLE_KD].item_data.floatData;
	angle_prop = Item[ANGLE_PROP].item_data.floatData;
	speed_set = Item[SPEED_SET].item_data.floatData;
	Speed_pid.kp = Item[SPEED_KP].item_data.floatData;
	Speed_pid.ki = Item[SPEED_KI].item_data.floatData;
	Speed_pid.kd = Item[SPEED_KD].item_data.floatData;
	kp_mid_l = Item[KP_MID_L].item_data.floatData;
	dir_max_l = Item[DIR_MAX_L].item_data.floatData;
	kp_mid_r = Item[KP_MID_R].item_data.floatData;
	dir_max_r = Item[DIR_MAX_R].item_data.floatData;	
	
	CAR[1]= Item[MODE].item_data;       //注意顺序，由参量型菜单项插入菜单顺序决定
	CAR[2]= Item[ANGLE_SET].item_data; 
  CAR[3]= Item[STEER_MID].item_data;
	CAR[4]= Item[ANGLE_KP].item_data;
	CAR[5]= Item[ANGLE_KI].item_data;
	CAR[6]= Item[ANGLE_KD].item_data;
	CAR[7]= Item[ANGLE_PROP].item_data;
	CAR[8]= Item[SPEED_SET].item_data;
	CAR[9]= Item[SPEED_KP].item_data;
	CAR[10]= Item[SPEED_KI].item_data;
	CAR[11]= Item[SPEED_KD].item_data;	
	CAR[12]= Item[KP_MID_L].item_data;
	CAR[13]= Item[DIR_MAX_L].item_data;
	CAR[14]= Item[KP_MID_R].item_data;
	CAR[15]= Item[DIR_MAX_R].item_data;	
	Item[GYRO_X].item_data.floatData = gyrox; 
	Item[GYRO_Y].item_data.floatData = gyroy;
	Item[GYRO_Z].item_data.floatData = gyroz;
	Item[ACC_X].item_data.floatData = aacx;
	Item[ACC_Y].item_data.floatData = aacy;
	Item[ACC_Z].item_data.floatData = aacz;
	Item[ANGLE_CUR].item_data.floatData = angle_cur;
	Item[ANGLE_OUT].item_data.floatData = Balance_pidoutput;
	Item[SPEED_CUR].item_data.floatData = speed_cur;
	Item[SPEED_MOTOR].item_data.floatData = speed_motor;
	Item[DIR_ANGLE].item_data.floatData = Dir_bias;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief         将字符数组转化为数据值，保存修改
//  @param     
//  @attention     字符数组里没有存进小数点，若是浮点型参量数据则要先除以100再转化     
//  @return     
//  Sample usage:   
//-------------------------------------------------------------------------------------------------------------------
void ArrayToData(char* DataArray, u16 Digit)
{
	u16 i;
	int Data = 0;
	for(i = 1; i <= Digit; i++)
	{
		Data = Data * 10;		
		Data += (DataArray[i] - '0');
	}
	if(DataArray[0] == '-')
		Data = -Data;
	if(Item[CurItem].item_type == paraF_item && (((float)Data) / 100 >= Item[CurItem].data_min) && (((float)Data) / 100 <= Item[CurItem].data_max))  // 若为浮点型菜单项，把data除以100再转化
		{
			Item[CurItem].item_data.floatData = ((float)Data) / 100;
		}
	else if(Item[CurItem].item_type == paraI_item && (Data >= Item[CurItem].data_min) && (Data <= Item[CurItem].data_max)) 
		Item[CurItem].item_data.intData = Data;
}





