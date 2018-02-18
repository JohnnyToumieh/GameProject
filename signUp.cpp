#include "signUp.h"
#include "homePage.h"
#include "message.h"
signUp::signUp(QWidget *parent) : QWidget(parent)
{
    passChecked=false;
    firstName = new QLabel("FirstName");
    lastName = new QLabel("LastName");
    email = new QLabel("Email");
    userName = new QLabel("Username");
    password = new QLabel("Password");
    confirmPassword = new QLabel("Confirm Password");
    empty = new QLabel();

    fn = new QLineEdit();
    ln = new QLineEdit();
    e = new QLineEdit();
    un = new QLineEdit();
    pass = new QLineEdit();
    cpass = new QLineEdit();

    submit = new QPushButton("Submit");
    checkPassword = new QPushButton("Check Password");
    back = new QPushButton("Back to home");

    age = new QSpinBox();
    male = new QRadioButton("Male");
    female = new QRadioButton("Female");


    VerticalL = new QVBoxLayout();
    VLayout = new  QVBoxLayout();
    Grid = new QGridLayout();
    group = new QGroupBox();

    VLayout->addWidget(male);
    VLayout->addWidget(female);
    group->setLayout(VLayout);
    pass->setEchoMode(QLineEdit::Password);
    cpass->setEchoMode(QLineEdit::Password);

    setVerticalLayout();
    setGridLayout();
    setLayout(VerticalL);
    submit->setDisabled(true);

    QObject::connect(checkPassword, SIGNAL(clicked()), SLOT(checkPassClicked()));
    QObject::connect(back, SIGNAL(clicked()), SLOT(backToHomeClicked()));
    QObject::connect(submit, SIGNAL(clicked()), SLOT(submitClicked()));
}


void signUp::setVerticalLayout(){
    VerticalL->addItem(Grid);
    VerticalL->addWidget(checkPassword);
    VerticalL->addWidget(empty);
    VerticalL->addWidget(back);
    VerticalL->addWidget(submit);
}

void signUp::setGridLayout(){
    Grid->addWidget(firstName,0,0);
    Grid->addWidget(fn,0,1);
    Grid->addWidget(lastName,1,0);
    Grid->addWidget(ln,1,1);
    Grid->addWidget(email,2,0);
    Grid->addWidget(e,2,1);
    Grid->addWidget(userName,3,0);
    Grid->addWidget(un,3,1);
    Grid->addWidget(password,4,0);
    Grid->addWidget(pass,4,1);
    Grid->addWidget(confirmPassword,5,0);
    Grid->addWidget(cpass,5,1);
    Grid->addWidget(age,6,0);
    Grid->addWidget(group,6,1);

}

void signUp::checkPassClicked(){
    if(pass->text() == NULL || pass->text() == ""){
        empty->setText("Empty Password");
        submit->setDisabled(true);
    }
    else if(cpass->text() == NULL || cpass->text() == ""){
        empty->setText("Empty Password");
        submit->setDisabled(true);
        passChecked == false;
    }
    else if(pass->text()!=cpass->text()){
        empty->setText("Passwords don't match");
        submit->setDisabled(true);
        passChecked == false;
    }
    else{
        empty->setText("Passwords match");
        submit->setEnabled(true);
        passChecked == true;
    }

}

void signUp::submitClicked(){
    if(fn->text() == NULL || fn->text() == ""
       || ln->text() == NULL || ln->text() == ""
       || e->text() == NULL ||  e->text() == ""
       || un->text() == NULL ||  un->text() == ""
       || passChecked == false){
        message *msg = new message("Some Fields are empty! Please Fill");
        msg->show();
    }
    else{

    }
}

void signUp::backToHomeClicked(){
    homePage *homepage = new homePage();
    QVBoxLayout *VerticalLayout = new QVBoxLayout();
    VerticalLayout->addWidget(homepage);
    qDeleteAll(this->children());
    setLayout(VerticalLayout);
}
