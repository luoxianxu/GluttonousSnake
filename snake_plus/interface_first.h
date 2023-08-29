#ifndef INTERFACE_FIRST_H
#define INTERFACE_FIRST_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

class interface_first : public QWidget
{
    Q_OBJECT
public:
    explicit interface_first(QWidget *parent = nullptr);
    void interface_init(void);
private:

    QVBoxLayout *layout;
    QLabel *label;
    QPushButton *start_button;
    QPushButton *record_button;

signals:

};

#endif // INTERFACE_FIRST_H
