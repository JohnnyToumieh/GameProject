#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QGraphicsScene>
#include <QTime>

#include "SpongeBob.h"
#include "Aquarium.h"

class Item : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Item(Aquarium* aquarium, SpongeBob *spongeBob,bool isHealthy = true, int type = -1,QObject *parent = nullptr);

    bool isHealthy;
    int type;
    int speed;

    bool toDelete;

    Aquarium* aquarium;
    SpongeBob *spongeBob;

    bool justPaused;

    QTimer* checkGameStateTimer;
    QTimer* speedTimer;

signals:

public slots:
    void update();
    void checkGameState();
};

#endif // ITEM_H
