#include "Bacteria.h"
#include <QtMath>
#include <QPainter>
#include<QBrush>
#include<QWidget>
#include <QGraphicsScene>
#include <QList>
#include <QDebug>

Bacteria::Bacteria(int type,SpongeBob *spongeBob,Aquarium* aquarium, QGraphicsPixmapItem* greenColorItem, QGraphicsPixmapItem** pixmapLifeList,QObject *parent)
{
    this->type=type;
    this->spongeBob=spongeBob;
    this->aquarium=aquarium;
    this->greenColorItem=greenColorItem;
    this->pixmapLifeList=pixmapLifeList;

    this->justPaused = true;
    this->toDelete = false;

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
}

void Bacteria::update(){
    if(!scene()->collidingItems(this).isEmpty()){
        QList<QGraphicsItem*> collisions = scene()->collidingItems(this);
        for (int i = 0; i < collisions.size(); i++) {
            if (collisions.at(i)->hasFocus()) {
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
                    aquarium->currentCleanliness += aquarium->incrementCleanliness;
                    QPixmap *greenColor = new QPixmap("needle.png");
                    greenColor->fill(Qt::green);
                    greenColorItem->setPixmap(greenColor->scaled((230 / aquarium->maxCleanliness) * aquarium->currentCleanliness, 20));

                    //delete this bacteria
                    toDelete = true;
                    speedTimer->stop();
                    return;
                }
                break;
            }
        }
    }

    if (x() + 30 >= 950) {
        toDelete = true;
        speedTimer->stop();
        return;
    } else {
        if(spongeBob->vulnerable){
            qreal currentX = x();
            qreal currentY = y();
            qreal spongeBobX = spongeBob->x();
            qreal spongeBobY = spongeBob->y();

            int speed = 4;

            if ((int) spongeBobX != (int) currentX) {
                qreal a = (spongeBobY - currentY) / (spongeBobX - currentX);
                qreal b = currentY - a * currentX;

                qreal coAoX = pow(a, 2) + 1;
                qreal coBoX = 2 * a * (b - currentY) - 2 * currentX;
                qreal coCoX = pow(currentX, 2) + pow((b - currentY), 2) - pow(speed, 2);

                qreal newX1 = (- coBoX + sqrt(pow(coBoX, 2) - 4 * coAoX * coCoX)) / (2 * coAoX);
                qreal newX2 = (- coBoX - sqrt(pow(coBoX, 2) - 4 * coAoX * coCoX)) / (2 * coAoX);

                qreal newY1 = a * newX1 + b;
                qreal newY2 = a * newX1 + b;

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
            if (type == 1) {
                setPos(x()+30,200+20*qSin(x()+30));
            } else if (type == 2) {
                setPos(x()+30,300+30*qSin(x()+30));
            } else {
                setPos(x()+30,400+20*qSin(x()+30));
            }
        }
    }
}
