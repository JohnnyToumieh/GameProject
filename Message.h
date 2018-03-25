#ifndef MESSAGE_H
#define MESSAGE

#include <QWidget>
#include <QtWidgets>
/**
 *\file Message.h
 *@brief The Message class, represents object used to display different messages to the user
 *
 */
class Message : public QWidget
{
    Q_OBJECT
public:
    explicit Message();

    Message(QString m);

    QLabel *msg;//!<QLabel msg which is the message to be displayed
    QVBoxLayout *VerticalL;

signals:

public slots:
};

#endif // MESSAGE_H
