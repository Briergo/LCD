/*
 * MenuThread.c
 *
 *  Created on: 18 апр. 2022 г.
 *      Author: Vania
 */

#include "ch.h"
#include "menu.h"
#include "MenuThread.h"
#include "TFT_8080.h"


mailbox_t menu_mb;
msg_t menu_mb_buffer[BUFFER_SIZE];

static THD_WORKING_AREA(menuThread, 1024);// 256 - stack size

static THD_FUNCTION(menu, arg)
{
    arg=arg;
    msg_t my_msg;
    int8_t button=0;
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
                if(current->next!=NULL)
                {
                  current=current->next;
                  Menu_Disp();
                  Cursor();
                }
              }
              break;
              case RIGHT:
              {
                if(current->child!=NULL)
                {
                  current=current->child;
                  Menu_Disp();
                  Cursor();
                }
              }
              break;
              case LEFT:
              {
                if(current->parent!=NULL)
                {
                  current=current->parent;
                  Menu_Disp();
                  Cursor();
                }
              }
              break;
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
  chThdCreateStatic(menuThread, sizeof(menuThread), NORMALPRIO-1, menu, NULL);
}

