#include "SignUp.h"
#include "HomePage.h"
#include "Message.h"

SignUp::SignUp()
{
    verticalLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();
    gridLayout1= new QGridLayout();

    firstNameL = new QLabel("First Name:");
    lastNameL = new QLabel("Last Name:");
    emailL = new QLabel("Email:");
    usernameL = new QLabel("Username:");
    passwordL = new QLabel("Password:");
    passwordConfirmL = new QLabel("Confirm Password:");
    ageL = new QLabel("Age");
    genderL = new QLabel("Gender");
    emptyL = new QLabel("");
    profilePictureL = new QLabel("");

    profilePictureL->setFixedHeight(100);
    profilePictureL->setFixedWidth(100);
    QPixmap profilePicture;
    profilePicture.load(QDir::currentPath()+"/user_photos/profilepicture.png");
    profilePicture.scaled( 100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    profilePictureL->setPixmap(profilePicture);
    profilePictureL->setScaledContents(true);

    firstName = new QLineEdit();
    lastName = new QLineEdit();
    email = new QLineEdit();
    username = new QLineEdit();
    password = new QLineEdit();
    password->setEchoMode(QLineEdit::Password);
    passwordConfirm = new QLineEdit();
    passwordConfirm->setEchoMode(QLineEdit::Password);

    age = new QSpinBox();

    genderMale = new QRadioButton("Male");
    genderFemale = new QRadioButton("Female");
    genderVL = new QVBoxLayout();
    genderVL->addWidget(genderMale);
    genderVL->addWidget(genderFemale);
    genderGB = new QGroupBox();
    genderGB->setLayout(genderVL);

    checkPassword = new QPushButton("Check Password");
    submit = new QPushButton("Submit");
    submit->setEnabled(false);
    back = new QPushButton("Back to home");
    choosePicture = new QPushButton("Browse");
    choosePicture->setFixedHeight(30);
    choosePicture->setFixedWidth(100);

    setVerticalLayout();
    setGridLayout();

    setLayout(verticalLayout);

    QObject::connect(checkPassword, SIGNAL(clicked()), SLOT(checkPassClicked()));
    QObject::connect(back, SIGNAL(clicked()), SLOT(backToHomeClicked()));
    QObject::connect(submit, SIGNAL(clicked()), SLOT(submitClicked()));
    QObject::connect(choosePicture, SIGNAL(clicked()), SLOT(choosePictureClicked()));
}


void SignUp::setVerticalLayout()
{
    gridLayout1->addWidget(profilePictureL,0,0);
    gridLayout1->addWidget(new QLabel(""),0,1);
    gridLayout1->addWidget(new QLabel(""),0,2);
    gridLayout1->addWidget(choosePicture,1,0);
    gridLayout1->addWidget(new QLabel(""),1,1);
    gridLayout1->addWidget(new QLabel(""),1,2);
    verticalLayout->addItem(gridLayout1);
    verticalLayout->addItem(gridLayout);
    verticalLayout->addWidget(checkPassword);
    verticalLayout->addWidget(emptyL);
    verticalLayout->addWidget(back);
    verticalLayout->addWidget(submit);
}

void SignUp::setGridLayout()
{
    gridLayout->addWidget(firstNameL, 0, 0);
    gridLayout->addWidget(firstName, 0, 1);
    gridLayout->addWidget(lastNameL, 1, 0);
    gridLayout->addWidget(lastName, 1, 1);
    gridLayout->addWidget(emailL, 2, 0);
    gridLayout->addWidget(email, 2, 1);
    gridLayout->addWidget(usernameL, 3, 0);
    gridLayout->addWidget(username, 3, 1);
    gridLayout->addWidget(passwordL, 4, 0);
    gridLayout->addWidget(password, 4, 1);
    gridLayout->addWidget(passwordConfirmL, 5, 0);
    gridLayout->addWidget(passwordConfirm, 5, 1);
    gridLayout->addWidget(ageL, 6, 0);
    gridLayout->addWidget(genderL, 6, 1);
    gridLayout->addWidget(age, 7, 0);
    gridLayout->addWidget(genderGB, 7, 1);
}

void SignUp::checkPassClicked(){
    if(password->text() == NULL || password->text() == ""){
        emptyL->setText("Empty Password");
        submit->setDisabled(true);
    }
    else if(passwordConfirm->text() == NULL || passwordConfirm->text() == ""){
        emptyL->setText("Empty Confirmed Password");
        submit->setDisabled(true);
        passChecked == false;
    }
    else if(password->text()!=passwordConfirm->text()){
        emptyL->setText("Passwords don't match");
        submit->setDisabled(true);
        passChecked == false;
    }
    else{
        emptyL->setText("Passwords match");
        submit->setEnabled(true);
        passChecked == true;
    }

}

void SignUp::submitClicked(){
    if(firstName->text() == NULL || firstName->text() == ""
       || lastName->text() == NULL || lastName->text() == ""
       || email->text() == NULL ||  email->text() == ""
       || username->text() == NULL ||  username->text() == ""
       || passChecked == false){
        Message *msg = new Message("Some Fields are empty! Please Fill");
        msg->show();
    }
    else{

    }
}

void SignUp::backToHomeClicked(){
    HomePage *homepage = new HomePage();
    QVBoxLayout *VerticalLayout = new QVBoxLayout();
    VerticalLayout->addWidget(homepage);
    qDeleteAll(this->children());
    setLayout(VerticalLayout);
}

void SignUp::choosePictureClicked(){
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    if(QString::compare(fileName,QString())!=0){
        QPixmap profilePicture;
        bool valid = profilePicture.load(fileName);
        if(valid){
            profilePicture.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
            profilePictureL->setPixmap(profilePicture);
            profilePictureL->setScaledContents(true);
            profilePicture.save(QDir::currentPath()+"/user_photos/user","png");
        }

    }
}
