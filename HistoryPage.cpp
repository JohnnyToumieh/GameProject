#include "HistoryPage.h"
#include "GameOnePage.h"

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
    topUserL = new QLabel("And its is the score of: \n" + topUser);

    setVerticalLayout();
    widget->setFixedSize(500,350);
    widget->setLayout(verticalLayout);

    QObject::connect(back, SIGNAL(clicked()), SLOT(backClicked()));
}

void HistoryPage::backClicked(){
    qDeleteAll(widget->children());
    GameOnePage *chooseGamePage = new GameOnePage(widget, gameNumber, user, usersFile);
}

void HistoryPage::setVerticalLayout()
{
    verticalLayout->addWidget(top10ScoresL);
    verticalLayout->addWidget(scores);
    verticalLayout->addWidget(new QLabel(""));
    verticalLayout->addWidget(topScoreL);
    verticalLayout->addWidget(topUserL);
    verticalLayout->addWidget(back);
}

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
