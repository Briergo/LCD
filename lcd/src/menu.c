#include "menu.h"
#include "pid.h"
#include <stdlib.h>
#include "TFT_8080.h"
#include "MenuThread.h"
#include <stdio.h>

GPTDriver *timer4 = &GPTD4;

/*Vspomogat peremenn*/
node_t* current=NULL;
int count=0;
char dataled=0xFF;
unsigned char  up_flag_ext=0;
unsigned char  down_flag_ext=0;
unsigned char up_flag=0;
unsigned char  down_flag=0;
unsigned char  left_flag=0;
unsigned char  right_flag=0;
unsigned char  left_flag_ext=0;
unsigned char  right_flag_ext=0;

/*Create glavnoe menu*/ 
node_t* menu1=NULL;
node_t* menu2=NULL;
node_t* menu3=NULL;
/*Create podmenu menu*/
node_t* menu1_1=NULL;
node_t* menu1_2=NULL;
node_t* menu1_3=NULL;
node_t* menu1_4=NULL;
node_t* menu2_1=NULL;
node_t* menu2_2=NULL;
node_t* menu3_1=NULL;
node_t* menu3_2=NULL;
node_t* menu3_3=NULL;

GPTConfig gpt4_conf = {
    .frequency = 50000,
    .callback = cbgptfun4,
    .cr2 = 0,
    .dier = 0
};









void Menu_GPIO_Init(void)
{
  //запуск таймера 4
  gptStart(timer4, &gpt4_conf);

  uint8_t arg = 5;
  // А ногу с кнопкой на вход
  palSetPadMode(UP_GPIO_Port, UP_Pin, PAL_MODE_INPUT_PULLDOWN);
  // Разрешаем прерывание, указываем callbcak функцию и передаем указатель на аргумент
  palEnablePadEvent(UP_GPIO_Port, UP_Pin, PAL_EVENT_MODE_RISING_EDGE);
  palSetPadCallback(UP_GPIO_Port, UP_Pin, up_button, &arg);

  palSetPadMode(DOWN_GPIO_Port, DOWN_Pin, PAL_MODE_INPUT_PULLDOWN);
  palEnablePadEvent(DOWN_GPIO_Port, DOWN_Pin, PAL_EVENT_MODE_RISING_EDGE);
  palSetPadCallback(DOWN_GPIO_Port, DOWN_Pin, down_button, &arg);

  palSetPadMode(LEFT_GPIO_Port, LEFT_Pin, PAL_MODE_INPUT_PULLDOWN);
  palEnablePadEvent(LEFT_GPIO_Port, LEFT_Pin, PAL_EVENT_MODE_RISING_EDGE);
  palSetPadCallback(LEFT_GPIO_Port, LEFT_Pin, left_button, &arg);

  palSetPadMode(RIGHT_GPIO_Port, RIGHT_Pin, PAL_MODE_INPUT_PULLDOWN);
  palEnablePadEvent(RIGHT_GPIO_Port, RIGHT_Pin, PAL_EVENT_MODE_RISING_EDGE);
  palSetPadCallback(RIGHT_GPIO_Port, RIGHT_Pin, right_button, &arg);
}
void Menu_Create(void)
{

   /* MENU 1*/
  menu1=malloc(sizeof(node_t));
  menu1->prev=NULL;
  menu1->parent=NULL;
  menu1->text="Motor Control";
  menu1->starttext="Main Menu";
  menu1->next=malloc(sizeof(node_t));
  menu1->child=malloc(sizeof(node_t));
  menu1->x_pos=60;
  menu1->y_pos=70;
  menu1->bool_value=NULL;
  menu1->menu_value=-1;
  menu1->cmd=0;
  // MENU 1_1
  menu1_1=menu1->child;
  menu1_1->parent=menu1;
  menu1_1->text="Motor Start";
  menu1_1->starttext="Menu Settings";
  menu1_1->child=NULL;
  menu1_1->prev=NULL;
  menu1_1->next=malloc(sizeof(node_t));
  menu1_1->cmd=START;
  menu1_1->x_pos=10;
  menu1_1->y_pos=50;
  menu1_1->bool_value="FALSE";
  menu1_1->menu_value=-1;
  //MENU 1_2
  menu1_2=menu1_1->next;
  menu1_2->parent=menu1;
  menu1_2->text="Motor Stop";
  menu1_2->starttext=NULL;
  menu1_2->child=NULL;
  menu1_2->prev=menu1_1;
  menu1_2->cmd=STOP;
  menu1_2->next=malloc(sizeof(node_t));
  menu1_2->x_pos=10;
  menu1_2->y_pos=90;
  menu1_2->bool_value="FALSE";
  menu1_2->menu_value=-1;
  //MENU 1_3
  menu1_3=menu1_2->next;
  menu1_3->parent=menu1;
  menu1_3->text="Motor Forward";
  menu1_3->starttext=NULL;
  menu1_3->child=NULL;
  menu1_3->prev=menu1_2;
  menu1_3->cmd=FORWARD;
  menu1_3->next=malloc(sizeof(node_t));;
  menu1_3->x_pos=10;
  menu1_3->y_pos=130;
  menu1_3->bool_value="FALSE";
  menu1_3->menu_value=-1;
  //MENU 1_4
  menu1_4=menu1_3->next;
  menu1_4->parent=menu1;
  menu1_4->text="Motor Back";
  menu1_4->starttext=NULL;
  menu1_4->child=NULL;
  menu1_4->prev=menu1_3;
  menu1_4->cmd=BACK;
  menu1_4->next=NULL;
  menu1_4->x_pos=10;
  menu1_4->y_pos=170;
  menu1_4->bool_value="FALSE";
  menu1_4->menu_value=-1;
  // MENU 2
  menu2 = menu1->next; 
  menu2->child=malloc(sizeof(node_t));
  menu2->parent=NULL;
  menu2->text="Motor Speed";
  menu2->starttext=NULL;
  menu2->prev=menu1;
  menu2->next=malloc(sizeof(node_t));
  menu2->x_pos=60;
  menu2->y_pos=110;
  menu2->cmd=0;
  menu2->bool_value=NULL;
  menu2->menu_value=-1;
  // MENU 2_1
  menu2_1=menu2->child;
  menu2_1->next=NULL;
  menu2_1->starttext="Motor Speed";
  menu2_1->parent=menu2;
  menu2_1->child=NULL;
  menu2_1->prev=NULL;
  menu2_1->text="Set Speed";
  menu2_1->x_pos=10;
  menu2_1->y_pos=70;
  menu2_1->cmd=SPEED;
  menu2_1->bool_value=NULL;
  menu2_1->menu_value=set_speed;
  //MENU 3
  menu3=menu2->next;
  menu3->child=malloc(sizeof(node_t));
  menu3->parent=NULL;
  menu3->text="PID";
  menu3->starttext=NULL;
  menu3->prev=menu2;
  menu3->next=NULL;
  menu3->x_pos=60;
  menu3->y_pos=150;
  menu3->cmd=0;
  menu3->bool_value=NULL;
  menu3->menu_value=-1;
  // MENU 3_1

  menu3_1=menu3->child;
  menu3_1->next=malloc(sizeof(node_t));
  menu3_1->starttext="Regulator coeff.";
  menu3_1->parent=menu3;
  menu3_1->child=NULL;
  menu3_1->prev=NULL;
  menu3_1->text="P";
  menu3_1->x_pos=10;
  menu3_1->y_pos=70;
  menu3_1->cmd=SPEED;
  menu3_1->bool_value=NULL;
  menu3_1->menu_value=Reg1.P;
  // MENU 3_2
  menu3_2=menu3_1->next;
  menu3_2->next=malloc(sizeof(node_t));
  menu3_2->parent=menu1;
  menu3_2->prev=menu3_1;
  menu3_2->child=NULL;
  menu3_2->text="D";
  menu3_2->x_pos=10;
  menu3_2->y_pos=110;
  menu3_2->cmd=SPEED;
  menu3_2->bool_value=NULL;
  menu3_2->menu_value=Reg1.D;
  // MENU 3_3
  menu3_3=menu3_2->next;
  menu3_3->next=NULL;
  menu3_3->parent=menu3;
  menu3_3->prev=menu3_2;
  menu3_3->child=NULL;
  menu3_3->text="I";
  menu3_3->x_pos=10;
  menu3_3->y_pos=150;
  menu3_3->cmd=SPEED;
  menu3_3->bool_value=NULL;
  menu3_3->menu_value=Reg1.I;

  //Menu init
  current=menu1;
  Menu_Button();
  Menu_Disp();
  Cursor();


}

void Menu_Button(void)
{
  TFT_Fill_Screen(0,320,0,240,WHITE);
  TFT_Draw_Horizontal_Line(0,201,320,BLACK);
  TFT_Draw_Horizontal_Line(0,202,320,BLACK);
  TFT_Draw_Triangle(20,220,40,210,40,230,BLACK);
  TFT_Draw_String(42,220, BLACK, WHITE, "BACK", 1);
  TFT_Draw_Triangle(100,210,90,230,110,230,BLACK);
  TFT_Draw_String(113,220, BLACK, WHITE, "PREV", 1);
  TFT_Draw_Triangle(160,210,180,210,170,230,BLACK);
  TFT_Draw_String(183,220, BLACK, WHITE, "NEXT", 1);
  TFT_Draw_Triangle(230,210,230,230,250,220,BLACK);
  TFT_Draw_String(253,220, BLACK, WHITE, "FARTHER", 1);
}

void Menu_Disp(void)
{
  TFT_Fill_Screen(0,320,0,200,WHITE);
  char String[20];
  node_t* i;
  i=current;
  while(i->prev!=NULL)
        i=i->prev;
  if(i->starttext!=NULL)
  {
    TFT_Draw_String(10,10, BLACK, WHITE, i->starttext, 2);
    TFT_Draw_Horizontal_Line(0,30,320,BLACK);
    TFT_Draw_Horizontal_Line(0,31,320,BLACK);
  }
  while(i!=NULL)
  {
      if(i->bool_value!=NULL)
      {
        TFT_Draw_String(i->x_pos+220,i->y_pos, BLUE, WHITE, i->bool_value, 1);
      }
      if(i->menu_value!=-1)
      {
        sprintf(String,"%d",i->menu_value);
        TFT_Draw_String(i->x_pos+150,i->y_pos, BLUE, WHITE,String , 2);
      }
      TFT_Draw_String(i->x_pos,i->y_pos, BLACK, WHITE, i->text, 2);
      i=i->next;
  }  
}

void Cursor(void)
{
    TFT_Draw_String(current->x_pos,current->y_pos, RED, WHITE, current->text, 2);
}

// callback функция, которая должна сработать по настроенному событию
void up_button(void* args)
{
    // Преобразование аргумента к требуемому типу, в данному случае к uint8_t
    uint8_t arg = *((uint8_t*) args);
    // Запрет прерываний
    palDisablePadEventI(UP_GPIO_Port, UP_Pin);
    chMBPostI(&menu_mb, UP);
    up_flag_ext=1;
    //запуск в непрерывном режиме с периодом 0.5 с
    gptStartContinuousI(timer4, 25000);
}

// callback функция, которая должна сработать по настроенному событию
void down_button(void* args)
{
    // Преобразование аргумента к требуемому типу, в данному случае к uint8_t
    uint8_t arg = *((uint8_t*) args);
    // Запрет прерываний
    palDisablePadEventI(DOWN_GPIO_Port, DOWN_Pin);
    chMBPostI(&menu_mb, DOWN);
    down_flag_ext=1;
    //запуск в непрерывном режиме с периодом 0.5 с
    gptStartContinuousI(timer4, 25000);
}

// callback функция, которая должна сработать по настроенному событию
void right_button(void* args)
{
    // Преобразование аргумента к требуемому типу, в данному случае к uint8_t
    uint8_t arg = *((uint8_t*) args);
    // Запрет прерываний
    palDisablePadEventI(RIGHT_GPIO_Port, RIGHT_Pin);
    chMBPostI(&menu_mb, RIGHT);
    right_flag_ext=1;
    //запуск в непрерывном режиме с периодом 0.5 с
    gptStartContinuousI(timer4, 25000);
}

// callback функция, которая должна сработать по настроенному событию
void left_button(void* args)
{
    // Преобразование аргумента к требуемому типу, в данному случае к uint8_t
    uint8_t arg = *((uint8_t*) args);
    // Запрет прерываний
    palDisablePadEventI(LEFT_GPIO_Port, LEFT_Pin);
      chMBPostI(&menu_mb, LEFT);
    left_flag_ext=1;
    //запуск в непрерывном режиме с периодом 0.5 с
    gptStartContinuousI(timer4, 25000);
}

// callback функция таймера 4
void cbgptfun4(GPTDriver *gptp)
{
    (void)gptp;
    uint8_t arg = 5;
    if(right_flag_ext)
    {
      palEnablePadEventI(RIGHT_GPIO_Port, RIGHT_Pin, PAL_EVENT_MODE_RISING_EDGE);
      palSetPadCallbackI(RIGHT_GPIO_Port, RIGHT_Pin, right_button, &arg);
      gptStopTimerI(timer4);
      right_flag_ext=0;
    }
    if(left_flag_ext)
    {
      palEnablePadEventI(LEFT_GPIO_Port, LEFT_Pin, PAL_EVENT_MODE_RISING_EDGE);
      palSetPadCallbackI(LEFT_GPIO_Port, LEFT_Pin, left_button, &arg);
      gptStopTimerI(timer4);
      left_flag_ext=0;
    }
    if(up_flag_ext)
    {
      palEnablePadEventI(UP_GPIO_Port, UP_Pin, PAL_EVENT_MODE_RISING_EDGE);
      palSetPadCallbackI(UP_GPIO_Port, UP_Pin, up_button, &arg);
      gptStopTimerI(timer4);
      up_flag_ext=0;
    }
    if(down_flag_ext)
    {
      palEnablePadEventI(DOWN_GPIO_Port, DOWN_Pin, PAL_EVENT_MODE_RISING_EDGE);
      palSetPadCallbackI(DOWN_GPIO_Port, DOWN_Pin, down_button, &arg);
      gptStopTimerI(timer4);
      down_flag_ext=0;
    }
}


