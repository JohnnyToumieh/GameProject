#include "HistoryPage.h"
/**
 *\file HistoryPage.cpp
 *@brief contains HistoryPage class definition which represents the page where user can check his/her
 * history of records in a game.
 *
 * HistoryPage class is page that allow the user view his records
 * and compare his prerformance globally.
 *
 */

/**
 * @brief HistoryPage::HistoryPage constructor of HistoryPage class
 *
 * A constructor that set up the page with all labels and buttons
 * including reading and dsiplaying user info from the JSON file.
 * @param QWidget *widget represents the main widget holding all items
 * @param int gameNumber 1 or 2 depends on what user chose at ChooseGamePage
 * @param User* user is the user signed in
 * @param QJsonObject usersFile holds the info of the user
 */
HistoryPage::HistoryPage(QWidget *widget, int gameNumber, User* user, QJsonObject usersFile)
{
    this->gameNumber = gameNumber;
    this->widget = widget;
    this->user = user;
    this->usersFile = usersFile;

    top10ScoresL = new QLabel("Your top 10 scores are:");
    scores = new QListWidget();
    scores->setFixedSize(100,200);

    verticalLayout = new QVBoxLayout();

    back = new QPushButton("Back");

    top10Scores = new QString[10]();

    read(usersFile);
    for(int i=0;i<10;i++){
        QListWidgetItem *item = new QListWidgetItem(top10Scores[i]);
         scores->addItem(item);
    }

    topScoreL = new QLabel("Top score in the game is: \n" + topScore);
    if(user->username == topUser) {
        topUserL = new QLabel("And it is the score of: \n" + topUser+" which is you!!!");
    } else {
        topUserL = new QLabel("And it is the score of: \n" + topUser);
    }

    setVerticalLayout();
    widget->setFixedSize(500,600);
    widget->setLayout(verticalLayout);

    QObject::connect(back, SIGNAL(clicked()), SLOT(backClicked()));
}

/**
 * @brief HistoryPage::backClicked member function: takes user back
 *
 * function that respond to clicking back button by taking the user to the previous page
 */
void HistoryPage::backClicked(){
    qDeleteAll(widget->children());

    GameOnePage *chooseGamePage = new GameOnePage(widget, gameNumber, user, usersFile);
}

/**
 * @brief HistoryPage::setVerticalLayout member function: sets the vertical layout by adding items to it
 *
 */
void HistoryPage::setVerticalLayout()
{
    verticalLayout->addWidget(top10ScoresL);
    verticalLayout->addWidget(scores);
    verticalLayout->addWidget(topScoreL);
    verticalLayout->addWidget(topUserL);
    verticalLayout->addWidget(back);
    verticalLayout->addItem(new QSpacerItem(400,200));
}

/**
 * @brief HistoryPage::read member function: reads user info from JSON file
 *
 * @return bool true if read is successful
 * @param QJsonObject &json reference to the JSON file to read from
 */
bool HistoryPage::read(const QJsonObject &json)
{
    if (json.contains("games") && json["games"].isArray()) {
        QJsonArray games = json["games"].toArray();
        if (games.size() > this->gameNumber - 1 && games[this->gameNumber - 1].isObject()) {
            QJsonObject gameData = games[this->gameNumber - 1].toObject();
            if (gameData.contains("users_score") && gameData["users_score"].isArray()) {
                QJsonArray userArray = gameData["users_score"].toArray();
                for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
                    QJsonObject userObject = userArray[userIndex].toObject();
                    if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username
                            && userObject.contains("scores") && userObject["scores"].isArray()) {
                        QJsonArray userScores = userObject["scores"].toArray();
                        for (int scoreIndex = 0; scoreIndex < userScores.size(); scoreIndex++) {
                            this->top10Scores[scoreIndex] = userScores[scoreIndex].toObject().value("score").toString();
                        }
                    }
                }
            }

            if (gameData.contains("top_score") && gameData["top_score"].isObject()) {
                QJsonObject topUser = gameData["top_score"].toObject();
                if (topUser.contains("username") && topUser["username"].isString()
                        && topUser.contains("score") && topUser["score"].isString()) {
                    this->topUser = topUser["username"].toString();
                    this->topScore = topUser["score"].toString();
                }
            }

            return true;
        }
    }
    return false;
}
