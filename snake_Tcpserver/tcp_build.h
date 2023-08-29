#ifndef THREAD_SEND_H
#define THREAD_SEND_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <QMetaType>
#include "Mythread.h"
#include "snake.h"


class tcp_build : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcp_build(QObject *parent = nullptr);
    ~tcp_build();

    void client_connect(void);

    QVector<Mythread *> ThreadList;

    void incomingConnection(qintptr handle) override;
    void triggerSendFoodPointToThreadSignal(QPoint point);      //触发发送食物坐标的信号函数



    int ClientNum;
    QVector<snake *> SnakeList;
    QPoint FoodPoint;

signals:
//    void ClientConnectSignal(void);
    void SnakeInitSignal(int num);
    void SendFoodPointToThreadSignal(QPoint point);             //发送食物坐标的信号
    void SendSnakeListToThreadSignal(QVector<snake *> snakelist);

public slots:
//    void timeoutSolt(void);
    void AddSnakeToListSlot(snake *Snake);
    void DeleteSnakeFromListSlot(snake *Snake);
    void DeleteThreadFromListSlot(void);
    void CreateFoodToServerSlot(void);

private:
    QTimer *timer;
    int havefood;
    int FoodState;

};

#endif // THREAD_SEND_H
