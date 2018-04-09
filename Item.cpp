#include "Item.h"
/**
 *\file Item.cpp
 *@brief contains Item class definition which represents the healthy/unhealthy items floating in the aquarium
 *
 * Item class represents the items (healthy/unhealthy) objects
 *
 */

/**
 * @brief Item::Item constructor of Item class
 *
 * A constructor that set the Item along with its attributes
 * it select one of the three healthy/unhealthy items to create
 * and randomly set the speed and position.
 * @param SpongeBob* spongeBob is the spongebob object of the active game level
 * @param Aquarium* aquarium
 * @param int type argument integer indicating the type of the bacteria
 * @param QObject *parent by default is null
 */
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

    int size1 = this->aquarium->width / 25;
    int size21 = this->aquarium->width / 16.67;
    int size22 = this->aquarium->height / 20;
    int size31 = this->aquarium->width / 33.33;
    int size32 = this->aquarium->height / 10;

    if(this->type==1){
        if (this->isHealthy) {
            imageName = ":healthyItem1";
            setPixmap((QPixmap(imageName)).scaled(size21, size22));
        } else {
            imageName = ":unhealthyItem1";
            setPixmap((QPixmap(imageName)).scaled(size1, size1));
        }
    }
    else if(this->type==2){
        if (this->isHealthy) {
            imageName = ":healthyItem2";
            setPixmap((QPixmap(imageName)).scaled(size1, size1));
        } else {
            imageName = ":unhealthyItem2";
            setPixmap((QPixmap(imageName)).scaled(size1, size1));
        }
    }
    else{
        if (this->isHealthy) {
            imageName = ":healthyItem3";
            setPixmap((QPixmap(imageName)).scaled(size31, size32));
        } else {
            imageName = ":unhealthyItem3";
            setPixmap((QPixmap(imageName)).scaled(size1, size1));
        }
    }
    setPos((rand() % (this->aquarium->width * 8 / 10)) + this->aquarium->width / 10, this->aquarium->height / 6);

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

/**
 * @brief Item::checkGameState triggered on checkGameStateTimer to check the game state and update
 * accordingly
 * A function that check if the game is paused to stop moving the item and checks for collisions
 * and accordingly modify attributes (immnuity level) and mark the item with toDelete as true
 * it also modify the needle position of the immnunity meter.
 *
 */
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

/**
 * @brief Item::update triggered on speedTimer to update the place and movement of the items
 *
 * update() function move the item from top and going down and delete them on at the end of the screen.
 *
 */
void Item::update(){
    if(y() + 30 >= this->aquarium->height * 11 / 12) {
        speedTimer->stop();
        checkGameStateTimer->stop();
        toDelete = true;
        return;
    }
    else
        setPos(x(), y()+30);
}
