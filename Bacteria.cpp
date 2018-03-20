#include "Bacteria.h"
#include <QtMath>
#include <QPainter>
#include<QBrush>
#include<QWidget>
#include <QGraphicsScene>
#include <QList>
#include <QDebug>

Bacteria::Bacteria(int type,SpongeBob *spongeBob,Aquarium* aquarium, QGraphicsPixmapItem* greenColorItem, QGraphicsPixmapItem** pixmapLifeList,QObject *parent)
{
    this->type=type;
    this->spongeBob=spongeBob;
    this->aquarium=aquarium;
    this->greenColorItem=greenColorItem;
    this->pixmapLifeList=pixmapLifeList;

    if(type==1){
        QPixmap *pic  = new QPixmap("bacteria1.png");
        setPixmap(pic->scaled(80,80));
        setPos(0,200);
    }
    else if(type==2){
        QPixmap *pic  = new QPixmap("bacteria2.png");
        setPixmap(pic->scaled(80,80));
        setPos(0,300);
    }else{
        QPixmap *pic  = new QPixmap("bacteria3.png");
        setPixmap(pic->scaled(80,80));
        setPos(0,400);
    }

    int random_number= (rand()%3)+1;

    if (random_number==1) {
        this->speed = 200;
    } else if (random_number==2) {
        this->speed = 250;
    } else {
        this->speed = 300;
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(this->speed);
}

void Bacteria::update(){
    if(!(scene()->collidingItems(this).isEmpty())&& scene()->collidingItems(this).at(0)->hasFocus()){
        if (this->type > this->spongeBob->immunityLevel && this->spongeBob->canCollide) {
            //decrease score
            aquarium->score -= type * 50;

            this->spongeBob->collisionWithBacteria(this->type);
        } else if(this->type <= this->spongeBob->immunityLevel) {
            //increase score
            aquarium->score += type * 100;

            //increase cleanliness
            aquarium->currentCleanliness += aquarium->incrementCleanliness;
            QPixmap *greenColor = new QPixmap("needle.png");
            greenColor->fill(Qt::green);
            greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->maxCleanliness) * aquarium->currentCleanliness, 20));

            //delete this bacteria
            scene()->removeItem(this);
            delete this;
            return;
         }
    }

    if (x() + 30 >= 950) {
        scene()->removeItem(this);
        delete this;
        return;
    } else {
        if (type == 1) {
            setPos(x()+30,200+20*qSin(x()+30));
        } else if (type == 2) {
            setPos(x()+30,300+30*qSin(x()+30));
        } else {
            setPos(x()+30,400+20*qSin(x()+30));
        }
    }
}
