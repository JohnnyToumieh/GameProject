#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include <qmath.h>
#include <QPixmap>
#include <QTime>

#include "SpongeBob.h"
#include "Aquarium.h"

/**
 *\file Virus.h
 *@brief The Virus class, represents the moving bacteria
 *
 */


class Virus : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Virus(int type,SpongeBob *spongeBob,Aquarium *aquarium,QObject *parent = nullptr);

    int speed;//!<Integer member type (1,2 or 3) that represents type of the virus
    int type;//!<Integer member speed that represents the speed of the virus

    int baseY;//!<Integer member that represents the base Y position of the virus

    bool toDelete;//!<Boolean member that state if the virus is to be deleted from the saving list

    SpongeBob *spongeBob;//!<SpongeBob member that represents spongebob
    Aquarium* aquarium;//!<Aquarium member that represents the aquarium

    bool justPaused;//!<Boolean member that state if the game is just paused

    QTimer* checkGameStateTimer;//!<QTimer member trigger checking the game state
    QTimer* speedTimer;//!<QTimer member that trigger moving the virus


signals:

public slots:
    void update();//!<Member function updates the bactrium by moving/removing it
    void checkGameState();//!<Member function check the game state (paused,unpaused);
};

#endif // VIRUS_H
