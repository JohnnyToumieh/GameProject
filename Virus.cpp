#include "Virus.h"
#include <qmath.h>
#include <QPixmap>
Virus::Virus(int type,SpongeBob* spongeBob,Aquarium* aquarium,QObject *parent)
{
    this->spongeBob=spongeBob;
    this->aquarium=aquarium;
    this->type=type;
    this->justPaused = true;
    this->toDelete = false;

    if(type==1){
        QPixmap *pic  = new QPixmap("virus.png");
        setPixmap(pic->scaled(60,60));
        setPos(0,150);
    }else{
        QPixmap *pic  = new QPixmap("evil-virus.png");
        setPixmap(pic->scaled(60,60));
        setPos(0,150);
    }

    int random_number= (rand()%3)+1;

    if (random_number==1) {
        this->speed = 200;
    } else if (random_number==2) {
        this->speed = 250;
    } else {
        this->speed = 300;
    }

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

void Virus::checkGameState() {
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
}

void Virus::update(){
    if(!scene()->collidingItems(this).isEmpty()){
        QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
        for (int i = 0; i < collisions.size(); i++) {
            if (collisions.at(i)->hasFocus()) {
                //make SpongeBob vulnerable to all bacteria
                spongeBob->setVulnerable(type);

                //delete this Virus
                toDelete = true;
                speedTimer->stop();
                return;
            }
        }
    }

    if (x() + 30 >= 950) {
        toDelete = true;
        speedTimer->stop();
        return;
    } else {
        setPos(x()+30,100+20*qSin(x()+30));
    }
}
