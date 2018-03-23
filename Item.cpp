#include "Item.h"

Item::Item(Aquarium* aquarium, SpongeBob *spongeBob, bool isHealthy, int type, QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->aquarium = aquarium;
    this->spongeBob = spongeBob;

    this->justPaused = true;
    this->toDelete = false;

    this->type = type;
    this->isHealthy = isHealthy;

    if (this->type == -1) {
        this->type = (rand()%3)+1;
    }

    if(this->type==1){
        if (this->isHealthy) {
            setPixmap((QPixmap("healthy1.png")).scaled(60,30));
        } else {
            setPixmap((QPixmap("unhealthy1.png")).scaled(40,40));
        }
    }
    else if(this->type==2){
        if (this->isHealthy) {
            setPixmap((QPixmap("healthy2.png")).scaled(40,40));
        } else {
            setPixmap((QPixmap("unhealthy2.png")).scaled(40,40));
        }
    }
    else{
        if (this->isHealthy) {
            setPixmap((QPixmap("healthy3.png")).scaled(30,60));
        } else {
            setPixmap((QPixmap("unhealthy3.png")).scaled(40,40));
        }
    }
    setPos((rand() % 800) + 100, 100);

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(500);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

void Item::checkGameState() {
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

    if(!scene()->collidingItems(this).isEmpty()){
        QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
        for (int i = 0; i < collisions.size(); i++) {
            if (collisions.at(i)->hasFocus()) {
                int degree=spongeBob->immunityLevelDegree;

                if (isHealthy && spongeBob->unchangeableImmunityLevel == false) {
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
                } else if (!isHealthy && spongeBob->unchangeableImmunityLevel == false) {
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
                speedTimer->stop();
                checkGameStateTimer->stop();
                toDelete = true;
                return;
            }
        }
    }
}

void Item::update(){
    if((y()+30) >= 500) {
        speedTimer->stop();
        checkGameStateTimer->stop();
        toDelete = true;
        return;
    }
    else
        setPos(x(),y()+30);
}
