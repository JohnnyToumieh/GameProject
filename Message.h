#ifndef MESSAGE_H
#define MESSAGE

#include <QWidget>
#include <QtWidgets>
class Message : public QWidget
{
    Q_OBJECT
public:
    explicit Message();
    Message(QString m);

    QLabel *msg;
    QVBoxLayout *VerticalL;

signals:

public slots:
};

#endif // MESSAGE_H
