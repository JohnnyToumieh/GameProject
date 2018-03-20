#include "SpongeBob.h"
#include <QTimer>
SpongeBob::SpongeBob(QGraphicsPixmapItem *needle, QGraphicsPixmapItem** pixmapLifeList, QObject *parent) : QObject(parent)
{
    this->pixmapLifeList=pixmapLifeList;

    setPixmap((QPixmap("bob1.png")).scaled(80,80));
    setPos(500,100);

    this->needle=needle;
    this->immunityLevel=1;
    this->immunityLevelDegree=1;
    this->lives=3;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeGlow()));
    timer->start(500);
}

void SpongeBob::changeGlow(){
    if(immunityLevel==1){
        setPixmap((QPixmap("bob1.png")).scaled(80,80));
    }
    if(immunityLevel==2){
        setPixmap((QPixmap("bob2.png")).scaled(80,80));
    }
    if(immunityLevel==3){
        setPixmap((QPixmap("bob3.png")).scaled(80,80));

    }

}

void SpongeBob::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Right && x()+10 < 930)
        setPos(x()+10,y());
    if (event->key() == Qt::Key_Left && x()-10 > -30)
        setPos(x()-10,y());
    if (event->key() == Qt::Key_Up && y()-10 > 0)
        setPos(x(),y()-10);
    if (event->key() == Qt::Key_Down && y()+10 < 550)
        setPos(x(),y()+10);
}

void SpongeBob::collisionWithBacteria(){
    // Update lives
    if(lives > 0) {
        lives = lives - 1;
        if(lives == 2) {
            scene()->removeItem(pixmapLifeList[0]);
        } else if(lives == 1) {
            scene()->removeItem(pixmapLifeList[1]);
        } else {
            scene()->removeItem(pixmapLifeList[2]);
        }
    }
}
