#include "tcp_build.h"

tcp_build::tcp_build(QObject *parent) : QTcpServer(parent)
{
    this->ClientNum = 0;
    this->havefood = 0;     //食物有无  0表示没有食物  1有食物 用于初始化食物  只会用到一次
    timer = new QTimer(this);
    this->listen(QHostAddress::Any,8888);             //监听端口号为8888的网络端口
    timer->start(1000);
    qRegisterMetaType<QVector<snake *>>("QVector<snake *>");
}

tcp_build::~tcp_build(){

}

void tcp_build::incomingConnection(qintptr handle){
    this->havefood = 0;
    qDebug()<<ThreadList.length()<<"连接了";
    Mythread *mythread = new Mythread(handle,0);
    ThreadList.append(mythread);
    connect(mythread,&Mythread::DeleteThreadFromListSignal,this,&tcp_build::DeleteThreadFromListSlot);          //客户端断开连接 从链表删除线程的信号槽
    connect(mythread,&Mythread::DeleteSnakeFromListSignal,this,&tcp_build::DeleteSnakeFromListSlot);            //客户端断开连接 从链表删除蛇身的信号槽

    connect(mythread,&Mythread::AddSnakeToListSignal,this,&tcp_build::AddSnakeToListSlot);                      //在链表中添加蛇身的信号槽
    //食物被吃创建食物
    connect(mythread,&Mythread::CreateFoodToServerSignal,this,&tcp_build::CreateFoodToServerSlot);              //创建食物的信号槽
    connect(timer,&QTimer::timeout,mythread,&Mythread::TimeOutAddSnakeToListInThreadSlot);                      //定时器超时将客户端蛇身信息存储值list中
    connect(this,&tcp_build::SendFoodPointToThreadSignal,mythread,&Mythread::SendFoodPointToThreadSlot);        //发送食物坐标的信号与线程信号槽连接
    connect(this,&tcp_build::SendSnakeListToThreadSignal,mythread,&Mythread::SendSnakeListToThreadSlot);        //发送蛇身list的信号与线程信号槽链接
    mythread->start();
    CreateFoodToServerSlot();
}

void tcp_build::DeleteThreadFromListSlot(void){                 //从链表中删除线程
    Mythread *thread = (Mythread *)this->sender();
    for(int i=0;i<ThreadList.length();i++){
        if(thread == ThreadList[i]){
            qDebug()<<"删除了"<<i<<"号线程";
            ThreadList[i]->quit();
            ThreadList[i]->wait();
            ThreadList[i]->deleteLater();
            ThreadList.remove(i);
        }
    }
    if(ThreadList.length() == 0){
        this->havefood = 0;
    }
}

void tcp_build::AddSnakeToListSlot(snake *Snake){               //添加蛇至链表

    qDebug()<<"当前AddSnakeToListSlot线程id为"<<QThread::currentThreadId();
    qDebug()<<"SnakeList的长度为"<<SnakeList.length();
    snake *S = Snake;
    qDebug()<<"蛇的地址为："<<S;
    SnakeList.append(S);
    qDebug()<<"SnakeList的长度为"<<SnakeList.length();
    if(SnakeList.length() == ThreadList.length()){
        emit SendSnakeListToThreadSignal(SnakeList);
        SnakeList.clear();
        if(!this->havefood){
            triggerSendFoodPointToThreadSignal(FoodPoint);
            this->havefood = 1;
        }
    }
}

void tcp_build::DeleteSnakeFromListSlot(snake *SNake){          //从链表删除蛇
    qDebug()<<"当前DeleteSnakeFromListSlot线程id为"<<QThread::currentThreadId();
    qDebug()<<"SnakeList的长度为"<<SnakeList.length();
    snake *S = SNake;
    qDebug()<<"蛇的地址为："<<S;

    for(int i=0;i<SnakeList.length();i++){
        if( S == SnakeList[i] ){
            SnakeList.removeAt(i);
            qDebug()<<"删除了第"<<i<<"号位置的蛇";
        }
    }
    qDebug()<<"SnakeList的长度为"<<SnakeList.length();
}

void tcp_build::CreateFoodToServerSlot(void){                   //创建食物
    qDebug()<<"CreateFoodToServerSlot函数";
    /*
    if(this->havefood == 1){                 //如果有食物就发送食物坐标之后再返回
        triggerSendFoodPointToThreadSignal(FoodPoint);
        return;
    }
    */
    srand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    FoodPoint.rx() = rand()%53;
    FoodPoint.ry() = rand()%39;
    qDebug()<<"创建食物的横坐标为："<<FoodPoint.rx()<<"  纵坐标为："<<FoodPoint.ry();
    triggerSendFoodPointToThreadSignal(FoodPoint);
}

void tcp_build::triggerSendFoodPointToThreadSignal(QPoint point){
    qDebug()<<"发送食物坐标的信号发送了";
    qDebug()<<"创建食物的横坐标为："<<point.rx()<<"  纵坐标为："<<point.ry();
    emit SendFoodPointToThreadSignal(point);
}
