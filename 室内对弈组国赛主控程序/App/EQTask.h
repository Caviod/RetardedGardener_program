# ifndef _EQTASK_H_
# define _EQTASK_H_

//顺序改过了
typedef enum
{
    Forward,
    Back,
    Left,
    Right,
    Stop,
    HoldChess,
    DownChess,
    Forward_Left,
    SpecialStop,
    SpecialForward,
    YuandiXuanzhuan_You,
    NoneCMD
}_EQ_RunCMD;//一个任务的运行指令枚举

typedef struct
{
    _EQ_RunCMD TaskCommand;//该任务的运行指令
    int TaskPara;//该任务的运行参数，与TaskCommand配合使用就相当于代表了前进2格这种指令
    int TaskProcess;//该任务的进度值，当该任务的进度值=TaskPara时任务完成
    unsigned char IsFinish;//该任务是否完成
}_EQ_Task;//一个任务的结构体

void RenewTaskProcess(_EQ_Task * NowTask);
void CreateTaskPool();
void DoTask();
extern void ConfigTask(_EQ_Task * NowTask, _EQ_RunCMD CMD, int Para);
extern _EQ_Task EQ_TaskPool[60];//任务池

extern void DoTaskEx(uint8 action);
# endif