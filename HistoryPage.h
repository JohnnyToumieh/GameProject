#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>


class HistoryPage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit HistoryPage(QWidget *widget,int gameNumber);
    int gameNumber;
    QWidget *widget;

    QVBoxLayout* verticalLayout;

    QPushButton *back;


signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();

};

#endif // HISTORYPAGE_H
