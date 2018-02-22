#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "SignUp.h"
#include <QWidget>
#include <QObject>
#include <QtWidgets>
class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage();

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
