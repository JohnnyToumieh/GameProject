#include "HomePage.h"

#include "Message.h"
/**
 *\file HomePage.cpp
 *@brief contains HomePage class definition and functions implementation, which represents main home application's page
 *
 *
 */

/**
 * @brief HomePage::HomePage constructor of HomePage class
 *
 * A constructor that set up the page with all labels and buttons
 * In addition to creating the JSON file
 * @param QWidget *widget represents the main widget holding all items
 */
HomePage::HomePage(QWidget *widget)
{
    this->widget=widget;

    signInButton = new QPushButton("Sign in");
    signUpButton = new QPushButton("Sign up");
    guestButton = new QPushButton("Play as guest");

    doneByL = new QLabel("\t\t\tGaming Platform\n\t\t\t       Done by \n\t\t\tHassan & Johnny");

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

    QFile loadFile(QStringLiteral("Data.json"));

    if (!loadFile.open(QIODevice::ReadWrite)) {
        Message *msg = new Message("Couldn't open data file to load.");
        msg->show();
    }

    user = new User();

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    usersFile = loadDoc.object();
}

/**
 * @brief HistoryPage::setVerticalLayout member function: sets the vertical layout by adding items to it
 *
 */
void HomePage::setVerticalLayout(){
    VerticalL->addWidget(imageLabel);
    VerticalL->addWidget(doneByL);
    VerticalL->addWidget(signInButton);
    VerticalL->addWidget(signUpButton);
    VerticalL->addWidget(guestButton);
    VerticalL->addItem(new QSpacerItem(500,200));
}

/**
 * @brief HistoryPage::backClicked member function: take user to sigUp page
 *
 * function that respond to clicking signUp button by taking the user to the sigUp page
 */
void HomePage::signUpClicked(){
    qDeleteAll(widget->children());
    SignUp *signup = new SignUp(widget, user, usersFile);
}

/**
 * @brief HistoryPage::backClicked member function: take user to signIn page
 *
 * function that respond to clicking signIn button by taking the user to the signIn page
 */
void HomePage::signInClicked(){
    qDeleteAll(widget->children());
    SignIn *signIn = new SignIn(widget, user, usersFile);
}

/**
 * @brief HistoryPage::backClicked member function: take user to ChooseGamePage directly as a guest
 *
 * function that respond to clicking playAsGuest button by taking the user to ChooseGamePage directly as a guest
 */
void HomePage::playAsGuestClicked(){
    user->username = "Guest";
    user->firstName = "Guest";
    user->isGuest = true;

    qDeleteAll(widget->children());
    ChooseGamePage *choosegamePage = new ChooseGamePage(widget, user, usersFile);
}
