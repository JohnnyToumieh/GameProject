#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

#include "ChooseGamePage.h"
#include "HomePage.h"
#include "User.h"
/**
 *\file SignIn.h
 *@brief The SignIn class, represents the signIn page
 *
 */

class SignIn : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit SignIn(QWidget *widget, User* user, QJsonObject dataFile);
    QWidget *widget;

    QVBoxLayout *verticalLayout;

    QPushButton* submit;
    QPushButton* back;

    QLineEdit* username;
    QLineEdit* password;

    QLabel* usernameL;
    QLabel* passwordL;

    User* user;//!<User member user represents the signed in user
    QJsonObject dataFile;//!QJsonObject member dataFile that holds information of the user
signals:

private:
     void setVerticalLayout();

public slots:
    void submitClicked();//!<Member function submit credentials to signin
    void backClicked();//!<Member function takes user to previous page
};

#endif // SIGNIN_H
