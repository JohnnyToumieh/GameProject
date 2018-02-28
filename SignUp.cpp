#include "SignUp.h"
#include "HomePage.h"
#include "Message.h"
#include "ChooseGamePage.h"

SignUp::SignUp(QWidget *widget, User* user, QJsonObject usersFile)
{
    this->widget=widget;
    this->user = user;
    this->usersFile = usersFile;

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
    dateOfBirthL = new QLabel("Date of birth:");

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

    checkPassword = new QPushButton("Check password");
    submit = new QPushButton("Submit");
    submit->setEnabled(false);
    back = new QPushButton("Back to home");
    choosePicture = new QPushButton("Browse");
    choosePicture->setFixedHeight(30);
    choosePicture->setFixedWidth(100);

    day = new QComboBox;
    year = new QComboBox;
    month = new QComboBox;

    QStringList years;
    years.append("Year");
    for(int i=1970;i<=2017;i++){
        years.append(QString::number(i));
    }
    year->addItems(years);

    QStringList months;
    months.append("Month");
    for(int i=1;i<=12;i++){
        months.append(QString::number(i));
    }
    month->addItems(months);

    QStringList days;
    days.append("Day");
    for(int i=1;i<=31;i++){
        days.append(QString::number(i));
    }
    day->addItems(days);

    setVerticalLayout();
    setGridLayout();

    widget->setLayout(verticalLayout);

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
    gridLayout1->addWidget(dateOfBirthL,2,0);
    gridLayout1->addWidget(new QLabel(""),2,1);
    gridLayout1->addWidget(day,2,2);
    gridLayout1->addWidget(new QLabel(""),2,3);
    gridLayout1->addWidget(month,2,4);
    gridLayout1->addWidget(new QLabel(""),2,5);
    gridLayout1->addWidget(year,2,6);
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
        emptyL->setText("Empty password");
        submit->setDisabled(true);
        passChecked = false;
    }
    else if(passwordConfirm->text() == NULL || passwordConfirm->text() == ""){
        emptyL->setText("Empty confirmed password");
        submit->setDisabled(true);
        passChecked = false;
    }
    else if(password->text()!=passwordConfirm->text()){
        emptyL->setText("Passwords don't match");
        submit->setDisabled(true);
        passChecked = false;
    }
    else{
        if (password->text().size() < 8
                || password->text().toLower() == password->text()
                || password->text().toUpper() == password->text()
                || !containsANumber(password->text())) {
            emptyL->setText("Make sure your password is at least 8 characters, has a number,\na lower case character and an upper case character.");
            submit->setDisabled(true);
            passChecked = false;
        } else {
            emptyL->setText("Passwords match");
            submit->setEnabled(true);
            passChecked = true;
        }
    }

}

bool SignUp::containsANumber(QString text) {
    if (text.contains("0")
            || text.contains("1")
            || text.contains("2")
            || text.contains("3")
            || text.contains("4")
            || text.contains("5")
            || text.contains("6")
            || text.contains("7")
            || text.contains("8")
            || text.contains("9")) {
        return true;
    } else {
        return false;
    }
}

void SignUp::submitClicked(){
    QButtonGroup group;
    QList<QRadioButton *> allButtons = genderGB->findChildren<QRadioButton *>();
    for(int i = 0; i < allButtons.size(); ++i) {
        group.addButton(allButtons[i],i);
    }

    if(firstName->text() == NULL || firstName->text() == ""
       || lastName->text() == NULL || lastName->text() == ""
       || email->text() == NULL ||  email->text() == ""
       || username->text() == NULL ||  username->text() == ""
       || passChecked == false || day->currentText()=="Day"
       || month->currentText()=="Month" || year->currentText()=="Year"
       || allButtons.size() == 0 || group.checkedId() < 0){
        Message *msg = new Message("Some fields are empty! Please fill.");
        msg->show();
    } else {
        user->username = username->text();
        user->password = password->text();

        if (user->read(usersFile)) {
            Message *msg = new Message("Username already exist!");
            msg->show();
            return;
        }

        user->firstName = firstName->text();
        user->lastName = lastName->text();
        user->email = email->text();
        user->age = age->text();
        user->gender = group.checkedButton()->text();
        user->DoBday = day->currentText();
        user->DoBmonth = month->currentText();
        user->DoByear = year->currentText();

        QPixmap const* profilePicture = profilePictureL->pixmap();
        profilePicture->save(QDir::currentPath()+"/user_photos/" + user->username + ".png","png");

        user->write(usersFile);

        QFile saveFile(QStringLiteral("Data.json"));

        if (!saveFile.open(QIODevice::WriteOnly)) {
            Message *msg = new Message("Couldn't open data file to save.");
            msg->show();
        }

        QJsonDocument saveDoc(usersFile);
        saveFile.write(saveDoc.toJson());

        qDeleteAll(widget->children());
        ChooseGamePage *choosegamePage = new ChooseGamePage(widget, user);
    }
}

void SignUp::backToHomeClicked(){
    qDeleteAll(widget->children());
    HomePage *homepage = new HomePage(widget);

}

void SignUp::choosePictureClicked(){
    QString fileName = QFileDialog::getOpenFileName(widget,tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    if(QString::compare(fileName,QString())!=0){
        QPixmap profilePicture;
        bool valid = profilePicture.load(fileName);
        if(valid){
            profilePicture.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
            profilePictureL->setPixmap(profilePicture);
            profilePictureL->setScaledContents(true);
        }
    }
}
