#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

class SignIn : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit SignIn(QWidget *widget);
    QWidget *widget;

    QVBoxLayout *verticalLayout;

    QPushButton* submit;
    QPushButton* back;

    QLineEdit* username;
    QLineEdit* password;

    QLabel* usernameL;
    QLabel* passwordL;
signals:

private:
     void setVerticalLayout();

public slots:
    void submitClicked();
    void backClicked();
};

#endif // SIGNIN_H
