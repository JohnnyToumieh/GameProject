#include "homePage.h"
#include <QtWidgets>
#include "signUp.h"
homePage::homePage(QWidget *parent) : QWidget(parent)
{
    signInButton = new QPushButton("SignIn");
    signUpButton = new QPushButton("SignUp");
    guestButton = new QPushButton("Play as guest");
    QPixmap logo;
    logo.load("logo.JPG");
    imageLabel = new QLabel();
    imageLabel->setPixmap(logo);
    imageLabel->setScaledContents(true);
    VerticalL = new QVBoxLayout();

    QObject::connect(signUpButton, SIGNAL(clicked()), SLOT(signupClicked()));

    setVerticalLayout();
    setLayout(VerticalL);


}


void homePage::setVerticalLayout(){
    VerticalL->addWidget(imageLabel);
    VerticalL->addWidget(signInButton);
    VerticalL->addWidget(signUpButton);
    VerticalL->addWidget(guestButton);
}

void homePage::signupClicked(){
    signUp *signup = new signUp();
    QVBoxLayout *VerticalLayout = new QVBoxLayout();
    VerticalLayout->addWidget(signup);
    qDeleteAll(this->children());
    setLayout(VerticalLayout);
}

