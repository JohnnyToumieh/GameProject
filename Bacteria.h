#ifndef BACTERIA_H
#define BACTERIA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include "SpongeBob.h"
#include "Aquarium.h"

class Bacteria : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Bacteria(int type,SpongeBob *spongeBob, Aquarium* aquarium, QGraphicsPixmapItem* greenColorItem, QGraphicsPixmapItem** pixmapLifeList,QObject *parent = nullptr);

    int type;
    int speed;

    int baseY;

    bool toDelete;

    SpongeBob *spongeBob;
    Aquarium* aquarium;

    bool justPaused;

    QTimer* checkGameStateTimer;
    QTimer* speedTimer;

    QGraphicsPixmapItem* greenColorItem;
    QGraphicsPixmapItem** pixmapLifeList;
signals:

public slots:
    void update();
    void checkGameState();
};

#endif // BACTERIA_H
