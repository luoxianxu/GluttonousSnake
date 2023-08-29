#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>
#include <QVector>
#include <QDebug>

#include "tcp_build.h"
#include "Mythread.h"

#define  Connect_Maxnum     50

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();


    tcp_build *server;

signals:

public slots:



private:
    Ui::MyWidget *ui;
};
#endif // MYWIDGET_H
