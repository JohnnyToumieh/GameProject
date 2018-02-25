#ifndef HOMEPAGE_H
#define HOMEPAGE_H
#include "SignUp.h"
#include <QWidget>
#include <QObject>
#include <QtWidgets>
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
