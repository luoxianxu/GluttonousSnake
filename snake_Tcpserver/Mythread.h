#ifndef SEND_WORK_H
#define SEND_WORK_H

#include <QObject>
#include <QVector>
#include <QTcpSocket>
#include <QThread>
#include <QTime>

#include "mysocket.h"

class Mythread : public QThread
{
    Q_OBJECT
public:
    explicit Mythread(QObject *parent = nullptr);
    Mythread(qintptr sock,QObject *parent);
    ~Mythread();

    void run() override;
    void triggerSnakeInitSignal(void);

signals:
    void DeleteThreadFromListSignal(void);
    void DeleteSnakeFromListSignal(snake *Snake);
    void TimeOutSendSnakeListSignal(void);
    void TimeOutAddSnakeToListSignal(void);
    void AddSnakeToListSignal(snake *Snake);
    void SnakeInitSignal(int num);                          //触发蛇初始化的信号
    void CreateFoodToServerSignal(void);
    void SendFoodPointSignal(QPoint point);
    void SendSnakeListSignal(QVector<snake *> SnakeList);
public slots:
    void SocketDisconnectSlot(void);
//    void TimeOutSendSnakeListToThreadSlot(void);            //在thread中定时器超时发送存储蛇身链表的槽函数
    void TimeOutAddSnakeToListInThreadSlot(void);                   //定时器超时，添加客户端蛇身信息至list中的槽函数
    void AddSnakeToThreadSlot(void);
    void DeleteSnakeFromListSlot(void);
    void CreateFoodToThreadSlot(void);
    void SendFoodPointToThreadSlot(QPoint point);
    void SendSnakeListToThreadSlot(QVector<snake *> SnakeList);

private:
    qintptr sock;

};

#endif // SEND_WORK_H
