# ifndef _TASK_H_
# define _TASK_H

# define PushingTime_5ms 57
# define PullingTime_5ms 56
# define UsedIndex_Electromagnet 3     //PTA13

typedef enum
{
    HoldChess_Electromagnet_On,
    HoldChess_PushrodPushing,
    HoldChess_PushrodPulling,
    Moving_HaveChess,
    DownChess_PushrodPushing,
    DownChess_Electromagnet_Off,
    DownChess_PushrodPulling,
    Moving_NotHaveChess
}Enum_HoldAndDownChessStatus;

void CountGrid();
extern Enum_HoldAndDownChessStatus HoldAndDownStatus;
extern unsigned char StartHoldChess();
extern unsigned char StartDownChess();
extern unsigned char Check_HoldOrDownFinish();
extern void HoldAndDownForTimer();
extern int GridNum_Front;//前进的格子数
extern int GridNum_Right;
# endif