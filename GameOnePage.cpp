#include "GameOnePage.h"
#include "ChooseGamePage.h"
#include "Message.h"

GameOnePage::GameOnePage(QWidget *widget, int gameNumber, User* user)
{
    this->gameNumber = gameNumber;
    this->widget=widget;
    this->user = user;

    verticalLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();
    grid = new QGridLayout();

    newGame = new QPushButton("Start a new game");
    resumeGame = new QPushButton("Resume old game");
    back = new QPushButton("Back");
    description = new QPushButton("Description");
    checkHistory = new QPushButton("Check your history");

    selectLevel = new QComboBox();
    selectLevel->addItem("Select level");
    selectLevel->addItem("Easy");
    selectLevel->addItem("Medium");
    selectLevel->addItem("Hard");

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
    widget->setFixedSize(500,350);
    widget->setLayout(verticalLayout);

    QObject::connect(back, SIGNAL(clicked()), SLOT(backClicked()));
    QObject::connect(description, SIGNAL(clicked()), SLOT(descriptionClicked()));
}

void GameOnePage::descriptionClicked(){
    if(gameNumber==1){
        Message *msg = new Message("SpongeBob has to move around the screen\nto collect items that increase his immunity level\nin order to be able to kill more bacteria\nand thus clean the aquarium.");
        msg->show();
    }
    else{
        Message *msg = new Message("Game 2 is still in the unknown :)!");
        msg->show();
    }
}

void GameOnePage::backClicked(){
    qDeleteAll(widget->children());
    ChooseGamePage *chooseGamePage = new ChooseGamePage(widget, user);
}

void GameOnePage::setVerticalLayout()
{
    gridLayout->addWidget(profilePictureL,0,0);
    gridLayout->addWidget(new QLabel(""),0,1);
    gridLayout->addWidget(new QLabel(""),0,2);
    gridLayout->addWidget(nameL,1,0);
    gridLayout->addWidget(new QLabel(""),1,1);
    gridLayout->addWidget(new QLabel(""),1,2);
    verticalLayout->addItem(gridLayout);
    verticalLayout->addWidget(description);
    verticalLayout->addWidget(newGame);
    verticalLayout->addWidget(resumeGame);
    verticalLayout->addWidget(selectLevel);
    verticalLayout->addWidget(checkHistory);
    verticalLayout->addWidget(back);
}
