#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    socket = new QTcpSocket(this);
    this->grade = 0;
}

MyWidget::~MyWidget()
{
    delete ui;
}

//开始游戏按钮信号函数
void MyWidget::on_start_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    draw_borad();
}

//历史记录按钮信号函数
void MyWidget::on_record_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//游戏开始按钮信号函数
void MyWidget::on_game_buttton_clicked()
{
    draw_flag = 1;
    ui->game_widget->setStyleSheet(NULL);
    QString ip;
    ip = ui->IpLineEdit->text();
    quint16 port;
    port = ui->PortLinePort->text().toUInt();
    socket->connectToHost(ip,port);
    connect(socket,&QTcpSocket::readyRead,this,&MyWidget::SocketReceDataSlot);
}


void MyWidget::draw_borad(void)
{
    board_x = ui->game_widget->x()+23;
    board_y = ui->game_widget->y()+23;
    int i=0;
    QPainter painter(this);
    painter.translate(board_x,board_y);     //坐标系平移
    for(i=0;i<=board_row;i++)        //画行
        painter.drawLine(0,i*board_heigth,board_col*board_heigth,i*board_heigth);
    for(i=0;i<=board_col;i++)        //画列
        painter.drawLine(i*board_width,0,i*board_width,board_width*board_row);

}

void MyWidget::draw_body(void){
    QPainter painter(this);
    QBrush brush;
    board_x = ui->game_widget->x()+23;
    board_y = ui->game_widget->y()+23;
    painter.translate(board_x,board_y);     //坐标系平移
    QLinearGradient linearGradient(60, 50, 200, 200);
    linearGradient.setColorAt(0.2, Qt::white);
    linearGradient.setColorAt(0.6, Qt::green);
    linearGradient.setColorAt(1.0, Qt::black);
    painter.setBrush(QBrush(linearGradient));

    for(int i=0;i<SnakeList.length();i++){
        for(int j=0;j<SnakeList[i].length();j++){
            painter.drawRect(analysis_point(SnakeList[i][j]));
        }
    }
}

void MyWidget::draw_Food(void){
    QPainter painter(this);
    QBrush brush;
    board_x = ui->game_widget->x()+23;
    board_y = ui->game_widget->y()+23;
    painter.translate(board_x,board_y);     //坐标系平移
    painter.setBrush(Qt::red);
    painter.drawRect(analysis_point(SnakeFoodPoint));
}

QRect MyWidget::analysis_point(QPoint point){
    return QRect(point.rx()*board_width,point.ry()*board_heigth,board_width,board_heigth);
}

void MyWidget::paintEvent(QPaintEvent *){
    if( draw_flag ){
        draw_borad();
        draw_body();
        draw_Food();
    }

}

void MyWidget::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_W:
            socket->write("up");
            break;
        case Qt::Key_S:
            socket->write("down");
            dir = 0;
            break;
        case Qt::Key_A:
            socket->write("left");
            dir = 2;
            break;
        case Qt::Key_D:
            socket->write("right");
            dir = 3;
            break;
    }
}

void MyWidget::SocketReceDataSlot(void){
    QString str = socket->readAll();
    qDebug()<<"收到的信息为："<<str;
    qDebug()<<"收到的信息长度为："<<str.length();
    if(str!=""){
        if(str[0] == 'F'){
            SnakeFoodPoint.rx() = str.mid(12,2).toUInt();
            SnakeFoodPoint.ry() = str.mid(16,2).toUInt();
            qDebug()<<"食物创建的横坐标为："<<SnakeFoodPoint.rx();
            qDebug()<<"食物创建的纵坐标为："<<SnakeFoodPoint.ry();
        }
        else{
            if(str.contains('-')){          //正常情况下，不可能出现负数，除非蛇头出现在了图外
                socket->write("dead");
                return;
            }
            QVector<QPoint> Snake;
            int SnakeNum;
            int SnakeLength;
            int j=0;
            SnakeHeadPoint.rx() = str.mid(j,2).toUInt();
            j+=2;
            SnakeHeadPoint.ry() = str.mid(j,2).toUInt();
            j+=2;
            qDebug()<<"蛇头横坐标为："<<SnakeHeadPoint.rx()<<"  纵坐标为："<<SnakeHeadPoint.ry();
            SnakeNum = str.mid(j,2).toUInt();
            j+=2;
            SnakeList.clear();
            for(int i=0;i<SnakeNum;i++){
                SnakeLength = str.mid(j,2).toUInt();
                j+=2;
                for(int z=0;z<SnakeLength;z++){
                    QPoint point;
                    point.rx() = str.mid(j,2).toUInt();
                    j+=2;
                    point.ry() = str.mid(j,2).toUInt();
                    j+=2;
                    Snake.append(point);
                }
                SnakeList.append(Snake);
                Snake.clear();
            }
        }
        SnakeDeadJudge();
        eatFoodJudge();
        update();
    }
}

void MyWidget::SnakeDeadJudge(void){
    if(SnakeHeadPoint.rx()>53||SnakeHeadPoint.ry()>39)      //小于零的部分已经在接收数据的时候考虑过了
        socket->write("dead");
    memset(board,0,sizeof(board));      //初始化
    for(int i=0;i<SnakeList.length();i++){
        for(int j=0;j<SnakeList[i].length();j++){
            board[SnakeList[i][j].rx()][SnakeList[i][j].ry()]++;
            /*
            if(board[SnakeList[i][j].rx()][SnakeList[i][j].ry()]>=2){
                socket->write("dead");
                return;
            }
            */
        }
    }
    if(board[SnakeHeadPoint.rx()][SnakeHeadPoint.ry()]>1)
        socket->write("dead");
}

void MyWidget::eatFoodJudge(void){
    if(SnakeHeadPoint.rx() == SnakeFoodPoint.rx() && SnakeHeadPoint.ry() == SnakeFoodPoint.ry()){
        socket->write("eat");
        this->grade++;
        char data[4];
//        itoa(this->grade,data,10);
        snprintf(data,4,"%d",this->grade);
        ui->grade_label->setText(data);
    }
}

