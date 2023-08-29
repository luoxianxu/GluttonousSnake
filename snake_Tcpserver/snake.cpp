#include "snake.h"

snake::snake(QObject *parent) : QObject(parent){
    this->dir = 1;  //默认向下运动  1向下 2向上 3向左 4向右
    connect(this,&snake::SnakeEatFoodSingal,this,&snake::SnakeEatFoodSlot);
    connect(this,&snake::TimeOutAddSnakeToListSignal,this,&snake::TimeOutAddSnakeToListSlot);
}

snake::~snake(){

}
void snake::SnakeInitSlot(int num){
    QPoint point1;
    QPoint point2;
    switch(num%8){
        case 0:
            point1.rx() = 0;
            point1.ry() = 1;
            point2.rx() = 0;
            point2.ry() = 0;
            break;
        case 1:
            point1.rx() = 54/2;
            point1.ry() = 1;
            point2.rx() = 54/2;
            point2.ry() = 0;
            break;
        case 2:
            point1.rx() = 53;
            point1.ry() = 1;
            point2.rx() = 53;
            point2.ry() = 0;
            break;
        case 3:
            point1.rx() = 1;
            point1.ry() = 40/2;
            point2.rx() = 0;
            point2.ry() = 40/2;
            dir = 4;
            break;
        case 4:
            point1.rx() = 52;
            point1.ry() = 40/2;
            point2.rx() = 53;
            point2.ry() = 40/2;
            dir = 3;
            break;
        case 5:
            point1.rx() = 0;
            point1.ry() = 38;
            point2.rx() = 0;
            point2.ry() = 39;
            dir = 2;
            break;
        case 6:
            point1.rx() = 54/2;
            point1.ry() = 38;
            point2.rx() = 54/2;
            point2.ry() = 39;
            dir = 2;
            break;
        case 7:
            point1.rx() = 53;
            point1.ry() = 38;
            point2.rx() = 53;
            point2.ry() = 39;
            dir = 2;
            break;
    }
    Snake.append(point1);
    Snake.append(point2);
    qDebug()<<"SnakeInitSlot函数";
}

void snake::triggerAddSnakeToThreadSignal(void){
    emit AddSnakeToThreadSignal();
}

void snake::SocketDisconnectedSlot(void){
    emit DeleteSnakeFromListToThreadSignal();
}

void snake::SnakeChangeSlot(QString string){
    qDebug()<<"SnakeChangeSlot函数";
    if(!string.compare("up")){
        if(this->dir != 1)
            this->dir = 2;
    }
    else if(!string.compare("down")){
        if(this->dir != 2)
            this->dir = 1;
    }
    else if(!string.compare("left")){
        if(this->dir != 4)
            this->dir = 3;

    }
    else if(!string.compare("right")){
        if(this->dir != 3)
            this->dir = 4;
    }
    else if(!string.compare("eat")){
        emit SnakeEatFoodSingal();
    }
    else if(!string.compare("dead")){
        emit DeleteSnakeFromListToThreadSignal();
        emit SnakeDeadDelThreadFromListToThreadSignal();
    }
}

void snake::TimeOutSnakeChangeSlot(void){
    qDebug()<<"蛇身变化函数";
    QPoint point;
    for(int i=Snake.length()-1;i>0;i--){
        Snake[i] = Snake[i-1];
    }
    switch(this->dir){
        case 1:
            point.rx() = Snake[0].rx();
            point.ry() = Snake[0].ry()+1;
            break;
        case 2:
            point.rx() = Snake[0].rx();
            point.ry() = Snake[0].ry()-1;
            break;
        case 3:
            point.rx() = Snake[0].rx()-1;
            point.ry() = Snake[0].ry();
            break;
        case 4:
            point.rx() = Snake[0].rx()+1;
            point.ry() = Snake[0].ry();
            break;
    }
    SnakeHeadPoint = point;       //拷贝蛇头坐标
    Snake.replace(0,point);

    //判断蛇是否走出边界
    if(Snake[0].rx() > 53 || Snake[0].ry() > 39 || Snake[0].rx() < 0 || Snake[0].ry() < 0){
        emit DeleteSnakeFromListToThreadSignal();
        emit SnakeDeadDelThreadFromListToThreadSignal();
        return;
    }

    emit storageSnakeHeadPointSignal();
    emit TimeOutAddSnakeToListSignal();

}

void snake::SnakeEatFoodSlot(void){
    QPoint point;
    point = Snake[Snake.length()-1];
    Snake.append(point);
    triggerCreateFoodToThreadSignal();
}

void snake::triggerCreateFoodToThreadSignal(void){
    emit CreateFoodToThreadSignal();
}

void snake::TimeOutAddSnakeToListSlot(void){
    triggerAddSnakeToThreadSignal();        //初始化完成后触发与套接字之间的槽函数
}


