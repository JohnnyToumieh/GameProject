#ifndef GAME1SCENE_H
#define GAME1SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include "SpongeBob.h"
#include "Bacteria.h"
#include "Aquarium.h"

class Game1Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Game1Scene(QGraphicsScene *parent = nullptr);
    void gameOver(bool result);

    Aquarium *aquarium;
    SpongeBob *spongeBob;

    QLabel* timeLabel;
    QTime* time;
    QTimer* timeUpdater;

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
};

#endif // GAME1SCENE_H
