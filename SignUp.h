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
    QLabel *firstName;
    QLabel *lastName;
    QLabel *email;
    QLabel *userName;
    QLabel *password;
    QLabel *confirmPassword;
    QLabel *empty;

    QLineEdit *fn;
    QLineEdit *ln;
    QLineEdit *e;
    QLineEdit *un;
    QLineEdit *pass;
    QLineEdit *cpass;

    QPushButton *submit;
    QPushButton *checkPassword;
    QPushButton *back;

    QRadioButton *male;
    QRadioButton *female;
    QSpinBox *age;

    QVBoxLayout *VerticalL;
    QGridLayout *Grid;
    QVBoxLayout *VLayout;
    QGroupBox *group;


private:
    void setVerticalLayout();
    void setGridLayout();

signals:

public slots:
    void checkPassClicked();
    void backToHomeClicked();
    void submitClicked();
};

#endif // SIGNUP_H
