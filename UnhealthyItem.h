#ifndef UNHEALTHYITEM_H
#define UNHEALTHYITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

#include "SpongeBob.h"
#include "Aquarium.h"

class UnhealthyItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit UnhealthyItem(Aquarium* aquarium, SpongeBob *spongeBob,QObject *parent = nullptr);

    Aquarium* aquarium;
    SpongeBob *spongeBob;

signals:

public slots:
    void update();
};

#endif // UNHEALTHYITEM_H
