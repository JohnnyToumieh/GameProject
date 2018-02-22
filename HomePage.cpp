#include "HomePage.h"
#include <QtWidgets>
#include "SignUp.h"
HomePage::HomePage()
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


void HomePage::setVerticalLayout(){
    VerticalL->addWidget(imageLabel);
    VerticalL->addWidget(signInButton);
    VerticalL->addWidget(signUpButton);
    VerticalL->addWidget(guestButton);
}

void HomePage::signupClicked(){
    SignUp *signup = new SignUp();
    QVBoxLayout *VerticalLayout = new QVBoxLayout();
    VerticalLayout->addWidget(signup);
    qDeleteAll(this->children());
    setLayout(VerticalLayout);
}

