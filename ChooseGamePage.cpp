#include "ChooseGamePage.h"

#include "Message.h"

/**
 *\file ChooseGamePage.cpp
 *@brief contains ChooseGamePage class definition which represents the page where user chooses game
 *
 * ChooseGamePage class allow the user to choose game 1 or game 2
 * it takes him to the common page between the two games.
 *
 */

/**
 * @brief ChooseGamePage::ChooseGamePage constructor of ChooseGamePage class
 *
 * A constructor that set page buttons and labels
 * including game1,game2, and back buttons
 * along with the user's name and picture.
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject usersFile holds the info of the user
 */

ChooseGamePage::ChooseGamePage(QWidget *widget, User* user, QJsonObject usersFile)
{
    this->widget=widget;
    this->user = user;
    this->usersFile = usersFile;

    verticalLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();
    grid = new QGridLayout();

    game1 = new QPushButton("Game 1: Aqua Clean");
    game2 = new QPushButton("Game 2: TBA");
    back = new QPushButton("Back to home");

    profilePictureL = new QLabel();

    profilePictureL->setFixedHeight(100);
    profilePictureL->setFixedWidth(100);
    QPixmap profilePicture;
    profilePicture.load(QDir::currentPath() + "/user_photos/" + user->username + ".png");
    profilePicture.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    profilePictureL->setPixmap(profilePicture);
    profilePictureL->setScaledContents(true);


    nameL= new QLabel("     " + user->firstName + " " + user->lastName);

    setVerticalLayout();
    widget->setFixedSize(500,600);
    widget->setLayout(verticalLayout);

    QObject::connect(back, SIGNAL(clicked()), SLOT(backToHomeClicked()));
    QObject::connect(game1, SIGNAL(clicked()), SLOT(game1Clicked()));
    QObject::connect(game2, SIGNAL(clicked()), SLOT(game2Clicked()));

    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);

    if (user->DoBday == QString::number(now->tm_mday)
            && user->DoBmonth == QString::number(now->tm_mon + 1)) {
        Message *msg = new Message("Happy birthday " + user->firstName + "!");
        msg->show();
    }
}

/**
 * @brief ChooseGamePage::backToHomeClicked member function: takes user back
 *
 * function that respond to clicking back button by taking the user to the previous page
 */
void ChooseGamePage::backToHomeClicked(){
    qDeleteAll(widget->children());
    HomePage *homePage = new HomePage(widget);

}

/**
 * @brief ChooseGamePage::game1Clicked member function: takes user to game 1 page
 *
 * function that respond to clicking game 1 button by taking the user to the game 1 page
 */
void ChooseGamePage::game1Clicked(){
    qDeleteAll(widget->children());
    GameOnePage *gameOnePage = new GameOnePage(widget, 1, user, usersFile);
}

/**
 * @brief ChooseGamePage::game2Clicked member function: takes user to game 2 page
 *
 * function that respond to clicking game 2 button by taking the user to the game 2 page
 */
void ChooseGamePage::game2Clicked(){
    qDeleteAll(widget->children());
    GameOnePage *gameOnePage = new GameOnePage(widget, 2, user, usersFile);
}

/**
 * @brief ChooseGamePage::setVerticalLayout member function: set the vertical layout by adding items to it
 *
 */
void ChooseGamePage::setVerticalLayout()
{
    gridLayout->addWidget(profilePictureL,0,0);
    gridLayout->addItem(new QSpacerItem(200,5),0,1);
    gridLayout->addItem(new QSpacerItem(200,5),0,2);
    gridLayout->addWidget(nameL,1,0);
    gridLayout->addItem(new QSpacerItem(200,5),1,1);
    gridLayout->addItem(new QSpacerItem(200,5),1,2);
    verticalLayout->addItem(gridLayout);

    verticalLayout->addItem(new QSpacerItem(400,200));
    verticalLayout->addWidget(game1);
    verticalLayout->addWidget(game2);
    verticalLayout->addWidget(back);
    verticalLayout->addItem(new QSpacerItem(400,200));
}

