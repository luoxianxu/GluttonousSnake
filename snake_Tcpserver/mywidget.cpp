#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    server = new tcp_build(this);

    qDebug()<<"MyWidget的线程为"<<QThread::currentThreadId();
}

MyWidget::~MyWidget()
{
    delete ui;
}

