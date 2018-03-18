#ifndef SPONGEBOB_H
#define SPONGEBOB_H

#include <QObject>
#include<QGraphicsRectItem>
#include<QKeyEvent>
class SpongeBob : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit SpongeBob(QGraphicsPixmapItem *needle,QObject *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);

    QGraphicsPixmapItem *needle;
    int immunityLevel;
    int immunityLevelDegree;
    int lives;


signals:

public slots:
     void changeGlow();
};

#endif // SPONGEBOB_H
