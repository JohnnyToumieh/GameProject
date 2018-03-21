#include "HealthyItem.h"
#include <QTimer>
#include <QGraphicsScene>
HealthyItem::HealthyItem(Aquarium* aquarium, SpongeBob *spongeBob,QObject *parent)
{
    this->aquarium = aquarium;
    this->spongeBob = spongeBob;

    this->justPaused = true;
    this->toDelete = false;

    this->type = (rand()%3)+1;

    if(this->type==1){
        setPixmap((QPixmap("healthy1.png")).scaled(60,30));
        setPos(200,100);

    }
    else if(this->type==2){
        setPixmap((QPixmap("healthy2.png")).scaled(40,40));
        setPos(400,100);
    }
    else{
        setPixmap((QPixmap("healthy3.png")).scaled(30,60));
        setPos(600,100);
    }

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(500);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

void HealthyItem::checkGameState() {
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

void HealthyItem::update(){
    if(!(scene()->collidingItems(this).isEmpty())&& scene()->collidingItems(this).at(0)->hasFocus()){
        int degree=spongeBob->immunityLevelDegree;
        if(!(degree>=6 && spongeBob->immunityLevel==3)){
            degree=spongeBob->immunityLevelDegree++;
            spongeBob->needle->setTransformOriginPoint(spongeBob->needle->boundingRect().center().x()+20,
                                                       spongeBob->needle->boundingRect().center().y());
            spongeBob->needle->setRotation(spongeBob->needle->rotation()+8);
        }

        if((degree>=6 && spongeBob->immunityLevel==1) ||
           (degree>=9 && spongeBob->immunityLevel==2)){
           spongeBob->immunityLevelDegree=1;

           spongeBob->immunityLevel++;
        }
        scene()->removeItem(this);
        speedTimer->stop();
        toDelete = true;
        return;
    }

    if((y()+30) >= 500) {
        scene()->removeItem(this);
        speedTimer->stop();
        toDelete = true;
        return;
    }
    else
        setPos(x(),y()+30);
}
