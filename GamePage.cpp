#include "GamePage.h"

#include "Message.h"

/**
 *\file GamePage.cpp
 *@brief contains GamePage class definition which represents the page before starting the game (common between game1 and game2)
 *
 *
 * ChooseGamePage class  where user can start new game,resume an old game, check history, and select level.
 *
 */

/**
 * @brief GamePage::GamePage constructor of ChooseGamePage class
 *
 * A constructor that set page buttons and labels
 *
 * @param QWidget *widget represents the main widget holding all items
 * @param int gameNumber to specify what game it is
 * @param User* user represents the user signed in
 * @param QJsonObject dataFile holds the info of the user
 */

GamePage::GamePage(QWidget *widget, int gameNumber, User* user, QJsonObject dataFile)
{
    this->gameNumber = gameNumber;
    this->widget=widget;
    this->user = user;
    this->dataFile = dataFile;

    verticalLayout = new QVBoxLayout();
    gridLayout = new QGridLayout();
    grid = new QGridLayout();

    newGame = new QPushButton("Start a new game");
    resumeGame = new QPushButton("Resume old game");
    back = new QPushButton("Back");
    description = new QPushButton("Description");
    checkHistory = new QPushButton("Check your history");

    selectLevel = new QComboBox();
    selectLevel->addItem("Select Level");
    selectLevel->addItem("1");
    selectLevel->addItem("2");
    selectLevel->addItem("3");

    profilePictureL = new QLabel();

    //TODO: get specific user's image according to his username
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

    //Add background image
   // QPalette* pal = new QPalette();
   // pal->setBrush(QPalette::Background, QPixmap(QDir::currentPath() + "/user_photos/Guest.png"));
   // widget->setAutoFillBackground(true);
   // widget->setPalette(*pal);

    QObject::connect(back, SIGNAL(clicked()), SLOT(backClicked()));
    QObject::connect(description, SIGNAL(clicked()), SLOT(descriptionClicked()));
    QObject::connect(checkHistory, SIGNAL(clicked()), SLOT(checkHistoryClicked()));
    QObject::connect(newGame, SIGNAL(clicked()), SLOT(startNewGameClicked()));
    QObject::connect(resumeGame, SIGNAL(clicked()), SLOT(resumeGameClicked()));
}

/**
 * @brief GamePage::descriptionClicked member function: show game description
 *
 * function that respond to clicking Description button by showing a message
 * that briefly describe the game
 */
void GamePage::descriptionClicked(){
    if(gameNumber==1){
        Message *msg = new Message("SpongeBob has to move around the screen\nto collect items that increase his immunity level\nin order to be able to kill more bacteria\nand thus clean the aquarium.");
        msg->show();
    }
    else{
        Message *msg = new Message("Game 2 is still in the unknown :)!");
        msg->show();
    }
}

/**
 * @brief GamePage::startNewGameClicked member function: starts a new game
 *
 * function that responds to clicking Start New Game button by starting a new
 * game (depends on what game user chose on ChooseGamePage and level selected).
 */
void GamePage::startNewGameClicked(){
    QString levelSelected = selectLevel->currentText();
    int level = 1;
    if (levelSelected == "2") {
        level = 2;
    } else if (levelSelected == "3") {
        level = 3;
    }

    qDeleteAll(widget->children());
    widget->close();

    QGraphicsView *view;
    if (gameNumber == 1) {
        GameScene2 *game1 = new GameScene2(widget, 1000, 600, user, dataFile, false, level);

        view = new QGraphicsView(game1);
    } else {
        GameScene3 *game2 = new GameScene3(widget, 1000, 600, user, dataFile, false, level);

        view = new QGraphicsView(game2);
    }
    view->setFixedSize(1000,600);
    view->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setGeometry(
                QStyle::alignedRect(
                           Qt::LeftToRight,
                           Qt::AlignCenter,
                           view->size(),
                           qApp->desktop()->availableGeometry()
                    ));
    view->show();
}

/**
 * @brief GamePage::resumeGameExists member function: checks if the user has a game saved
 *
 * function that checks in JSON file of saved games, and looks if the current user
 * has any existing games to resume
 *
 * @return bool true if a game exist
 */
bool GamePage::resumeGameExists() {
    if (dataFile.contains("games") && dataFile["games"].isArray()) {
        QJsonArray games = dataFile["games"].toArray();
        if (games.size() > gameNumber - 1 && games.at(gameNumber).isObject()) {
            QJsonObject gameData = games.at(gameNumber).toObject();
            if (gameData.contains("users_save") && gameData["users_save"].isArray()) {
                QJsonArray userArray = gameData["users_save"].toArray();

                // If save for user already created, overwrite it
                for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
                    QJsonObject userObject = userArray[userIndex].toObject();
                    if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 * @brief GamePage::startNewGameClicked member function: resumer an old game
 *
 * function that responds to clicking Resume Old Game button by resuming an old
 * game (if the user has any).
 */
void GamePage::resumeGameClicked(){
    if (!resumeGameExists()) {
        Message *msg = new Message("No game found to resume.");
        msg->show();

        return;
    }

    qDeleteAll(widget->children());
    widget->close();

    QGraphicsView *view;
    if (gameNumber == 1) {
        GameScene1 *game1 = new GameScene1(widget, 1000, 600, user, dataFile, true);

        view = new QGraphicsView(game1);
    } else {
        GameScene3 *game2 = new GameScene3(widget, 1000, 600, user, dataFile, true);

        view = new QGraphicsView(game2);
    }

    view->setFixedSize(1000,600);
    view->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setVerticalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    view->setGeometry(
                QStyle::alignedRect(
                           Qt::LeftToRight,
                           Qt::AlignCenter,
                           view->size(),
                           qApp->desktop()->availableGeometry()
                    ));
    view->show();
}

/**
 * @brief GamePage::backClicked member function: takes user back
 *
 * function that respond to clicking back button by taking the user to the previous page
 */
void GamePage::backClicked(){
    qDeleteAll(widget->children());

    ChooseGamePage *chooseGamePage = new ChooseGamePage(widget, user, dataFile);
}

/**
 * @brief GamePage::startNewGameClicked member function: display history of the user
 *
 * function that responds to clicking History button by taking user to  history page
 * where user can check his performance and compare it to others.
 */
void GamePage::checkHistoryClicked(){
    qDeleteAll(widget->children());

    HistoryPage *historyPage = new HistoryPage(widget, gameNumber, user, dataFile);
}

/**
 * @brief GamePage::setVerticalLayout member function: sets the vertical layout by adding items to it
 *
 */
void GamePage::setVerticalLayout()
{
    gridLayout->addWidget(profilePictureL,0,0);
    gridLayout->addItem(new QSpacerItem(400,5),0,1);
    gridLayout->addItem(new QSpacerItem(400,5),0,2);
    gridLayout->addWidget(nameL,1,0);
    gridLayout->addItem(new QSpacerItem(400,5),1,1);
    gridLayout->addItem(new QSpacerItem(400,5),1,2);
    verticalLayout->addItem(gridLayout);

    verticalLayout->addItem(new QSpacerItem(400,200));

    verticalLayout->addWidget(description);
    verticalLayout->addWidget(newGame);
    if (!this->user->isGuest) {
        verticalLayout->addWidget(resumeGame);
    }
    verticalLayout->addWidget(selectLevel);
    if (!this->user->isGuest) {
        verticalLayout->addWidget(checkHistory);
    }
    verticalLayout->addWidget(back);

    verticalLayout->addItem(new QSpacerItem(400,200));
}
