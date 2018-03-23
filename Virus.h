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

class Virus : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Virus(int type,SpongeBob *spongeBob,Aquarium *aquarium,QObject *parent = nullptr);

    int speed;
    int type;

    int baseY;

    bool toDelete;

    SpongeBob *spongeBob;
    Aquarium* aquarium;

    bool justPaused;

    QTimer* checkGameStateTimer;
    QTimer* speedTimer;


signals:

public slots:
    void update();
    void checkGameState();
};

#endif // VIRUS_H
