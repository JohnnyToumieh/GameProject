#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QGraphicsScene>
#include <QTime>

#include "SpongeBob.h"
#include "Aquarium.h"
/**
 *\file item.h
 *@brief The item class, represents the moving item (healthy and unhealthy)
 *
 */

class Item : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Item(Aquarium* aquarium, SpongeBob *spongeBob,bool isHealthy = true, int type = -1,QObject *parent = nullptr);

    bool isHealthy;//!<Boolean member that state if item is healthy or not
    int type;//!<Integer member type (1,2 or 3) that represents type of the bacteria
    int speed;//!<Integer member speed that represents the speed of the bacteria

    bool toDelete;//!<Boolean member that state if the bacteria is to be deleted from the saving list

    SpongeBob *spongeBob;//!<SpongeBob member that represents spongebob
    Aquarium* aquarium;//!<Aquarium member that represents the aquarium

    bool justPaused;//!<Boolean member that state if the game is just paused

    QTimer* checkGameStateTimer;//!<QTimer member trigger checking the game state
    QTimer* speedTimer;//!<QTimer member that trigger moving the item

signals:

public slots:
    void update();//!<Member function updates the item by moving/removing it
    void checkGameState();//!<Member function check the game state (paused,unpaused)
};

#endif // ITEM_H
