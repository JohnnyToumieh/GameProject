#ifndef VIRUS_H
#define VIRUS_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include "SpongeBob.h"
#include "Aquarium.h"

class Virus : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Virus(SpongeBob *spongeBob,Aquarium *aquarium,QObject *parent = nullptr);

    int speed;

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
