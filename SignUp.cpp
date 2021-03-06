#include "SignUp.h"

#include "Message.h"
/**
 *\file SignUp.cpp
 *@brief contains SignUp class definition which represents the page where user chooses game
 *
 * SignUp class allow the user to fill up his info
 * and get signed up to our awsome gaming plaftorm (for free!!)
 *
 */

/**
 * @brief SignUp::SignUp constructor of SignUp class
 *
 * A constructor that set page buttons and labels
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject dataFile holds the info of the user
 */
SignUp::SignUp(QWidget *widget, User* user, QJsonObject dataFile)
{
    this->widget=widget;
    this->user = user;
    this->dataFile = dataFile;

    verticalLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();
    gridLayout1 = new QGridLayout();
    gridLayout2 = new QGridLayout();

    firstNameL = new QLabel("First Name:");
    lastNameL = new QLabel("Last Name:");
    emailL = new QLabel("Email:");
    usernameL = new QLabel("Username:");
    passwordL = new QLabel("Password:");
    passwordConfirmL = new QLabel("Confirm Password:");
    genderL = new QLabel("Gender");
    emptyL = new QLabel("");
    profilePictureL = new QLabel("");
    dateOfBirthL = new QLabel("Date of birth:");

    profilePictureL->setFixedHeight(100);
    profilePictureL->setFixedWidth(100);
    QPixmap profilePicture;
    profilePicture.load(QDir::currentPath()+"/user_photos/Guest.png");
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
    back = new QPushButton("Back");
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

/**
 * @brief SignUp::setVerticalLayout member function: set the vertical layout by adding items to it
 *
 */
void SignUp::setVerticalLayout()
{
    gridLayout1->addWidget(profilePictureL,0,0);
    gridLayout1->addItem(new QSpacerItem(400, 30), 0, 1);
    gridLayout1->addItem(new QSpacerItem(400, 30), 0, 2);
    gridLayout1->addWidget(choosePicture,1,0);
    gridLayout1->addItem(new QSpacerItem(400, 30), 1, 1);
    gridLayout1->addItem(new QSpacerItem(400, 30), 1, 2);
    verticalLayout->addItem(gridLayout1);
    verticalLayout->addItem(gridLayout);
    verticalLayout->addWidget(checkPassword);
    verticalLayout->addWidget(emptyL);
    verticalLayout->addWidget(submit);
    verticalLayout->addWidget(back);
}

/**
 * @brief SignUp::setVerticalLayout member function: set the gridlayout layout by adding items to it
 *
 */
void SignUp::setGridLayout()
{
    gridLayout->addWidget(firstNameL, 0, 0);
    gridLayout->addWidget(firstName, 0, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 1, 0);
    gridLayout->addWidget(lastNameL, 2, 0);
    gridLayout->addWidget(lastName, 2, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 3, 0);
    gridLayout->addWidget(usernameL, 4, 0);
    gridLayout->addWidget(username, 4, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 5, 0);
    gridLayout->addWidget(passwordL, 6, 0);
    gridLayout->addWidget(password, 6, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 7, 0);
    gridLayout->addWidget(passwordConfirmL, 8, 0);
    gridLayout->addWidget(passwordConfirm, 8, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 9, 0);
    gridLayout->addWidget(emailL, 10, 0);
    gridLayout->addWidget(email, 10, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 11, 0);
    gridLayout->addWidget(dateOfBirthL,12,0);

    gridLayout2->addWidget(day,0,0);
    gridLayout2->addWidget(new QLabel(""),0,1);
    gridLayout2->addWidget(month,0,2);
    gridLayout2->addWidget(new QLabel(""),0,3);
    gridLayout2->addWidget(year,0,4);

    gridLayout->addItem(gridLayout2,12,1);
    gridLayout->addWidget(genderL, 13, 0);
    gridLayout->addWidget(genderGB, 13, 1);
    gridLayout->addItem(new QSpacerItem(50, 8), 14, 0);
}

/**
 * @brief SignIn::checkPassClicked member function: check if passord match requirements and matches the
 * re-entered password by the user
 *
 * function that respond to clicking checkPassword button by cheking password requirements
 * and matching and then display a label accoridingly
 */
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

/**
 * @brief SignUp::containsANumber member function: check if the passowrd contains a number
 *
 * function that take a string checks if it has a number and then return a boolean
 * @param QString text to be checked if it contains a number
 * @return bool true if a number is found
 */
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

/**
 * @brief SignUp::submitClicked member function: signup the user
 *
 * function that respond to clicking submit button by checking user's entered info
 * sigining him up if everything is ok, or displaying message otherwise.
 */
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

        if (user->read(dataFile)) {
            Message *msg = new Message("Username already exist!");
            msg->show();
            return;
        }

        user->firstName = firstName->text();
        user->lastName = lastName->text();
        user->email = email->text();
        user->gender = group.checkedButton()->text();
        user->DoBday = day->currentText().toInt();
        user->DoBmonth = month->currentText().toInt();
        user->DoByear = year->currentText().toInt();

        QPixmap const* profilePicture = profilePictureL->pixmap();
        profilePicture->save(QDir::currentPath()+"/user_photos/" + user->username + ".png","png");

        user->write(dataFile);

        QFile saveFile(QStringLiteral("Data.json"));

        if (!saveFile.open(QIODevice::WriteOnly)) {
            Message *msg = new Message("Couldn't open data file to save.");
            msg->show();
        }

        QJsonDocument saveDoc(dataFile);
        saveFile.write(saveDoc.toJson());

        qDeleteAll(widget->children());
        ChooseGamePage *choosegamePage = new ChooseGamePage(widget, user, dataFile);
    }
}

/**
 * @brief SignUp::backClicked member function: takes user back
 *
 * function that respond to clicking back button by taking the user to the previous page
 */
void SignUp::backToHomeClicked(){
    qDeleteAll(widget->children());
    HomePage *homepage = new HomePage(widget);

}

/**
 * @brief SignUp::choosePictureClicked member function: allow user to browse for a picture
 *
 * open the directory of the user allow him to choose a photo and take it to be set as his/her
 * profile picture
 */
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
