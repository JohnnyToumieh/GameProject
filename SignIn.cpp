#include "SignIn.h"

#include "Message.h"

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

void SignIn::backClicked(){
    qDeleteAll(widget->children());
    HomePage *homePage = new HomePage(widget);

}
