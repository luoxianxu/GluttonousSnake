#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPainter>
#include <QDebug>
#include <QPoint>
#include <QTimer>
#include <QKeyEvent>


#define board_row   40
#define board_col   54

#define body_num    board_row*board_col

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

    void time_init(void);               //定时器的初始化啊
    void draw_borad(void);
    void snake_init(void);
    void draw_body(void);
    void draw_Food(void);
    void SnakeDeadJudge(void);
    void eatFoodJudge(void);
    QRect analysis_point(QPoint point);            //用于解析创建的蛇身坐标
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);


    int board_width = 15;
    int board_heigth = 15;
    int draw_flag = 0;
    int board_x;
    int board_y;
    int dir = 0;                        //记录蛇身移动的方向 默认 0为下  1为上 2为左 3为右

    QVector<QVector<QPoint>> SnakeList;

private slots:
    void on_start_button_clicked();

    void on_record_button_clicked();

    void on_game_buttton_clicked();

    void SocketReceDataSlot(void);


private:
    Ui::MyWidget *ui;
    int board[board_col][board_row];            //使用一个二维数组判断蛇是否死亡
    QTimer *timer;
    QTcpSocket *socket;
    QPoint SnakeFoodPoint;
    QPoint SnakeHeadPoint;
    int grade;


};
#endif // MYWIDGET_H
