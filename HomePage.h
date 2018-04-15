#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>
#include <QtWidgets>

#include "SignUp.h"
#include "ChooseGamePage.h"
#include "SignIn.h"

/**
 *\file HomePage.h
 *@brief The HomePage class, represents the main hompe page of the application (Healthier)
 *
 */

class HomePage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *widget);

    QWidget *widget;

    QPushButton *signInButton;
    QPushButton *signUpButton;
    QPushButton *guestButton;

    QLabel *imageLabel;
    QLabel *doneByL;

    QVBoxLayout *VerticalL;

    User* user;//!<User member user represents the signed in user
    QJsonObject dataFile;//!QJsonObject member dataFile that holds information of the user

signals:

private:
    void setVerticalLayout();
    void RemoveLayout (QWidget* widget);
    void setUpDataFile();
public slots:
    void signUpClicked();//!<Member function takes user to sigin page
    void signInClicked();//!<Member function takes user to signup page
    void playAsGuestClicked();//!<Member function takes user to choose game page (as a guest)
};

#endif // HOMEPAGE_H
