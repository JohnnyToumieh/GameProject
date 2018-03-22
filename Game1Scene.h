#ifndef GAME1SCENE_H
#define GAME1SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTime>
#include <QtWidgets>

#include "SpongeBob.h"
#include "Bacteria.h"
#include "Aquarium.h"
#include "GameOnePage.h"
#include "Item.h"

class Game1Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Game1Scene(QWidget* widget, User* user, QJsonObject usersFile, bool resume = false, int level = 1, QGraphicsScene *parent = nullptr);
    void gameOver(bool result);
    bool saveProgress();
    bool saveScore();
    void saveFile();
    QJsonDocument read(QString type);
    void setUpNextLevel();

    QWidget* widget;
    User* user;
    QJsonObject usersFile;

    Aquarium *aquarium;
    SpongeBob *spongeBob;

    Bacteria** bacterias;
    int bacteriasIndex;

    Item** items;
    int itemsIndex;

    QLabel* timeLabel;
    QLabel* scoreLabel2;
    QLabel* gameOverLabel;

    QTime* time;
    int pausedTime;

    QTimer* timeUpdater;
    int pausedTimeUpdater;
    QTimer* updateItemsTimer;
    int pausedUpdateItemsTimer;
    QTimer* updateBacteriasTimer;
    int pausedUpdateBacteriasTimer;
    QTimer* checkGameStateTimer;

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
    void checkGameState();
    void unpauseClicked();
    void quitClicked();
    void nextLevel();
};

#endif // GAME1SCENE_H
