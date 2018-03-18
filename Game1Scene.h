#ifndef GAME1SCENE_H
#define GAME1SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include "SpongeBob.h"
#include "Bacteria.h"

class Game1Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Game1Scene(QGraphicsScene *parent = nullptr);
    SpongeBob *spongeBob;

    Bacteria *bacteria;

    QGraphicsPixmapItem *pixmapNeedle;
    QGraphicsPixmapItem *pixmapLife1;
    QGraphicsPixmapItem *pixmapLife2;
    QGraphicsPixmapItem *pixmapLife3;

    QGraphicsPixmapItem** pixmapLifeList;

signals:

public slots:
    void update();
};

#endif // GAME1SCENE_H
