#include "SignIn.h"

#include "Message.h"
/**
 *\file SignIn.cpp
 *@brief contains SignIn class definition which represents the page where user chooses game
 *
 * SignIn class allow the user to choose game 1 or game 2
 * it takes him to the common page between the two games.
 *
 */

/**
 * @brief SignIn::SignIn constructor of SignIn class
 *
 * A constructor that set page buttons and labels
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject usersFile holds the info of the user
 */
SignIn::SignIn(QWidget *widget, User* user, QJsonObject usersFile)
{
    this->widget=widget;
    this->user = user;
    this->usersFile = usersFile;

    verticalLayout = new QVBoxLayout();

    submit = new QPushButton("Submit");
    back = new QPushButton("Back");

    username = new QLineEdit();
    password = new QLineEdit();
    password->setEchoMode(QLineEdit::Password);

    usernameL = new QLabel("Username");
    passwordL = new QLabel("Password");

    setVerticalLayout();
    widget->setFixedSize(500,600);
    widget->setLayout(verticalLayout);

    QObject::connect(submit, SIGNAL(clicked()), SLOT(submitClicked()));
    QObject::connect(back, SIGNAL(clicked()), SLOT(backClicked()));
}

/**
 * @brief SignIn::setVerticalLayout member function: set the vertical layout by adding items to it
 *
 */
void SignIn::setVerticalLayout()
{
    verticalLayout->addItem(new QSpacerItem(500, 200));
    verticalLayout->addWidget(usernameL);
    verticalLayout->addWidget(username);
    verticalLayout->addWidget(passwordL);
    verticalLayout->addWidget(password);
    verticalLayout->addItem(new QSpacerItem(500, 50));
    verticalLayout->addWidget(submit);
    verticalLayout->addWidget(back);
    verticalLayout->addItem(new QSpacerItem(500, 200));
}

/**
 * @brief SignIn::submitClicked member function: signin the user
 *
 * function that respond to clicking submit button by checking user's credentials
 * sigining him in if they match, or displaying message otherwise.
 */
void SignIn::submitClicked(){
    if(username->text() == NULL ||  username->text() == ""
       || password->text() ==NULL || password->text()==""){
        Message *msg = new Message("Some fields are empty! Please fill them and retry.");
        msg->show();
    }
    else{
        user->username = username->text();
        user->password = password->text();

        if (user->read(usersFile)) {
            qDeleteAll(widget->children());
            ChooseGamePage *choosegamePage = new ChooseGamePage(widget, user, usersFile);
        } else {
            Message *msg = new Message("Username/Password combination does not match.");
            msg->show();
            user->clear();
        }
    }
}

/**
 * @brief SignIn::backClicked member function: takes user back
 *
 * function that respond to clicking back button by taking the user to the previous page
 */
void SignIn::backClicked(){
    qDeleteAll(widget->children());
    HomePage *homePage = new HomePage(widget);

}
