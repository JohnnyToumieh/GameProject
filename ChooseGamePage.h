#ifndef CHOOSEGAMEPAGE_H
#define CHOOSEGAMEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>
#include <ctime>

#include "HomePage.h"
#include "GameOnePage.h"
#include "User.h"

/**
 *\file ChooseGamePage.h
 *@brief The ChooseGamePage class, represents page at which user choose the game
 *
 */

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

    User* user;//!<User member user represents the signed in user
    QJsonObject usersFile;//!<QJsonObject member usersFile that holds information of the user
signals:

private:
    void setVerticalLayout();

public slots:
    void backToHomeClicked();//!<Member function takes user to previous page
    void game1Clicked();//!<Member function takes user to game1 page
    void game2Clicked();//!<Member function takes user to game2 page
};

#endif // CHOOSEGAMEPAGE_H
