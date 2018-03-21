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
#include "HealthyItem.h"
#include "UnhealthyItem.h"

class Game1Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Game1Scene(QWidget* widget, User* user, QJsonObject usersFile, bool resume = false, QGraphicsScene *parent = nullptr);
    void gameOver(bool result);
    bool saveProgress();
    QJsonObject read(QString type);

    QWidget* widget;
    User* user;
    QJsonObject usersFile;

    Aquarium *aquarium;
    SpongeBob *spongeBob;

    Bacteria** bacterias;
    int bacteriasIndex;

    HealthyItem** healthyItems;
    int healthyItemsIndex;

    UnhealthyItem** unhealthyItems;
    int unhealthyItemsIndex;

    QLabel* timeLabel;

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

signals:

public slots:
    void updateItems();
    void updateBacterias();
    void updateTimer();
    void checkGameState();
    void unpauseClicked();
    void quitClicked();
};

#endif // GAME1SCENE_H
