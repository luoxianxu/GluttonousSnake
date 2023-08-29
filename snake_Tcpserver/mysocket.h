#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "snake.h"

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent = nullptr);

    MySocket(qintptr sock,QObject *parent);

    ~MySocket();

    QPoint point;

private:
    qintptr sock;

signals:
    void SnakeChangeSignal(QString string);

    //2023.8.29
    void SnakeDeadSignal(void);
public slots:
    void ReceDataSlot(void);
    void SendFoodPointSlot(QPoint point);
    void SendSnakeListSlot(QVector<snake *> SnakeList);
//    void SendSnakeHeadPointSlot(QPoint point);
    void TimeOutSendSnakeListSlot(void);
    void storageSnakeHeadPointSlot(void);
};

#endif // MYSOCKET_H
