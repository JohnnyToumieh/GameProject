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
#include "HomePage.h"

class Game1Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Game1Scene(QGraphicsScene *parent = nullptr);
    void gameOver(bool result);

    Aquarium *aquarium;
    Bacteria** bacterias;
    int bacteriasIndex;
    SpongeBob *spongeBob;

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
