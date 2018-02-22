#include "Message.h"

Message::Message()
{

}

Message::Message(QString m)
{
    msg = new QLabel(m);
    VerticalL = new QVBoxLayout();
    VerticalL->addWidget(msg);
    setLayout(VerticalL);
}
