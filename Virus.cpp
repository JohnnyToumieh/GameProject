#include "Virus.h"

Virus::Virus(int type,SpongeBob* spongeBob,Aquarium* aquarium,QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->spongeBob=spongeBob;
    this->aquarium=aquarium;
    this->type=type;
    this->justPaused = true;
    this->toDelete = false;

    if (this->type == 1) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["virusSpeed1"] - 50;
    } else if (this->type == 2) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["virusSpeed2"] - 50;
    } else if (this->type == 3) {
        this->speed = this->aquarium->levels[this->aquarium->level]["virusSpeed3"];
    }

    if(type==1){
        QPixmap *pic  = new QPixmap("virus.png");
        setPixmap(pic->scaled(60,60));
    }else if (type==2){
        QPixmap *pic  = new QPixmap("evil-virus.png");
        setPixmap(pic->scaled(60,60));
    }else if (type==3){
        QPixmap *pic  = new QPixmap("pestilence.png");
        setPixmap(pic->scaled(120,120));
    }
    baseY = (rand() % 300) + 300;
    setPos(0, baseY);

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

    if(!scene()->collidingItems(this).isEmpty()){
        QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
        for (int i = 0; i < collisions.size(); i++) {
            if (collisions.at(i)->hasFocus()) {
                //make SpongeBob vulnerable to all bacteria
                spongeBob->setVulnerable(type);

                //delete this Virus
                toDelete = true;
                speedTimer->stop();
                checkGameStateTimer->stop();
                return;
            }
        }
    }
}

void Virus::update(){
    if (x() + 30 >= 950) {
        toDelete = true;
        speedTimer->stop();
        checkGameStateTimer->stop();
        return;
    } else {
        if (this->type == 3) {
            setPos(x()+30, baseY +60*qSin(x()+30));
        } else {
            setPos(x()+30, baseY +20*qSin(x()+30));
        }
    }
}
