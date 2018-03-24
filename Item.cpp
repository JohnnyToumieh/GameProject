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

    if (this->isHealthy) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["healthyItemSpeed"] - 50;
    } else if (!this->isHealthy) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["unhealthyItemSpeed"] - 50;
    }

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

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
            speedTimer->start(this->speed);

            justPaused = true;
        }
    }

    if(!scene()->collidingItems(this).isEmpty()){
        QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
        for (int i = 0; i < collisions.size(); i++) {
            if (collisions.at(i)->hasFocus()) {
                int degree=spongeBob->immunityLevelDegree;

                int steps = aquarium->levels[aquarium->level]["stepsPerImmunity"];
                if (isHealthy && spongeBob->unchangeableImmunityLevel == false) {
                    if(!(degree > steps && spongeBob->immunityLevel==3)){
                        degree=spongeBob->immunityLevelDegree++;
                        if (spongeBob->immunityLevel == 2) {
                            spongeBob->needle->setRotation(spongeBob->needle->rotation() + 80 / steps);
                        } else {
                            spongeBob->needle->setRotation(spongeBob->needle->rotation() + 48 / steps);
                        }
                    }

                    if(degree >= steps && (spongeBob->immunityLevel==1 || spongeBob->immunityLevel==2)){
                       spongeBob->immunityLevelDegree = 1;

                       spongeBob->immunityLevel++;
                    }
                } else if (!isHealthy && spongeBob->unchangeableImmunityLevel == false) {
                    if(!(degree==1 && spongeBob->immunityLevel==1)){
                        degree=spongeBob->immunityLevelDegree--;
                        if ((spongeBob->immunityLevel == 2 && degree > 1) || (spongeBob->immunityLevel == 3 && degree == 1)) {
                            spongeBob->needle->setRotation(spongeBob->needle->rotation() - 80 / steps);
                        } else {
                            spongeBob->needle->setRotation(spongeBob->needle->rotation() - 48 / steps);
                        }
                    }

                    if(degree==1 && (spongeBob->immunityLevel==2 || spongeBob->immunityLevel==3)){
                       spongeBob->immunityLevelDegree = steps;

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
