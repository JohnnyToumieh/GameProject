#ifndef SPONGEBOB_H
#define SPONGEBOB_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include<QKeyEvent>
#include <QTimer>

#include "Aquarium.h"

/**
 *\file SpongeBob.h
 *@brief The SpongeBob class, represents spongebob
 *
 */

class SpongeBob : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpongeBob(Aquarium* aquarium, QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent = nullptr);

    void keyPressEvent(QKeyEvent *event);//!<Member function moves spomgebob
    void collisionWithBacteria(int bacteriaType);//!<Member function handles collision with bacteria
    void setVulnerable(int type);//!<Member function make spongebob vulnerable to all bacteria
    void reset();//!<Member function that reset spongebob

    Aquarium* aquarium;//!<Aquarium member that represents the aquarium

    QGraphicsPixmapItem** pixmapLifeList;//!<QGraphicsPixmapItem member that represents the heart shapes (lives of spongebob).
    QGraphicsPixmapItem *needle;//!<QGraphicsPixmapItem member that represents the needle of the immunity meter

    int immunityLevel;//!<Integer member indicates the immunity level
    int savedImmunityLevel;//!<Integer used to save immunity level
    int immunityLevelDegree;//!<Integer memer type indicates the immunity level degree
    int savedImmunityLevelDegree;//!<Integer member save immunity level degree
    int lives;//!<Integer member indicates lives of sponegbob

    bool unchangeableImmunityLevel;//!<Boolean member that state if immunity level is changeable
    bool vulnerable;//!<Boolean member that state if spongebob is vulnerable
    bool canCollide;//!<Boolean member that state if spongebob can Collide
    bool blinkerStatus;//!<Boolean member that state if spongebob should be blinking

    QTimer* collisionTimer;//!<QTimer member triggers making spongebob can collide
    QTimer* collisionBlinker;//!<QTimer member triggers making spongebob can blinking
    QTimer* vulnerableTimer;//!<QTimer member triggers making spongebob can vulnerable

    int* numCollisionsWithBacterias;//!<Integer array hold spongebob collisions

    QString imageName;

signals:

public slots:
     void changeGlow();//!<Member function updates glow color of spongebob
     void setCanCollide();//!<Member function make sponge able to collide
     void toggleVisibility();//!<Member function toggles visibility
     void resetVulnerability();//!<Member function that resets Vulnerability
};

#endif // SPONGEBOB_H
