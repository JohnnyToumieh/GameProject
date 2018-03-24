#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

#include "ChooseGamePage.h"
#include "HomePage.h"
#include "User.h"

class SignIn : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit SignIn(QWidget *widget, User* user, QJsonObject usersFile);
    QWidget *widget;

    QVBoxLayout *verticalLayout;

    QPushButton* submit;
    QPushButton* back;

    QLineEdit* username;
    QLineEdit* password;

    QLabel* usernameL;
    QLabel* passwordL;

    User* user;
    QJsonObject usersFile;
signals:

private:
     void setVerticalLayout();

public slots:
    void submitClicked();
    void backClicked();
};

#endif // SIGNIN_H
