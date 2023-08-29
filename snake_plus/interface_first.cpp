#include "interface_first.h"

interface_first::interface_first(QWidget *parent) : QWidget(parent)
{
    interface_init();
}

void interface_first::interface_init(void){

    layout = new QVBoxLayout(this);
    label = new QLabel(tr("贪吃蛇"));
    start_button = new QPushButton(tr("开始游戏"));
    record_button = new QPushButton(tr("历史记录"));
//    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    layout->addWidget(start_button);
    layout->addWidget(record_button);
    layout->setAlignment(Qt::AlignCenter);
}
