#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtWidgets>

#include "User.h"

/**
 *\file GameScene.h
 *@brief The GameScene class, represents the general game shell.
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QWidget* widget, User* user, QJsonObject dataFile, int gameNumber, QGraphicsScene *parent = nullptr);

    bool saveProgress();
    bool saveScore();
    void saveFile();
    QJsonDocument read(QString type);
    void backToGamePage();

    virtual void gameOver(bool result) = 0;
    virtual int getCurrentScore() = 0;

    QWidget* widget;
    User* user;//!<User member that represents the signed in user
    QJsonObject dataFile;//!QJsonObject member dataFile that holds information of the user

    int gameNumber;

private:
    virtual void saveProgressHelper(QJsonObject &userObject) const = 0;
    virtual void saveScoreHelper(QJsonObject &userObject) const = 0;

signals:

public slots:
    virtual void checkGameState() = 0;
    virtual void unpauseGame() = 0;
};

#endif // GAMESCENE_H
