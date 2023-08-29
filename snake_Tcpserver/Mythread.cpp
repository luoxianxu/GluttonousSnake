#include "Mythread.h"

Mythread::Mythread(QObject *parent) : QThread(parent)
{

}

Mythread::Mythread(qintptr sock,QObject *parent) : QThread(parent){
    this->sock = sock;
}

Mythread::~Mythread(){

}

void Mythread::run(){
    qDebug()<<"当前run线程id为"<<QThread::currentThreadId();
    MySocket *mysocket = new MySocket(sock,0);
    snake *Snake = new snake;
    if(!mysocket->setSocketDescriptor(sock))
        return;
    //客户端断开连接删除线程的槽函数连接
    connect(mysocket,&MySocket::disconnected,this,&Mythread::SocketDisconnectSlot);                     //客户端断开连接时与线程之间的信号槽
    //客户端断开连接删除snake的槽函数连接
    connect(mysocket,&MySocket::disconnected,Snake,&snake::SocketDisconnectedSlot);                     //客户端断开连接时与snake之间的信号槽，用于从链表中删除蛇
    //接收到客户端信息的槽函数连接  为定时器更新做准备
    connect(mysocket,&MySocket::SnakeChangeSignal,Snake,&snake::SnakeChangeSlot);                       //收到客户端信息，方向变换信号
    //线程创建时用于蛇的初始化
    connect(this,&Mythread::SnakeInitSignal,Snake,&snake::SnakeInitSlot);                               //蛇初始化信号与槽的连接

    connect(this,&Mythread::TimeOutAddSnakeToListSignal,Snake,&snake::TimeOutSnakeChangeSlot);          //定时器超时向snake发送存储蛇身的信号
    connect(this,&Mythread::SendFoodPointSignal,mysocket,&MySocket::SendFoodPointSlot);                 //发送食物坐标的信号槽
    //发送蛇的坐标
    connect(this,&Mythread::SendSnakeListSignal,mysocket,&MySocket::SendSnakeListSlot);

    connect(Snake,&snake::AddSnakeToThreadSignal,this,&Mythread::AddSnakeToThreadSlot);                 //在线程中触发添加蛇身至list的信号槽

    //客户端断开连接后，snake从链表删除的槽函数连接    用于snake的地址  不然可以直接从socket连接
    connect(Snake,&snake::DeleteSnakeFromListToThreadSignal,this,&Mythread::DeleteSnakeFromListSlot);   //从存储蛇身的链表中删除某一条蛇的信号槽
    //食物创建槽函数连接
    connect(Snake,&snake::CreateFoodToThreadSignal,this,&Mythread::CreateFoodToThreadSlot);             //需要创建食物的信号槽
    //存储蛇头坐标槽函数连接
    connect(Snake,&snake::storageSnakeHeadPointSignal,mysocket,&MySocket::storageSnakeHeadPointSlot);

    connect(Snake,&snake::SnakeDeadDelThreadFromListToThreadSignal,this,&Mythread::SocketDisconnectSlot);   //蛇死亡删除线程的信号
    triggerSnakeInitSignal();
    exec();
}

void Mythread::SocketDisconnectSlot(void){
    qDebug()<<"有客户端断开链接了";
//    MySocket *socket;
//    socket = (MySocket *)this->sender();
//    socket->disconnectFromHost();
//    socket->close();
//    socket->deleteLater();
    emit DeleteThreadFromListSignal();
}

void Mythread::triggerSnakeInitSignal(){
    srand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    int num = rand()%8;
    qDebug()<<"triggerSnakeInitSignal函数中的值为："<<num;
    emit SnakeInitSignal(num);
}

void Mythread::AddSnakeToThreadSlot(void){
    snake *Snake = (snake *)this->sender();
    qDebug()<<"AddSnakeToThreadSlot函数";
    emit AddSnakeToListSignal(Snake);
}

void Mythread::DeleteSnakeFromListSlot(void){
    snake *Snake = (snake *)this->sender();
    emit DeleteSnakeFromListSignal(Snake);
}

void Mythread::CreateFoodToThreadSlot(void){
    emit CreateFoodToServerSignal();
}

void Mythread::SendFoodPointToThreadSlot(QPoint point){
    qDebug()<<"线程中发送食物坐标的信号";
    emit SendFoodPointSignal(point);
}

void Mythread::SendSnakeListToThreadSlot(QVector<snake *> SnakeList){
    emit SendSnakeListSignal(SnakeList);
}

void Mythread::TimeOutAddSnakeToListInThreadSlot(void){
    qDebug()<<"thread中向snake发送存储蛇身的信号";
    emit TimeOutAddSnakeToListSignal();
}
