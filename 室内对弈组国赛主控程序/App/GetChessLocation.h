#ifndef __GetChessLocation_h
#define __GetChessLocation_h

#include "include.h"

#define GetLocaction_Uart_Port UART0

extern void GetChess_Location();
extern void GetChessLocation_Init();
extern float Gz,Vx,Vy;
extern int16 gz;
extern float Distance;
#endif