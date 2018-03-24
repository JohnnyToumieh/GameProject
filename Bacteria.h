#ifndef BACTERIA_H
#define BACTERIA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include <QtMath>
#include <QPainter>
#include <QBrush>
#include <QWidget>
#include <QGraphicsScene>
#include <QList>
#include <QDebug>
#include <QTime>
#include "SpongeBob.h"
#include "Aquarium.h"

/**
 *\file Bacteria.h
 *@brief The Bacteria class, represents the moving bacteria
 *
 */

class Bacteria : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Bacteria(int type,SpongeBob *spongeBob, Aquarium* aquarium, QGraphicsPixmapItem* greenColorItem, QGraphicsPixmapItem** pixmapLifeList,QObject *parent = nullptr);

    int type;//!<Integer member type (1,2 or 3) that represents type of the bacteria
    int speed;//!<Integer member speed that represents the speed of the bacteria

    int baseY;//!<Integer member that represents the base Y position of the bacteria

    bool toDelete;//!<Boolean member that state if the bacteria is to be deleted from the saving list

    SpongeBob *spongeBob;//!<SpongeBob member that represents spongebob
    Aquarium* aquarium;//!<Aquarium member that represents the aquarium

    bool justPaused;//!<Boolean member that state if the game is just paused

    QTimer* checkGameStateTimer;//!<QTimer member trigger checking the game state
    QTimer* speedTimer;//!<QTimer member that trigger moving the bacteria

    QGraphicsPixmapItem* greenColorItem;//!<QGraphicsPixmapItem member that represents the green color filling the cleanliness bar
    QGraphicsPixmapItem** pixmapLifeList;//!<QGraphicsPixmapItem member that represents the hearts (lives) of spongebob
signals:

public slots:
    void update();//!<Member function updates the bactrium by moving/removing it
    void checkGameState();//!<Member function check the game state (paused,unpaused)
};

#endif // BACTERIA_H
