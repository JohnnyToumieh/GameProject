#ifndef HEALTHYITEM_H
#define HEALTHYITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

#include "SpongeBob.h"
#include "Aquarium.h"

class HealthyItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit HealthyItem(Aquarium* aquarium, SpongeBob *spongeBob,QObject *parent = nullptr);

    Aquarium* aquarium;
    SpongeBob *spongeBob;

    bool justPaused;

    QTimer* checkGameStateTimer;
    QTimer* speedTimer;

signals:

public slots:
    void update();
    void checkGameState();
};

#endif // HEALTHYITEM_H
