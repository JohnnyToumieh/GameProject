#ifndef MESSAGE_H
#define MESSAGE

#include <QWidget>
#include <QtWidgets>
class message : public QWidget
{
    Q_OBJECT
public:
    explicit message(QWidget *parent = nullptr);
    message(QString m);
    QLabel *msg;
    QVBoxLayout *VerticalL;

signals:

public slots:
};

#endif // MESSAGE_H
