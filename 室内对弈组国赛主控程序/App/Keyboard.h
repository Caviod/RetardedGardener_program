#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "include.h"

#define R1      PTC1
#define R2      PTC2
#define R3      PTD0
#define R4      PTC7

#define C1      PTC5
#define C2      PTC6
#define C3      PTD1
#define C4      PTD4

extern void key_board_init(void);      //³õÊ¼»¯¾ØÕó¼üÅÌ
extern uint8 key_board_getval(void);





#endif