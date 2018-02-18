#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "signUp.h"
#include <QWidget>
#include <QObject>
#include <QtWidgets>
class homePage : public QWidget
{
    Q_OBJECT
public:
    explicit homePage(QWidget *parent = nullptr);
    QPushButton *signInButton;
    QPushButton *signUpButton;
    QPushButton *guestButton;
    QLabel *imageLabel;
    //QPixmap *logo;
    QVBoxLayout *VerticalL;


signals:

private:
    void setVerticalLayout();
    void RemoveLayout (QWidget* widget);
public slots:
    void signupClicked();
};

#endif // HOMEPAGE_H
