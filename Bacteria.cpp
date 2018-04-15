#include "Bacteria.h"

/**
 *\file Bacteria.cpp
 *@brief contains Bacteria class definition which represents the bacteria floating in the aquarium
 *
 * Bacteria class represents the bacteria object with its different attributes
 * such as type and position...
 *
 */

/**
 * @brief Bacteria::Bacteria constructor of Bacteria class
 *
 * A constructor that set the bacteria along with its attributes
 * @param int type argument integer indicating the type of the bacteria
 * @param SpongeBob* spongeBob is the spongebob object of the active game level
 * @param Aquarium* aquarium
 * @param QGraphicsPixmapItem* greenColorItem is the cleanliness green bar
 * @param QGraphicsPixmapItem** pixmapLifeList the array of spongebob's lives
 */

Bacteria::Bacteria(int type,SpongeBob *spongeBob,Aquarium* aquarium, QGraphicsPixmapItem* greenColorItem, QGraphicsPixmapItem** pixmapLifeList,QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->type=type;
    this->spongeBob=spongeBob;
    this->aquarium=aquarium;
    this->greenColorItem=greenColorItem;
    this->pixmapLifeList=pixmapLifeList;

    this->justPaused = true;
    this->toDelete = false;

    int size = this->aquarium->width / 12.5;

    if(type==1){
        imageName = ":bacteria1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(size, size));
    }
    else if(type==2){
        imageName = ":bacteria2";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(size, size));
    }else{
        imageName = ":bacteria3";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(size, size));
    }

    baseY = (rand() % (this->aquarium->height / 2)) + this->aquarium->height * 3 / 12;
    setPos(0, baseY);

    if (this->type == 1) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["bacteriaSpeed1"] - 50;
    } else if (this->type == 2) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["bacteriaSpeed2"] - 50;
    } else if (this->type == 3) {
        this->speed = (rand() % 100) + this->aquarium->levels[this->aquarium->level]["bacteriaSpeed3"] - 50;
    }

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

/**
 * @brief Bacteria::checkGameState triggered on checkGameStateTimer to check the game state and update
 * accordingly
 *
 * A function that check if the game is paused to stop moving the bacteria and checks for collisions
 * and accordingly modify attributes (score,cleanliness) and mark the bacteria with toDelete as true
 *
 */
void Bacteria::checkGameState() {
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
            if (dynamic_cast<SpongeBob*>(collisions.at(i))) {
                if (this->type > this->spongeBob->immunityLevel && this->spongeBob->canCollide) {
                    //decrease score
                    if (aquarium->score <= type * 50) {
                        aquarium->score = 0;
                    } else {
                        aquarium->score -= type * 50;
                    }

                    this->spongeBob->collisionWithBacteria(this->type);
                } else if(this->type <= this->spongeBob->immunityLevel) {
                    //increase score
                    aquarium->score += type * 100;

                    //increase cleanliness
                    aquarium->currentCleanliness += aquarium->levels[aquarium->level]["incrementCleanliness"];
                    QPixmap *greenColor = new QPixmap(":needle");
                    greenColor->fill(Qt::green);
                    greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->levels[aquarium->level]["maxCleanliness"]) * aquarium->currentCleanliness, 20));

                    //delete this bacteria
                    toDelete = true;
                    speedTimer->stop();
                    checkGameStateTimer->stop();
                    return;
                }
                break;
            }
        }
    }
}

/**
 * @brief Bacteria::checkGameState triggered on speedTimer to update the place and movement of the bacteria
 *
 * update() function move the bacteria in a sinusoidal movement and delete them on borders. When spongebob
 * is vulnerable then bacteria chase him.
 *
 */

void Bacteria::update(){
    if (x() + 30 >= aquarium->width) {
        toDelete = true;
        speedTimer->stop();
        checkGameStateTimer->stop();
        return;
    } else {
        if(spongeBob->vulnerable){
            qreal currentX = x();
            qreal currentY = y();
            qreal spongeBobX = spongeBob->x();
            qreal spongeBobY = spongeBob->y();

            int speed = 4;

            if (spongeBobX != currentX) {
                qreal a = (spongeBobY - currentY) / (spongeBobX - currentX);
                qreal b = currentY - a * currentX;

                qreal coAoX = pow(a, 2) + 1;
                qreal coBoX = 2 * a * (b - currentY) - 2 * currentX;
                qreal coCoX = pow(currentX, 2) + pow((b - currentY), 2) - pow(speed, 2);

                qreal newX1 = (- coBoX + sqrt(pow(coBoX, 2) - 4 * coAoX * coCoX)) / (2 * coAoX);
                qreal newX2 = (- coBoX - sqrt(pow(coBoX, 2) - 4 * coAoX * coCoX)) / (2 * coAoX);

                qreal newY1 = a * newX1 + b;
                qreal newY2 = a * newX2 + b;

                qreal dist1 = sqrt(pow((newX1 - spongeBobX), 2) + pow((newY1 - spongeBobY), 2));
                qreal dist2 = sqrt(pow((newX2 - spongeBobX), 2) + pow((newY2 - spongeBobY), 2));
                qreal dist = sqrt(pow((currentX - spongeBobX), 2) + pow((currentY - spongeBobY), 2));

                setPos((dist1 < dist2) ? ((dist < dist1) ? spongeBobX : newX1) : ((dist < dist2) ? spongeBobX : newX2),
                       (dist1 < dist2) ? ((dist < dist1) ? spongeBobY : newY1) : ((dist < dist2) ? spongeBobY : newY2));
            } else {
                qreal newY1 = currentY + speed;
                qreal newY2 = currentY - speed;

                qreal dist1 = sqrt(pow((newY1 - spongeBobY), 2));
                qreal dist2 = sqrt(pow((newY2 - spongeBobY), 2));
                qreal dist = sqrt(pow((currentY - spongeBobY), 2));

                setPos(currentX,
                       (dist1 < dist2) ? ((dist < dist1) ? spongeBobY : newY1) : ((dist < dist2) ? spongeBobY : newY2));
            }
        } else {
            setPos(x()+30, baseY +20*qSin(x()+30));
        }
    }
}
