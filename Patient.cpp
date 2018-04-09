#include "Patient.h"

/**
 *\file Patient.cpp
 *@brief contains Patient class definition which represents the patient floating in the aquarium
 *
 * Patient class represents the patient object with its different attributes
 * such as type and position...
 *
 */

/**
 * @brief Patient::Patient constructor of Patient class
 *
 * A constructor that set the patient along with its attributes
 * @param int type argument integer indicating the type of the patient
 * @param SpongeBob* spongeBob is the spongebob object of the active game level
 * @param Aquarium* aquarium
 * @param QGraphicsPixmapItem* greenColorItem is the cleanliness green bar
 * @param QGraphicsPixmapItem** pixmapLifeList the array of spongebob's lives
 */

Patient::Patient(int type, Office* office, QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->type=type;
    this->office=office;

    this->justPaused = true;
    this->toDelete = false;

    this->motionState = Patient::Arriving;
    this->statusState = Patient::None;

    if(type==1){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
    }
    else if(type==2){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
    }else{
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
    }

    setPos(900, 350);

    this->speed = 300;

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

/**
 * @brief Patient::checkGameState triggered on checkGameStateTimer to check the game state and update
 * accordingly
 *
 * A function that check if the game is paused to stop moving the patient and checks for collisions
 * and accordingly modify attributes (score,cleanliness) and mark the patient with toDelete as true
 *
 */
void Patient::checkGameState() {
    // Check if game paused
    if (office->gamePaused) {
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

/**
 * @brief Patient::checkGameState triggered on speedTimer to update the place and movement of the patient
 *
 * update() function move the patient in a sinusoidal movement and delete them on borders. When spongebob
 * is vulnerable then patient chase him.
 *
 */

void Patient::update(){
    if (x() + 30 > 1100) {
        motionState == Left;
        //toDelete = true;
        speedTimer->stop();
        checkGameStateTimer->stop();
        return;
    } else {
        if (motionState == Arriving && x() <= 700) {
            motionState = Waiting;
        }

        if (motionState == Arriving) {
            setX(x() - 30);
        }

        if (motionState == Leaving) {
            setX(x() + 30);
        }

        if (motionState == GettingReady) {
            if (x() > 600) {
                setX(x() - 30);
            } else {
                setPos(300, 350);
                setRotation(-60);
                motionState = Ready;
            }
        }

        if (motionState == Done) {
            setPos(600, 350);
            setRotation(0);

            if (statusState == Rejected || statusState == Unsatisfied) {
                motionState = Leaving;
            }
        }
    }
}
