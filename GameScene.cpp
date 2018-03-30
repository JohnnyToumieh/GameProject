#include "GameScene.h"

#include "Message.h"
#include "GamePage.h"

/**
 *\file GameScene.cpp
 *@brief contains GameScene class definition which represents the general game shell.
 *
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

/**
 * @brief GameScene::GameScene constructor of GameScene class
 *
 * A constructor that sets up the game with all initial attributes.
 * @param QWidget *widget represents the main widget holding all items
 * @param User* user is the user signed in
 * @param QJsonObject dataFile holds the info of the user
 * @param bool resume determines is the game is being resumed
 * @param int level determines if the game should start at a specific level
 */
GameScene::GameScene(QWidget *widget, User* user, QJsonObject dataFile, int gameNumber, QGraphicsScene *parent) : QGraphicsScene(parent)
{
    this->widget = widget;
    this->user = user;
    this->dataFile = dataFile;

    this->gameNumber = gameNumber;
}

/**
 * @brief GameScene::backToGamePage
 *
 * A member function that exists the game and goes back to the GamePage.
 */
void GameScene::backToGamePage() {
    views()[0]->close();
    GamePage *gamePage = new GamePage(widget, gameNumber, user, dataFile);
    widget->show();
}

/**
 * @brief GameScene::saveFile
 *
 * A member function that saves the data of dataFile to disk.
 */
void GameScene::saveFile() {
    QFile saveFile(QStringLiteral("Data.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        Message *msg = new Message("Couldn't open data file to save.");
        msg->show();
    }

    QJsonDocument saveDoc(dataFile);
    saveFile.write(saveDoc.toJson());
}

/**
 * @brief GameScene::read
 *
 * A member function that reads a specific part of the game saved.
 *
 * @param QString type that specifies the part of the saved progress to load.
 *
 * @return QJsonDocument the saved progress part needed.
 */
QJsonDocument GameScene::read(QString type) {
    QJsonObject save;
    if (dataFile.contains("games") && dataFile["games"].isArray()) {
        QJsonArray games = dataFile["games"].toArray();
        if (games.size() > gameNumber - 1 && games.at(gameNumber - 1).isObject()) {
            QJsonObject gameData = games.at(gameNumber - 1).toObject();
            if (gameData.contains("users_save") && gameData["users_save"].isArray()) {
                QJsonArray userArray = gameData["users_save"].toArray();

                // If save for user already created, overwrite it
                for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
                    QJsonObject userObject = userArray[userIndex].toObject();
                    if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
                        save = userObject["save"].toObject();
                        if (save[type].isObject()) {
                            return (QJsonDocument) save[type].toObject();
                        } else if (save[type].isArray()) {
                            return (QJsonDocument) save[type].toArray();
                        }
                    }
                }
            }
        }
    }
    return (QJsonDocument) save;
}

/**
 * @brief GameScene::saveProgress
 *
 * A member function that saves the current game progress.
 *
 * @return bool whether the saving of the game state was successful.
 */
bool GameScene::saveProgress() {
    QJsonArray games;
    if (dataFile.contains("games") && dataFile["games"].isArray()) {
        games = dataFile["games"].toArray();
    }

    QJsonObject gameData;
    if (games.size() > gameNumber - 1 && games.at(gameNumber - 1).isObject()) {
        gameData = games.at(gameNumber - 1).toObject();
    }

    QJsonArray userArray;
    if (gameData.contains("users_save") && gameData["users_save"].isArray()) {
        userArray = gameData["users_save"].toArray();
    }

    // If save for user already created, overwrite it
    for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
        QJsonObject userObject = userArray[userIndex].toObject();
        if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
            QJsonObject saveObject;
            saveProgressHelper(saveObject);
            userObject["save"] = saveObject;
            userArray[userIndex] = userObject;
            gameData["users_save"] = userArray;
            games.replace(gameNumber - 1, gameData);
            dataFile["games"] = games;
            return true;
        }
    }

    // If save for user never created before, create it
    QJsonObject userObject;
    QJsonObject saveObject;
    saveProgressHelper(saveObject);
    userObject["save"] = saveObject;
    userObject["username"] = this->user->username;
    userArray.append(userObject);
    gameData["users_save"] = userArray;
    if (games.size() > gameNumber - 1) {
        games.replace(gameNumber - 1, gameData);
    } else {
        games.insert(gameNumber - 1, gameData);
    }
    dataFile["games"] = games;
    return true;
}

/**
 * @brief GameScene::saveScore
 *
 * A member function that saves the current score of the user and updates the top score.
 *
 * @return bool whether the saving operation was successful.
 */
bool GameScene::saveScore() {
    QJsonArray games;
    if (dataFile.contains("games") && dataFile["games"].isArray()) {
        games = dataFile["games"].toArray();
    }

    QJsonObject gameData;
    if (games.size() > gameNumber - 1 && games.at(gameNumber - 1).isObject()) {
        gameData = games.at(gameNumber - 1).toObject();
    }

    if (gameData.contains("top_score") && gameData["top_score"].isObject()) {
        QJsonObject userObject = gameData["top_score"].toObject();
        if (userObject.contains("score") && userObject["score"].toInt() < getCurrentScore()) {
            userObject["score"] = getCurrentScore();
            userObject["username"] = user->username;
            gameData["top_score"] = userObject;
            games.replace(gameNumber - 1, gameData);
            dataFile["games"] = games;
        }
    } else {
        QJsonObject userObject;
        userObject["score"] = getCurrentScore();
        userObject["username"] = user->username;
        gameData["top_score"] = userObject;
        if (games.size() > gameNumber - 1) {
            games.replace(gameNumber - 1, gameData);
        } else {
            games.insert(gameNumber - 1, gameData);
        }
        dataFile["games"] = games;
    }

    QJsonArray userArray;
    if (gameData.contains("users_score") && gameData["users_score"].isArray()) {
        userArray = gameData["users_score"].toArray();
    }

    // If score for user already created, add another
    for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
        QJsonObject userObject = userArray[userIndex].toObject();
        if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->user->username) {
            QJsonObject saveObject;
            saveScoreHelper(saveObject);
            QJsonArray scoresArray;
            if (userObject.contains("scores") && userObject["scores"].isArray()) {
                scoresArray = userObject["scores"].toArray();
            }
            scoresArray.append(saveObject);
            userObject["scores"] = scoresArray;
            userArray[userIndex] = userObject;
            gameData["users_score"] = userArray;
            games.replace(gameNumber - 1, gameData);
            dataFile["games"] = games;
            return true;
        }
    }

    // If score for user never created before, create it
    QJsonObject userObject;
    QJsonObject saveObject;
    saveScoreHelper(saveObject);
    QJsonArray scoresArray;
    if (userObject.contains("scores") && userObject["scores"].isArray()) {
        scoresArray = userObject["scores"].toArray();
    }
    scoresArray.append(saveObject);
    userObject["scores"] = scoresArray;
    userObject["username"] = this->user->username;
    userArray.append(userObject);
    gameData["users_score"] = userArray;
    if (games.size() > gameNumber - 1) {
        games.replace(gameNumber - 1, gameData);
    } else {
        games.insert(gameNumber - 1, gameData);
    }
    dataFile["games"] = games;
    return true;
}
