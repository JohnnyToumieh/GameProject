#include "SignIn.h"
#include "Message.h"
#include "ChooseGamePage.h"

SignIn::SignIn(QWidget *widget, User* user, QJsonObject usersFile)
{
    this->widget=widget;

    verticalLayout = new QVBoxLayout();

    submit = new QPushButton("Submit");

    username = new QLineEdit();
    password = new QLineEdit();
    password->setEchoMode(QLineEdit::Password);

    usernameL = new QLabel("Username");
    passwordL = new QLabel("Password");

    setVerticalLayout();
    widget->setFixedSize(500,300);
    widget->setLayout(verticalLayout);

    QObject::connect(submit, SIGNAL(clicked()), SLOT(submitClicked()));

    this->user = user;
    this->usersFile = usersFile;
}

void SignIn::setVerticalLayout()
{
    verticalLayout->addWidget(usernameL);
    verticalLayout->addWidget(username);
    verticalLayout->addWidget(new QLabel(""));
    verticalLayout->addWidget(passwordL);
    verticalLayout->addWidget(password);
    verticalLayout->addWidget(new QLabel(""));
    verticalLayout->addWidget(submit);
}

void SignIn::submitClicked(){
    if(username->text() == NULL ||  username->text() == ""
       || password->text() ==NULL || password->text()==""){
        Message *msg = new Message("Some Fields are empty! Please Fill");
        msg->show();
    }
    else{
        user->username = username->text();
        user->password = password->text();

        if (user->read(usersFile)) {
            qDeleteAll(widget->children());
            ChooseGamePage *choosegamePage = new ChooseGamePage(widget, user);
        } else {
            Message *msg = new Message("Username/Password combination does not match.");
            msg->show();
            user->clear();
        }
    }
}
