#ifndef SPONGEBOB_H
#define SPONGEBOB_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include<QKeyEvent>
class SpongeBob : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpongeBob(QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void collisionWithBacteria();

    QGraphicsPixmapItem** pixmapLifeList;
    QGraphicsPixmapItem *needle;
    int immunityLevel;
    int immunityLevelDegree;
    int lives;

    bool canCollide;
    QTimer* collisionTimer;
    bool blinkerStatus;
    QTimer* collisionBlinker;

signals:

public slots:
     void changeGlow();
     void setCanCollide();
     void toggleVisibility();
};

#endif // SPONGEBOB_H
