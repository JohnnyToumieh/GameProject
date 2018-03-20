#include "Bacteria.h"
#include <QtMath>
#include <QPainter>
#include<QBrush>
#include<QWidget>
#include <QGraphicsScene>
#include <QList>
#include <QDebug>
#include "Game1Scene.h"
Bacteria::Bacteria(int type,SpongeBob *spongeBob,QGraphicsPixmapItem** pixmapLifeList,QObject *parent)
{
    this->type=type;
    this->spongeBob=spongeBob;
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

    if(random_number==1){
        this->speed = 200;
    } else if(random_number==2){
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
            this->spongeBob->collisionWithBacteria();
        } else if(this->type <= this->spongeBob->immunityLevel) {
            //increase cleanliness
            QGraphicsPixmapItem *greenColorItem= new QGraphicsPixmapItem();
            QPixmap *greenColor = new QPixmap("needle.png");
            greenColor->fill(Qt::green);
            greenColorItem->setPixmap(greenColor->scaled(80,20));
            greenColorItem->setPos(15,51);
            scene()->addItem(greenColorItem);

            //TODO: increase score
            // if cleanliness is 100
            //check is score reached next level ==> promote+reset timer
            // if low score then don't promote (make player re-do same level)

            //delete this bacteria
            scene()->removeItem(this);
            delete this;
            return;
         }
    }

    if(((x()+30) >= 950)){
       if(type==1)
            setPos(0,200);
       else if(type==2)
            setPos(0,300);
       else
            setPos(0,400);
    }
    else{
     if(type==1)
        setPos(x()+30,200+20*qSin(x()+30));
     else if(type==2)
         setPos(x()+30,300+30*qSin(x()+30));
     else
          setPos(x()+30,400+20*qSin(x()+30));
    }
}
