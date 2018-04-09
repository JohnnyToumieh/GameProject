#ifndef GAMESCENE3_H
#define GAMESCENE3_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTime>
#include <QtWidgets>

#include "Office.h"
#include "Patient.h"
#include "GameScene.h"

/**
 *\file GameScene3.h
 *@brief The GameScene3 class, represents the game 2 shell.
 * It handles saving of games, the creation and deletion of elements and tracking the game state.
 *
 */

class GameScene3 : public GameScene
{
    Q_OBJECT
public:
    explicit GameScene3(QWidget* widget, User* user, QJsonObject dataFile, bool resume = false, int level = 1);

    void setUpNextLevel();

    void gameOver(bool result);
    int getCurrentScore();

    Office* office;//!<Office member that represents the office

    Patient** patients;//!<Patient array member that represents the list of currently visibile patients
    int patientsIndex;//!<Int member that indexes the patients array

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
    QTimer* updatePatientsTimer;//!<QTimer member that creates a new patient
    int pausedUpdatePatientsTimer;//!<int member helper to save the QTimer's time left when the game pause

    QTimer* unpauseTimer;//!<QTimer member that counts down the time until the game can be interactable again

    QTimer* checkGameStateTimer;//!<QTimer member that updates the game state

    bool justPaused;

    QWidget* greyForeground;
    QWidget* patientBox;

    QPushButton* unpause;
    QPushButton* quit;
    QPushButton* nextLevelButton;
    QPushButton* quit2;
    QPushButton* accept;
    QPushButton* reject;

    QLabel* levelLabel;
    QLabel* scoreLabel;
    QLabel* description;

    QGraphicsPixmapItem *greenColorItem;
    QGraphicsPixmapItem *pixmapNeedle;
    QGraphicsPixmapItem *pixmapLife1;
    QGraphicsPixmapItem *pixmapLife2;
    QGraphicsPixmapItem *pixmapLife3;
    QGraphicsPixmapItem *aquarium;

    QGraphicsPixmapItem** pixmapLifeList;

private:
    void saveProgressHelper(QJsonObject &userObject) const;
    void saveScoreHelper(QJsonObject &userObject) const;

signals:

public slots:
    void updatePatients();
    void updateTimer();
    void unpauseClicked();//!<Member function that triggers when the unpause button is clicked
    void quitClicked();//!<Member function that triggers when the quit button is clicked
    void nextLevel();//!<Member function that triggers when the next level button is clicked
    void unpauseGame();
    void checkGameState();
    void handlePatient(int status);
};

#endif // GAMESCENE3_H
