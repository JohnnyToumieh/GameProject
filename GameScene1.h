#ifndef GAMESCENE1_H
#define GAMESCENE1_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTime>
#include <QtWidgets>

#include "SpongeBob.h"
#include "Bacteria.h"
#include "Aquarium.h"
#include "Item.h"
#include "Virus.h"
#include "GameScene.h"

/**
 *\file GameScene1.h
 *@brief The GameScene1 class, represents the game 1 shell.
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

class GameScene1 : public GameScene
{
    Q_OBJECT
public:
    explicit GameScene1(QWidget* widget, User* user, QJsonObject dataFile, bool resume = false, int level = 1);

    void setUpNextLevel();

    void gameOver(bool result);
    int getCurrentScore();

    Aquarium* aquarium;//!<Aquarium member that represents the aquarium
    SpongeBob *spongeBob;//!<SpongeBob member that represents spongebob

    Bacteria** bacterias;//!<Bacteria array member that represents the list of currently visibile bacterias
    int bacteriasIndex;//!<Int member that indexes the bacterias array

    Item** items;//!<Item array member that represents the list of currently visibile items
    int itemsIndex;//!<Int member that indexes the items array

    Virus** viruses;//!<Virus array member that represents the list of currently visibile viruses
    int virusesIndex;//!<Int member that indexes the viruses array

    QLabel* timeLabel;
    QLabel* pestilenceTimeLabel;
    QLabel* pestilenceTimeLabel2;
    QLabel* scoreLabel2;
    QLabel* gameOverLabel;
    QLabel* unpauseLabel;

    QTime* time;//!<QTime member that counts the current time elapsed
    int pausedTime;//!<int member that help save the time before the game was paused to use after the game unpauses

    QTimer* timeUpdater;//!<QTimer member that updates all QLabels that keeps track of time
    int pausedTimeUpdater;//!<int member helper to save the QTimer's time left when the game pauses
    QTimer* updateItemsTimer;//!<QTimer member that creates a new item
    int pausedUpdateItemsTimer;//!<int member helper to save the QTimer's time left when the game pauses
    QTimer* updateBacteriasTimer;//!<QTimer member that creates a new bacteria
    int pausedUpdateBacteriasTimer;//!<int member helper to save the QTimer's time left when the game pauses
    QTimer* virusTimer;//!<QTimer member that creates a new virus
    int pausedVirusTimer;//!<int member helper to save the QTimer's time left when the game pauses
    QTimer* pestilenceTimer;//!<QTimer member that counts down the time until the Pestilence shows up
    int pausedPestilenceTimer;//!<int member helper to save the QTimer's time left when the game pauses

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

    QGraphicsPixmapItem *greenColorItem;
    QGraphicsPixmapItem *pixmapNeedle;
    QGraphicsPixmapItem *pixmapLife1;
    QGraphicsPixmapItem *pixmapLife2;
    QGraphicsPixmapItem *pixmapLife3;

    QGraphicsPixmapItem** pixmapLifeList;

private:
    void saveProgressHelper(QJsonObject &userObject) const;
    void saveScoreHelper(QJsonObject &userObject) const;

signals:

public slots:
    void updateItems();
    void updateBacterias();
    void updateTimer();
    void unpauseClicked();//!<Member function that triggers when the unpause button is clicked
    void quitClicked();//!<Member function that triggers when the quit button is clicked
    void nextLevel();//!<Member function that triggers when the next level button is clicked
    void virusUpdate();
    void summonPestilence();
    void unpauseGame();
    void checkGameState();
};

#endif // GAMESCENE1_H
