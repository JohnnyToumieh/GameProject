#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>
#include <QFile>
#include <QIODevice>

#include "User.h"

class SignUp : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit SignUp(QWidget *widget, User* user, QJsonObject usersFile);

    bool passChecked;
    QWidget *widget;
    QVBoxLayout* verticalLayout;
    QGridLayout* gridLayout;
    QGridLayout* gridLayout1;
    QGridLayout* gridLayout2;

    QComboBox *day;
    QComboBox *month;
    QComboBox *year;

    QLabel* firstNameL;
    QLabel* lastNameL;
    QLabel* emailL;
    QLabel* usernameL;
    QLabel* passwordL;
    QLabel* passwordConfirmL;
    QLabel* genderL;
    QLabel* emptyL;
    QLabel* profilePictureL;
    QLabel* dateOfBirthL;

    QLineEdit* firstName;
    QLineEdit* lastName;
    QLineEdit* email;
    QLineEdit* username;
    QLineEdit* password;
    QLineEdit* passwordConfirm;

    QRadioButton* genderMale;
    QRadioButton* genderFemale;
    QVBoxLayout* genderVL;
    QGroupBox* genderGB;

    QPushButton* checkPassword;
    QPushButton* submit;
    QPushButton* back;
    QPushButton* choosePicture;

    User* user;
    QJsonObject usersFile;

private:
    void setVerticalLayout();
    void setGridLayout();
    bool containsANumber(QString text);

signals:

public slots:
    void checkPassClicked();
    void backToHomeClicked();
    void submitClicked();
    void choosePictureClicked();
};

#endif // SIGNUP_H
