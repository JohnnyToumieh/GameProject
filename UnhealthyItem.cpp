#include "UnhealthyItem.h"
#include <QTimer>
#include <QGraphicsScene>

UnhealthyItem::UnhealthyItem(Aquarium* aquarium, SpongeBob *spongeBob,QObject *parent) : QObject(parent)
{
    this->aquarium = aquarium;
    this->spongeBob = spongeBob;

    this->type = (rand()%3)+1;

    if(this->type==1){
        setPixmap((QPixmap("unhealthy1.png")).scaled(40,40));
        setPos(200,100);

    }
    else if(this->type==2){
        setPixmap((QPixmap("unhealthy2.png")).scaled(40,40));
        setPos(400,100);
    }
    else{
        setPixmap((QPixmap("unhealthy3.png")).scaled(40,40));
        setPos(600,100);
    }

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(500);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

void UnhealthyItem::checkGameState() {
    // Check if game paused
    if (aquarium->gamePaused) {
        if (justPaused) {
            speedTimer->stop();

            justPaused = false;
        }

        return;
    } else {
        if (!justPaused) {
            speedTimer->start(500);

            justPaused = true;
        }
    }
}

void UnhealthyItem::update(){
    if(!(scene()->collidingItems(this).isEmpty())&& scene()->collidingItems(this).at(0)->hasFocus()){
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
        scene()->removeItem(this);
        delete this;
        return;
    }

    if((y()+30) >= 500) {
        scene()->removeItem(this);
        delete this;
        return;
    }
    else
        setPos(x(),y()+30);
}
