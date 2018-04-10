#ifndef GAMESCENE2_H
#define GAMESCENE2_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTime>
#include <QtWidgets>

#include "StateTracker2.h"
#include "GameScene.h"

/**
 *\file GameScene2.h
 *@brief The GameScene2 class, represents the game 2 shell.
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

class GameScene2 : public GameScene
{
    Q_OBJECT
public:
    enum State {NotStarted, DisplayingTeeth, GuessingTeeth, GameWon, GameLost};

    explicit GameScene2(QWidget* widget, int width, int height, User* user, QJsonObject dataFile,
                        bool resume = false, int level = 1, int difficulity = 1, bool isMiniGame = false);

    void setUpNextLevel();

    void gameOver(bool result);
    int getCurrentScore();
    int getLevelState();

    void resetAllTeeth();

    State gameState;

    QWidget* widget;
    User* user;//!<User member that represents the signed in user
    QJsonObject dataFile;//!QJsonObject member dataFile that holds information of the user

    QLabel** upperTeeth;
    int upperTeethIndex;

    QLabel** lowerTeeth;
    int lowerTeethIndex;

    QGraphicsPixmapItem* mouth;
    QLabel* eyes;
    QMovie* eyesAnimation;

    QPushButton* start;
    QLabel* goLabel;

    int* order;
    bool* guessedOrder;
    int orderIndex;
    int orderSize;
    int orderSpeed;

    QTimer* toothUpdater;
    QTimer* teethUpdater;

    StateTracker2* stateTracker2;//!<stateTracker2 member that represents the aquarium

    QLabel* timeLabel;
    QLabel* scoreLabel2;
    QLabel* gameOverLabel;
    QLabel* unpauseLabel;

    QTime* time;//!<QTime member that counts the current time elapsed
    int pausedTime;//!<int member that help save the time before the game was paused to use after the game unpauses

    QTimer* timeUpdater;//!<QTimer member that updates all QLabels that keeps track of time
    int pausedTimeUpdater;//!<int member helper to save the QTimer's time left when the game pauses

    QTimer* unpauseTimer;//!<QTimer member that counts down the time until the game can be interactable again

    QTimer* checkGameStateTimer;//!<QTimer member that updates the game state

    bool justPaused;

    QWidget* greyForeground;

    QPushButton* unpause;
    QPushButton* quit;
    QPushButton* nextLevelButton;
    QPushButton* quit2;

    QLabel* levelLabel;
    QLabel* scoreLabel;

    QGraphicsPixmapItem *pixmapLife1;
    QGraphicsPixmapItem *pixmapLife2;
    QGraphicsPixmapItem *pixmapLife3;

    QGraphicsPixmapItem** pixmapLifeList;

private:
    void saveProgressHelper(QJsonObject &userObject) const;
    void saveScoreHelper(QJsonObject &userObject) const;

signals:

public slots:
    void startClicked();
    void highlightAllTeeth();
    void dehighlightAllTeeth();
    void highlightTooth();
    void updateTimer();
    void unpauseClicked();//!<Member function that triggers when the unpause button is clicked
    void quitClicked();//!<Member function that triggers when the quit button is clicked
    void nextLevel();//!<Member function that triggers when the next level button is clicked
    void unpauseGame();
    void checkGameState();
};

#endif // GAMESCENE2_H
