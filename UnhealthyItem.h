#ifndef UNHEALTHYITEM_H
#define UNHEALTHYITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "SpongeBob.h"
class UnhealthyItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit UnhealthyItem(SpongeBob *spongeBob,QObject *parent = nullptr);
     SpongeBob *spongeBob;

signals:

public slots:
    void update();
};

#endif // UNHEALTHYITEM_H
