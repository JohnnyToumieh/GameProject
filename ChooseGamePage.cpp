#include "ChooseGamePage.h"
#include "HomePage.h"
#include "GameOnePage.h"

ChooseGamePage::ChooseGamePage(QWidget *widget, User* user)
{
    this->widget=widget;
    this->user = user;

    verticalLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();
    grid = new QGridLayout();

    game1 = new QPushButton("Game 1: Aqua Clean");
    game2 = new QPushButton("Game 2: ");
    back = new QPushButton("Back to Home");

    profilePictureL = new QLabel();

    profilePictureL->setFixedHeight(100);
    profilePictureL->setFixedWidth(100);
    QPixmap profilePicture;
    profilePicture.load(QDir::currentPath() + "/user_photos/" + user->username + ".png");
    profilePicture.scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
    profilePictureL->setPixmap(profilePicture);
    profilePictureL->setScaledContents(true);


    nameL= new QLabel("     " + user->username);

    setVerticalLayout();
    widget->setFixedSize(500,300);
    widget->setLayout(verticalLayout);

    QObject::connect(back, SIGNAL(clicked()), SLOT(backToHomeClicked()));
    QObject::connect(game1, SIGNAL(clicked()), SLOT(game1Clicked()));
    QObject::connect(game2, SIGNAL(clicked()), SLOT(game2Clicked()));
}

void ChooseGamePage::backToHomeClicked(){
    qDeleteAll(widget->children());
    HomePage *homePage = new HomePage(widget);

}

void ChooseGamePage::game1Clicked(){
    qDeleteAll(widget->children());
    GameOnePage *gameOnePage = new GameOnePage(widget, 1, user);
}

void ChooseGamePage::game2Clicked(){
    qDeleteAll(widget->children());
    GameOnePage *gameOnePage = new GameOnePage(widget, 2, user);
}

void ChooseGamePage::setVerticalLayout()
{
    gridLayout->addWidget(profilePictureL,0,0);
    gridLayout->addWidget(new QLabel(""),0,1);
    gridLayout->addWidget(new QLabel(""),0,2);
    gridLayout->addWidget(nameL,1,0);
    gridLayout->addWidget(new QLabel(""),1,1);
    gridLayout->addWidget(new QLabel(""),1,2);
    verticalLayout->addItem(gridLayout);
    verticalLayout->addWidget(new QLabel(""));
    verticalLayout->addWidget(new QLabel(""));
    verticalLayout->addWidget(game1);
    verticalLayout->addWidget(game2);
    verticalLayout->addWidget(back);
    verticalLayout->addWidget(new QLabel(""));
}

