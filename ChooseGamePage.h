#ifndef CHOOSEGAMEPAGE_H
#define CHOOSEGAMEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>
#include <ctime>

#include "HomePage.h"
#include "GameOnePage.h"
#include "User.h"

class ChooseGamePage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit ChooseGamePage(QWidget *widget, User* user, QJsonObject usersFile);

    QVBoxLayout* verticalLayout;
    QGridLayout* gridLayout;
    QGridLayout *grid;

    QWidget *widget;

    QPushButton *game1;
    QPushButton *game2;
    QPushButton *back;

    QLabel* profilePictureL;
    QLabel* nameL;

    User* user;
    QJsonObject usersFile;
signals:

private:
    void setVerticalLayout();

public slots:
    void backToHomeClicked();
    void game1Clicked();
    void game2Clicked();
};

#endif // CHOOSEGAMEPAGE_H
