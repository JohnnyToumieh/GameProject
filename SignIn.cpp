#include "SignIn.h"
#include "Message.h"
#include "ChooseGamePage.h"

SignIn::SignIn(QWidget *widget)
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
        qDeleteAll(widget->children());
        ChooseGamePage *choosegamePage = new ChooseGamePage(widget);
    }
}
