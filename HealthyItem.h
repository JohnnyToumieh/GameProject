#ifndef HEALTHYITEM_H
#define HEALTHYITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "SpongeBob.h"

class HealthyItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit HealthyItem(SpongeBob *spongeBob,QObject *parent = nullptr);
     SpongeBob *spongeBob;

signals:

public slots:
    void update();
};

#endif // HEALTHYITEM_H
