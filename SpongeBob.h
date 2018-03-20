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
    void updateLives();

    QGraphicsPixmapItem** pixmapLifeList;
    QGraphicsPixmapItem *needle;
    int immunityLevel;
    int immunityLevelDegree;
    int lives;


signals:

public slots:
     void changeGlow();
};

#endif // SPONGEBOB_H
