#ifndef __BOARD_IMAGE_DEAL_H
#define __BOARD_IMAGE_DEAL_H

#include "include.h"

#define IMAGE_MID_COL 80
#define IMAGE_MID_ROW 40

extern uint8 Search_Start_Row;
extern uint8 Board_Left_Edge_Col[80];
extern uint8 Board_Left_Edge_Row[80];
extern uint8 Board_Right_Edge_Col[80];
extern uint8 Board_Right_Edge_Row[80];
extern uint8 Board_Right_Edge[40];
extern uint8 Board_Left_Edge[40];
extern uint8 Board_Center_Line[40];
extern void board_init_deal();
extern void ns_search();
extern void Board_Edge_Search();
extern uint8 ns_left_judge(uint8 row,uint8 col);
extern uint8 ns_right_judge(uint8 row,uint8 col);
#endif