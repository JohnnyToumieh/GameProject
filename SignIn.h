#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>
#include "User.h"

class SignIn : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit SignIn(QWidget *widget, User* user, QJsonObject usersFile);
    QWidget *widget;

    QVBoxLayout *verticalLayout;

    QPushButton* submit;

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
};

#endif // SIGNIN_H
