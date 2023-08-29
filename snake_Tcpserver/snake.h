#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QDebug>

class snake : public QObject
{
    Q_OBJECT
public:
    explicit snake(QObject *parent = nullptr);

    ~snake();

    void SnakeInitSlot(int num);

    void triggerAddSnakeToThreadSignal(void);
    void triggerCreateFoodToThreadSignal(void);

    QVector<QPoint> Snake;
    QPoint SnakeHeadPoint;
signals:
    void AddSnakeToThreadSignal(void);
    void DeleteSnakeFromListToThreadSignal(void);
    void SnakeEatFoodSingal(void);
    void CreateFoodToThreadSignal(void);
    void TimeOutAddSnakeToListSignal(void);
//    void SendSnakeHeadPointSignal(QPoint point);
    void storageSnakeHeadPointSignal(void);

    //2023.8.29
    void SnakeDeadDelSnakeFromListToThreadSignal(void);
    void SnakeDeadDelThreadFromListToThreadSignal(void);


public slots:
    void SocketDisconnectedSlot(void);
    void SnakeChangeSlot(QString string);
    void TimeOutSnakeChangeSlot(void);
    void TimeOutAddSnakeToListSlot(void);
    void SnakeEatFoodSlot(void);

private:
    int dir;
    int SnakeState;



signals:

};

#endif // SNAKE_H
