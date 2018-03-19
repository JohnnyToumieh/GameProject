#include "UnhealthyItem.h"
#include <QTimer>
#include <QGraphicsScene>

UnhealthyItem::UnhealthyItem(SpongeBob *spongeBob,QObject *parent) : QObject(parent)
{
    this->spongeBob=spongeBob;
    int random_number= (rand()%3)+1;

    if(random_number==1){
        setPixmap((QPixmap("unhealthy1.png")).scaled(40,40));
        setPos(200,100);

    }
    else if(random_number==2){
        setPixmap((QPixmap("unhealthy2.png")).scaled(40,40));
        setPos(400,100);
    }
    else{
        setPixmap((QPixmap("unhealthy3.png")).scaled(40,40));
        setPos(600,100);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(500);
}

void UnhealthyItem::update(){
    if(!(scene()->collidingItems(this).isEmpty())&& scene()->collidingItems(this).at(0)->hasFocus()){
        scene()->removeItem(this);
        delete this;
        int degree=spongeBob->immunityLevelDegree;
        if(!(degree==1 && spongeBob->immunityLevel==1)){
            degree=spongeBob->immunityLevelDegree--;
            spongeBob->needle->setTransformOriginPoint(spongeBob->needle->boundingRect().center().x()+20,
                                                       spongeBob->needle->boundingRect().center().y());
            spongeBob->needle->setRotation(spongeBob->needle->rotation()-8);
        }

        if((degree==1 && spongeBob->immunityLevel==2) ||
           (degree==1 && spongeBob->immunityLevel==3)){
           spongeBob->immunityLevelDegree=8;

           spongeBob->immunityLevel--;
        }
    }
    if((y()+30) >= 500) {
        scene()->removeItem(this);
        delete this;
    }
    else
        setPos(x(),y()+30);
}
