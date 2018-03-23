#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>
#include <QtWidgets>

#include "SignUp.h"
#include "ChooseGamePage.h"
#include "SignIn.h"

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

    User* user;
    QJsonObject usersFile;

signals:

private:
    void setVerticalLayout();
    void RemoveLayout (QWidget* widget);
public slots:
    void signUpClicked();
    void signInClicked();
    void playAsGuestClicked();
};

#endif // HOMEPAGE_H
