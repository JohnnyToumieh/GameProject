#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>
#include "User.h"

class HistoryPage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit HistoryPage(QWidget *widget,int gameNumber, User* user);
    int gameNumber;
    QWidget *widget;

    QVBoxLayout* verticalLayout;

    QPushButton *back;

    User* user;

signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();

};

#endif // HISTORYPAGE_H
