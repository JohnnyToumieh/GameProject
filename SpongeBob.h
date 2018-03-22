#ifndef SPONGEBOB_H
#define SPONGEBOB_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include<QKeyEvent>

#include "Aquarium.h"

class SpongeBob : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpongeBob(Aquarium* aquarium, QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void collisionWithBacteria(int bacteriaType);
    void setVulnerable(int type);
    void reset();

    Aquarium* aquarium;

    QGraphicsPixmapItem** pixmapLifeList;
    QGraphicsPixmapItem *needle;
    int immunityLevel;
    int savedImmunityLevel;
    bool unchangeableImmunityLevel;
    int immunityLevelDegree;
    int savedImmunityLevelDegree;
    int lives;

    bool vulnerable;
    bool canCollide;
    QTimer* collisionTimer;
    bool blinkerStatus;
    QTimer* collisionBlinker;
    QTimer* vulnerableTimer;

    int* numCollisionsWithBacterias;

signals:

public slots:
     void changeGlow();
     void setCanCollide();
     void toggleVisibility();
     void resetVulnerability();
};

#endif // SPONGEBOB_H
