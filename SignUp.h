#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>
#include <QFile>
#include <QIODevice>

#include "HomePage.h"
#include "ChooseGamePage.h"
#include "User.h"
/**
 *\file SignUp.h
 *@brief The SignUp class, represents the signIn page
 *
 */
class SignUp : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit SignUp(QWidget *widget, User* user, QJsonObject dataFile);

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

    User* user;//!<User member user represents the signed in user
    QJsonObject dataFile;//!QJsonObject member dataFile that holds information of the user

private:
    void setVerticalLayout();
    void setGridLayout();
    bool containsANumber(QString text);

signals:

public slots:
    void checkPassClicked();//!<Member function allows user to check password strength
    void backToHomeClicked();//!<Member function takes user to previous page
    void submitClicked();//!<Member function submit info to signup
    void choosePictureClicked();//!<Member function taht allow user choose his/her photo
};

#endif // SIGNUP_H
