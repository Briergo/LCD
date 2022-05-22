/*
 * MenuThread.c
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */

#include "ch.h"
#include "menu.h"
#include "pid.h"
#include "Motor.h"
#include "MenuThread.h"
#include "TFT_8080.h"
#include <stdio.h>


extern node_t* current;
extern node_t* menu1_1;
extern node_t* menu1_2;
extern node_t* menu1_3;
extern node_t* menu1_4;
extern node_t* menu3_1;
extern node_t* menu3_2;
extern node_t* menu3_3;
extern node_t* menu2_1;

mailbox_t menu_mb;
msg_t menu_mb_buffer[BUFFER_SIZE];

mailbox_t cmd_mb;
msg_t cmd_mb_buffer[BUFFER_SIZE];

mailbox_t holl_mb;
msg_t holl_mb_buffer[BUFFER_SIZE];

mutex_t mtx;

uint8_t flagmenuspeed=0;

static THD_WORKING_AREA(menuThread, 1024);// 256 - stack size

static THD_FUNCTION(menu, arg)
{
    char Value[20];
    arg=arg;
    msg_t my_msg;
    int8_t button=0;
    bool mutex;
    while(1)
    {
      msg_t msg = chMBFetchTimeout(&menu_mb, &my_msg, chTimeMS2I(200));
      if (msg == MSG_OK)
          {
            button=(int8_t)my_msg;
            switch(button)
            {
              case UP:
              {
                if(flagmenu2)
                {
                  mutex=chMtxTryLock(&mtx);
                  if(mutex==true)
                  {
                    flagmenu2=0;
                    Menu_Disp();
                    Cursor();
                    chMtxUnlock(&mtx);
                  }
                }
                else
                if(current->prev!=NULL)
                {
                  current=current->prev;
                  Menu_Disp();
                  Cursor();
                }
              }
              break;
              case DOWN:
              {
                if(flagmenu2)
                {
                  mutex=chMtxTryLock(&mtx);
                  if(mutex==true)
                  {
                    flagmenu2=0;
                    Menu_Disp();
                    Cursor();
                    chMtxUnlock(&mtx);
                  }
                }
                else
                {
                  if(current->next!=NULL)
                  {
                    current=current->next;
                    Menu_Disp();
                    Cursor();
                  }
                }
              }
              break;
              case RIGHT:
              {
                if(flagmenu2)
                {
                  mutex=chMtxTryLock(&mtx);
                  if(mutex==true)
                  {
                    if(current->menu_value<=200)
                    current->menu_value+=5;
                    sprintf(Value,"%d",current->menu_value);
                    TFT_Fill_Screen(current->x_pos+145,current->x_pos+200,current->y_pos,current->y_pos+20,WHITE);
                    TFT_Draw_String(current->x_pos+150,current->y_pos, GREEN, WHITE,Value , 2);
                    chMtxUnlock(&mtx);
                    Reg1.P=menu3_1->menu_value;
                    Reg1.D=menu3_2->menu_value;
                    Reg1.I=menu3_3->menu_value;
                    set_speed=menu2_1->menu_value;
                  }
                }
                else
                {
                  if(current->child!=NULL)
                  {
                    mutex=chMtxTryLock(&mtx);
                    if(mutex==true)
                    {
                      current=current->child;
                      Menu_Disp();
                      Cursor();
                      if(current==menu2_1)
                        flagmenuspeed=1;
                      chMtxUnlock(&mtx);
                    }
                  }
                  else if((current->cmd!=0)&&(current->child==NULL))
                  {
                    chMBPostI(&cmd_mb, current->cmd);
                  }
                }
              }
              break;
              case LEFT:
              {
                if(flagmenu2)
                {
                  mutex=chMtxTryLock(&mtx);
                  if(mutex==true)
                  {
                    if(current->menu_value>0)
                      current->menu_value-=5;
                    sprintf(Value,"%d",current->menu_value);
                    TFT_Fill_Screen(current->x_pos+145,current->x_pos+200,current->y_pos,current->y_pos+20,WHITE);
                    TFT_Draw_String(current->x_pos+150,current->y_pos, GREEN, WHITE,Value , 2);
                    chMtxUnlock(&mtx);
                    Reg1.P=menu3_1->menu_value;
                    Reg1.D=menu3_2->menu_value;
                    Reg1.I=menu3_3->menu_value;
                    set_speed=menu2_1->menu_value;
                  }
                }
                else
                {
                if(current->parent!=NULL)
                {
                  mutex=chMtxTryLock(&mtx);
                  if(mutex==true)
                  {
                    flagmenuspeed=0;
                    current=current->parent;
                    Menu_Disp();
                    Cursor();
                    chMtxUnlock(&mtx);
                  }
                }
               }
              }
              break;
            }
          }
    }
}


static THD_WORKING_AREA(cmdThread, 256);// 256 - stack size

static THD_FUNCTION(cmd, arg)
{
    char Value[20];
    arg=arg;
    msg_t my_msg;
    int8_t cmd=0;
    while(1)
        {
          msg_t msg = chMBFetchTimeout(&cmd_mb, &my_msg, chTimeMS2I(300));
          if (msg == MSG_OK)
          {
            cmd=(int8_t)my_msg;
            switch(cmd)
            {
              case START:
              {
                Motor_Forward();
                Reg1.Summ_Error=0;
                menu1_1->bool_value="TRUE";
                menu1_2->bool_value="FALSE";
                menu1_3->bool_value="TRUE";
                TFT_Fill_Screen(menu1_1->x_pos+220,menu1_1->x_pos+270,menu1_1->y_pos,menu1_1->y_pos+20,WHITE);
                TFT_Draw_String(menu1_1->x_pos+220,menu1_1->y_pos, BLUE, WHITE, menu1_1->bool_value, 1);
                TFT_Fill_Screen(menu1_2->x_pos+220,menu1_2->x_pos+270,menu1_2->y_pos,menu1_2->y_pos+20,WHITE);
                TFT_Draw_String(menu1_2->x_pos+220,menu1_2->y_pos, BLUE, WHITE, menu1_2->bool_value, 1);
                TFT_Fill_Screen(menu1_3->x_pos+220,menu1_3->x_pos+270,menu1_3->y_pos,menu1_3->y_pos+20,WHITE);
                TFT_Draw_String(menu1_3->x_pos+220,menu1_3->y_pos, BLUE, WHITE, menu1_3->bool_value, 1);
              }
              break;
              case STOP:
              {
                Motor_Stop();
                Reg1.Summ_Error=0;
                menu1_1->bool_value="FALSE";
                menu1_2->bool_value="TRUE";
                menu1_3->bool_value="FALSE";
                menu1_4->bool_value="FALSE";
                TFT_Fill_Screen(menu1_1->x_pos+220,menu1_1->x_pos+270,menu1_1->y_pos,menu1_1->y_pos+20,WHITE);
                TFT_Draw_String(menu1_1->x_pos+220,menu1_1->y_pos, BLUE, WHITE, menu1_1->bool_value, 1);
                TFT_Fill_Screen(menu1_2->x_pos+220,menu1_2->x_pos+270,menu1_2->y_pos,menu1_2->y_pos+20,WHITE);
                TFT_Draw_String(menu1_2->x_pos+220,menu1_2->y_pos, BLUE, WHITE, menu1_2->bool_value, 1);
                TFT_Fill_Screen(menu1_3->x_pos+220,menu1_3->x_pos+270,menu1_3->y_pos,menu1_3->y_pos+20,WHITE);
                TFT_Draw_String(menu1_3->x_pos+220,menu1_3->y_pos, BLUE, WHITE, menu1_3->bool_value, 1);
                TFT_Fill_Screen(menu1_4->x_pos+220,menu1_4->x_pos+270,menu1_4->y_pos,menu1_4->y_pos+20,WHITE);
                TFT_Draw_String(menu1_4->x_pos+220,menu1_4->y_pos, BLUE, WHITE, menu1_4->bool_value, 1);
              }
              break;
              case FORWARD:
              {
                Motor_Forward();
                Reg1.Summ_Error=0;
                menu1_1->bool_value="TRUE";
                menu1_3->bool_value="TRUE";
                menu1_4->bool_value="FALSE";
                TFT_Fill_Screen(menu1_1->x_pos+220,menu1_1->x_pos+270,menu1_1->y_pos,menu1_1->y_pos+20,WHITE);
                TFT_Draw_String(menu1_1->x_pos+220,menu1_1->y_pos, BLUE, WHITE, menu1_1->bool_value, 1);
                TFT_Fill_Screen(menu1_3->x_pos+220,menu1_3->x_pos+270,menu1_3->y_pos,menu1_3->y_pos+20,WHITE);
                TFT_Draw_String(menu1_3->x_pos+220,menu1_3->y_pos, BLUE, WHITE, menu1_3->bool_value, 1);
                TFT_Fill_Screen(menu1_4->x_pos+220,menu1_4->x_pos+270,menu1_4->y_pos,menu1_4->y_pos+20,WHITE);
                TFT_Draw_String(menu1_4->x_pos+220,menu1_4->y_pos, BLUE, WHITE, menu1_4->bool_value, 1);
              }
              break;
              case BACK:
              {
                Motor_Back();
                Reg1.Summ_Error=0;
                menu1_1->bool_value="TRUE";
                menu1_3->bool_value="FALSE";
                menu1_4->bool_value="TRUE";
                TFT_Fill_Screen(menu1_1->x_pos+220,menu1_1->x_pos+270,menu1_1->y_pos,menu1_1->y_pos+20,WHITE);
                TFT_Draw_String(menu1_1->x_pos+220,menu1_1->y_pos, BLUE, WHITE, menu1_1->bool_value, 1);
                TFT_Fill_Screen(menu1_3->x_pos+220,menu1_3->x_pos+270,menu1_3->y_pos,menu1_3->y_pos+20,WHITE);
                TFT_Draw_String(menu1_3->x_pos+220,menu1_3->y_pos, BLUE, WHITE, menu1_3->bool_value, 1);
                TFT_Fill_Screen(menu1_4->x_pos+220,menu1_4->x_pos+270,menu1_4->y_pos,menu1_4->y_pos+20,WHITE);
                TFT_Draw_String(menu1_4->x_pos+220,menu1_4->y_pos, BLUE, WHITE, menu1_4->bool_value, 1);
              }
              break;
              case SPEED:
              {
                bool mutex=chMtxTryLock(&mtx);
                if(mutex==true)
                {
                  flagmenu2=1;
                  sprintf(Value,"%d",current->menu_value);
                  TFT_Draw_String(current->x_pos+150,current->y_pos, GREEN, WHITE,Value , 2);
                  chMtxUnlock(&mtx);
                }
              }
              break;

            }
          }
        }
}

static THD_WORKING_AREA(speed_menuThread, 512);// 256 - stack size

static THD_FUNCTION(speed_menu, arg)
{
  char Speed_Value[20];
  arg=arg;
  msg_t my_msg;
  while(1)
  {
    msg_t msg = chMBFetchTimeout(&holl_mb, &my_msg, chTimeMS2I(300));
    if (msg == MSG_OK)
    {
      if(flagmenuspeed)
      {
        bool mutex=chMtxTryLock(&mtx);
        if(mutex==true)
        {
          sprintf(Speed_Value,"%d",(int16_t)my_msg);
          TFT_Fill_Screen(230,290,110,130,WHITE);
          TFT_Draw_String(230,110,RED,WHITE,Speed_Value,2);
          TFT_Draw_String(10,110,RED,WHITE,"Current Speed",2);
          chMtxUnlock(&mtx);
        }
      }
    }
  }
}


void Menu_Thread_Start(void)
{
  //настройка дисплея и инициализация меню
  TFT_Init();
  Menu_Create();
  Menu_GPIO_Init();
  chMBObjectInit(&menu_mb, menu_mb_buffer, BUFFER_SIZE);
  chMBObjectInit(&cmd_mb, cmd_mb_buffer, BUFFER_SIZE);
  chThdCreateStatic(menuThread, sizeof(menuThread), NORMALPRIO-1, menu, NULL);
  chThdCreateStatic(cmdThread, sizeof(cmdThread), NORMALPRIO-1, cmd, NULL);
  chThdCreateStatic(speed_menuThread, sizeof(speed_menuThread), NORMALPRIO-2, speed_menu, NULL);
  chMtxObjectInit(&mtx);
}

