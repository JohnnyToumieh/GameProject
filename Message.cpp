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
    setFixedSize(350,100);
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry()
        ));


}
