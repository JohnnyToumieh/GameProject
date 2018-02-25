#include "HomePage.h"
#include <QtWidgets>
#include "SignUp.h"
#include "ChooseGamePage.h"
#include "SignIn.h"

HomePage::HomePage(QWidget *widget)
{
    this->widget=widget;

    signInButton = new QPushButton("SignIn");
    signUpButton = new QPushButton("SignUp");
    guestButton = new QPushButton("Play as guest");

    doneByL = new QLabel("\t\t\tGaming Platform\n\t\t\t       Done by \n\t\t\tHassan & Jhonny");

    QPixmap logo;
    logo.load("logo2.JPG");
    imageLabel = new QLabel();
    imageLabel->setPixmap(logo);

    VerticalL = new QVBoxLayout();

    QObject::connect(signUpButton, SIGNAL(clicked()), SLOT(signUpClicked()));
    QObject::connect(signInButton, SIGNAL(clicked()), SLOT(signInClicked()));
    QObject::connect(guestButton, SIGNAL(clicked()), SLOT(playAsGuestClicked()));

    setVerticalLayout();

    widget->setFixedSize(500,600);

    widget->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            widget->size(),
            qApp->desktop()->availableGeometry()
        ));

    widget->setLayout(VerticalL);

}


void HomePage::setVerticalLayout(){
    VerticalL->addWidget(imageLabel);
    VerticalL->addWidget(doneByL);
    VerticalL->addWidget(signInButton);
    VerticalL->addWidget(signUpButton);
    VerticalL->addWidget(guestButton);
    VerticalL->addWidget(new QLabel(""));
}

void HomePage::signUpClicked(){
    qDeleteAll(widget->children());
    SignUp *signup = new SignUp(widget);
}

void HomePage::signInClicked(){
    qDeleteAll(widget->children());
    SignIn *signIn = new SignIn(widget);
}

void HomePage::playAsGuestClicked(){
    qDeleteAll(widget->children());
    ChooseGamePage *choosegamePage = new ChooseGamePage(widget);
}

