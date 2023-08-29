#include "mysocket.h"

#include <QThread>

MySocket::MySocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this,&MySocket::readyRead,this,&MySocket::ReceDataSlot);
}

MySocket::MySocket(qintptr sock,QObject *parent) : QTcpSocket(parent) , sock(sock){
    connect(this,&MySocket::readyRead,this,&MySocket::ReceDataSlot);
}

MySocket::~MySocket(){

}

void MySocket::ReceDataSlot(void){
    QString string;
    string = this->readAll();
    qDebug()<<"收到的数据为："<<string;
    emit SnakeChangeSignal(string);
    /*
    if(!string.compare("up")){
        qDebug()<<"蛇的运动方向为向上";
    }
    else if(!string.compare("down")){
        qDebug()<<"蛇的运动方向为向下";
    }
    else if(!string.compare("left")){
        qDebug()<<"蛇的运动方向为向左";
    }
    else if(!string.compare("right")){
        qDebug()<<"蛇的运动方向为向右";
    }
    else if(!string.compare("eat")){
        qDebug()<<"蛇吃到食物了";
    }
    else if(!string.compare("dead")){
        qDebug()<<"你的蛇死了！";
    }
    */
}

void MySocket::SendFoodPointSlot(QPoint point){
    qDebug()<<"发送食物坐标的线程号为："<<QThread::currentThreadId();
    qDebug()<<"食物横坐标为："<<point.rx()<<"  纵坐标为："<<point.ry();
    char FoodArray[20];
    if(point.rx()<10 && point.ry()>=10){
        sprintf(FoodArray,"FoodPoint:x=0%dy=%d",point.rx(),point.ry());
    }
    else if(point.rx()>=10 && point.ry()<10){
        sprintf(FoodArray,"FoodPoint:x=%dy=0%d",point.rx(),point.ry());
    }
    else if(point.rx()<10 &&point.ry()<10){
        sprintf(FoodArray,"FoodPoint:x=0%dy=0%d",point.rx(),point.ry());
    }
    else sprintf(FoodArray,"FoodPoint:x=%dy=%d",point.rx(),point.ry());
    this->write(FoodArray);
}

//信息结构  蛇头横坐标 纵坐标 蛇的条数  第某一条蛇的长度  第某一条蛇的某一个节点的x,y坐标
void MySocket::SendSnakeListSlot(QVector<snake *> SnakeList){
    QVector<int> SnakeData;

    if(this->point.rx()<10){
        SnakeData.append(0);
    }
    SnakeData.append(this->point.rx());

    if(this->point.ry()<10){
        SnakeData.append(0);
    }
    SnakeData.append(this->point.ry());

    if(SnakeList.length()<10){
        SnakeData.append(0);
    }
    SnakeData.append(SnakeList.length());
    for(int i=0;i<SnakeList.length();i++){
        qDebug()<<"第"<<i<<"条蛇的坐标信息：";
        if(SnakeList[i]->Snake.length()<10){
            SnakeData.append(0);
        }
        SnakeData.append(SnakeList[i]->Snake.length());
        qDebug()<<"length = "<<SnakeList[i]->Snake.length();
        for(int j=0;j<SnakeList[i]->Snake.length();j++){
            qDebug()<<"第"<<j<<"个节点的横坐标为："<<SnakeList[i]->Snake[j].rx()<<"  纵坐标为："<<SnakeList[i]->Snake[j].ry();
            if(SnakeList[i]->Snake[j].rx()<10){
                SnakeData.append(0);
            }
            SnakeData.append(SnakeList[i]->Snake[j].rx());
            if(SnakeList[i]->Snake[j].ry()<10){
                SnakeData.append(0);
            }
            SnakeData.append(SnakeList[i]->Snake[j].ry());
        }
    }
    QString str = "";
    for(int i=0;i<SnakeData.length();i++){
        char temp[4]="";
//        itoa(SnakeData[i],temp,10);            //将整数转换为字符串
        snprintf(temp,4,"%d",SnakeData[i]);
        str+=temp;
    }
    qDebug()<<"发送的信息为："<<str;
    this->write(str.toUtf8());                  //QString转换为QByteArray
}

void MySocket::storageSnakeHeadPointSlot(void){
    snake *Snake = (snake *)this->sender();
    this->point = Snake->SnakeHeadPoint;
}

//void MySocket::SendSnakeHeadPointSlot(QPoint point){
//    qDebug()<<"发送蛇头坐标的线程号为："<<QThread::currentThreadId();
//    qDebug()<<"蛇头横坐标为："<<point.rx()<<"  纵坐标为："<<point.ry();
//    char SnakeHeadArray[20];
//    if(point.rx()<10 && point.ry()>=10){
//        sprintf(SnakeHeadArray,"SnakeHeadPoint:x=0%dy=%d",point.rx(),point.ry());
//    }
//    else if(point.rx()>=10 && point.ry()<10){
//        sprintf(SnakeHeadArray,"SnakeHeadPoint:x=%dy=0%d",point.rx(),point.ry());
//    }
//    else if(point.rx()<10 &&point.ry()<10){
//        sprintf(SnakeHeadArray,"SnakeHeadPoint:x=0%dy=0%d",point.rx(),point.ry());
//    }
//    else sprintf(SnakeHeadArray,"SnakeHeadPoint:x=%dy=%d",point.rx(),point.ry());
//    this->write(SnakeHeadArray);
//}

void MySocket::TimeOutSendSnakeListSlot(void){
    qDebug()<<"发送存储蛇身的链表函数";
}
