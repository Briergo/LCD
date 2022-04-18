#ifndef _MENU_H_
#define _MENU_H_

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/




/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hal.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define UP_Pin 10u
#define UP_GPIO_Port GPIOF
#define LEFT_Pin 0u
#define LEFT_GPIO_Port GPIOC
#define DOWN_Pin 3u
#define DOWN_GPIO_Port GPIOF
#define RIGHT_Pin 5u
#define RIGHT_GPIO_Port GPIOF


extern unsigned char up_flag;
extern unsigned char  down_flag;
extern unsigned char  left_flag;
extern unsigned char  right_flag;


typedef struct node 
{
  char* text;
  char cmd;
  struct node* prev;
  struct node* next;
  struct node* parent;
  struct node* child;
  unsigned int x_pos;
  unsigned int y_pos;
}
node_t;

extern node_t* current;
extern GPTDriver *timer4;


/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

void Menu_Create(void);
void Menu_Disp(void);
void Cursor(void);
void up_button(void* args);
void down_button(void* args);
void left_button(void* args);
void right_button(void* args);
void Menu_GPIO_Init(void);
void cbgptfun4(GPTDriver *gptp);

/* USER CODE END PFP */

#endif 
