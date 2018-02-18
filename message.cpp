#include "message.h"

message::message(QWidget *parent) : QWidget(parent)
{

}

message::message(QString m)
{
    msg = new QLabel(m);
    VerticalL = new QVBoxLayout();
    VerticalL->addWidget(msg);
    setLayout(VerticalL);
}
