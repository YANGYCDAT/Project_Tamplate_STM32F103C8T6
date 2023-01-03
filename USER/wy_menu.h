#ifndef _WY_MENU_H_
#define _WY_MENU_H_

#include "wy_headfile.h"


#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)



#define NAME_MAX 20//菜单项名字长度的极限
#define ITEM_MAX 45//菜单项数目的极限
#define PARA_MAX 25//参量型菜单项数目的极限
#define MODE_NUM 2 //模式的数目
#define LED PBout(5)    //数据保存成功时亮起0.5s

#define KEY_L PAin(1)
#define KEY_R PEin(4)
#define KEY_U PAin(0)
#define KEY_D PEin(3)
//#define KEY0 PEin(4)	// 读取按键0
//#define KEY1 PEin(3)	// 读取按键1
//#define KEYUP PAin(0)	// 读取按键WK_UP


enum ITEMID//注意和ID一一对应***顺序不能搞错***
{
	MENU = 0,
	MODE = 1, POSITION, BALANCE, SPEED, DIR, HOLD,
	GYRO_X, GYRO_Y, GYRO_Z, ACC_X, ACC_Y, ACC_Z,
	ANGLE_SET, STEER_MID, ANGLE_CUR, ANGLE_KP, ANGLE_KI, ANGLE_KD, ANGLE_OUT, ANGLE_PROP,
	SPEED_SET, SPEED_CUR, SPEED_KP, SPEED_KI, SPEED_KD, SPEED_MOTOR,
	KP_MID_L, DIR_MAX_L, KP_MID_R, DIR_MAX_R, DIR_ANGLE, //BIAS_TOL, BIAS_K,
};
	

enum ITEMTYPE    //菜单项的类型
{
	list_item = 1, //菜单类型（含有子菜单项）
	paraI_item,    //整型参量类型（人为设定，可以修改，如:angle_kp
	stateI_item,   //整型状态量类型（传感器示数，或计算结果，不可修改，如：angle_cur
	paraF_item,    //浮点型参量类型
	stateF_item,   //浮点型状态量类型
	order_item,    //指令类型，如：Hold保存数据
};
typedef enum ITEMTYPE itemtype;


union CARDATA   //存储数据的变量，存储菜单项数据以及小车数据
{
	int  intData;
	float floatData;
};
typedef union CARDATA cardata;


struct ITEM
{
	u16 ID;             //处于所有菜单项中的序号
	u16 root_ID;        //所处菜单在所有菜单项中的序号
	u16 child_ID;       //如果是菜单型菜单项，child_ID表示它的第一个菜单项的ID
	u16 list_ID;        //处于所处菜单中的序号
	u16 para_ID;        //如果是参量型菜单项，则有参量序号，否则为0
	u16 order_ID;       //如果是指令型菜单项，则有指令序号，否则为0
	u16 list_num;       //如果是菜单型菜单项，list_num表示它含有的菜单项数目
	itemtype item_type;    //菜单项类型,枚举型变量
	char item_name[NAME_MAX];  //菜单项的名字
	cardata item_data;     //菜单项的数据，参量型的可修改，状态型的不可修改
	int data_max;        //菜单项数据的最大值
	int data_min;        //菜单项数据的最小值
};
typedef struct ITEM item;


extern u16 ItemNum;	
extern u16 ParaNum;
extern u16 OrderNum;
extern item Item[ITEM_MAX];
extern cardata CAR[PARA_MAX];
extern u16 CurItem;
extern u16 CurMenu;


item CreatItem(itemtype type, char* name, int max, int min);          //创建菜单项
void InsertItem(item* Item, u16 root, u16 item_t);                  //往菜单中插入菜单项
void CreatMenu(void);                                                     //创建菜单（创建菜单项并插入）
void MenuInit(void);                                                      //菜单初始化, 给状态菜单项赋值
void PrintMenu(void);                                                     //打印菜单
key GetKey(void);                                                         //获取按键操作类型
void KeyOperation(key Key);                                               //菜单界面的按键操作
void ModifyParaData(void);                                                //进入参量菜单项界面修改参量数据
void PrintParaData(char* DataArray, u16 Pos);                          //打印参量菜单项界面
u16 paraKeyOper(char* DataArray, key keyoper, u16 Pos, u16 Digit);//参量菜单项界面的按键操作
void DataUpdate(void);                                                    //更新数据（将新状态数据传给状态菜单项，将修改后菜单项的参量传给实际参量）
void ArrayToData(char* DataArray, u16 Digit);                          	//将存放数据的字符数组转化为整型或浮点型数据

#endif
