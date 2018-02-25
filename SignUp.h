#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QObject>
#include <QtWidgets>

class SignUp : public QWidget
{
    Q_OBJECT
public:
    explicit SignUp();

    bool passChecked;

    QVBoxLayout* verticalLayout;
    QGridLayout* gridLayout;
    QGridLayout* gridLayout1;

    QLabel* firstNameL;
    QLabel* lastNameL;
    QLabel* emailL;
    QLabel* usernameL;
    QLabel* passwordL;
    QLabel* passwordConfirmL;
    QLabel* ageL;
    QLabel* genderL;
    QLabel* emptyL;
    QLabel* profilePictureL;

    QLineEdit* firstName;
    QLineEdit* lastName;
    QLineEdit* email;
    QLineEdit* username;
    QLineEdit* password;
    QLineEdit* passwordConfirm;

    QSpinBox* age;

    QRadioButton* genderMale;
    QRadioButton* genderFemale;
    QVBoxLayout* genderVL;
    QGroupBox* genderGB;

    QPushButton* checkPassword;
    QPushButton* submit;
    QPushButton* back;
    QPushButton* choosePicture;

private:
    void setVerticalLayout();
    void setGridLayout();

signals:

public slots:
    void checkPassClicked();
    void backToHomeClicked();
    void submitClicked();
    void choosePictureClicked();
};

#endif // SIGNUP_H
