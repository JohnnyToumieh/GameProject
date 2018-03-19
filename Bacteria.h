#ifndef BACTERIA_H
#define BACTERIA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include "SpongeBob.h"

class Bacteria : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Bacteria(int type,SpongeBob *spongeBob,QGraphicsPixmapItem** pixmapLifeList,QObject *parent = nullptr);
    int type;
    SpongeBob *spongeBob;
    QGraphicsPixmapItem** pixmapLifeList;
signals:

public slots:
    void update();
    void updateLives();
};

#endif // BACTERIA_H