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

Patient::Patient(int type, Office* office, QString motionState, QString statusState, QObject *parent)
{
    srand(QTime::currentTime().msec());

    this->office=office;

    this->justPaused = true;
    this->toDelete = false;

    motionStates["Arriving"] = Arriving;
    motionStates["Waiting"] = Waiting;
    motionStates["GettingReady"] = GettingReady;
    motionStates["Ready"] = Ready;
    motionStates["InProgress"] = InProgress;
    motionStates["Done"] = Done;
    motionStates["ReadyForAdvice"] = ReadyForAdvice;
    motionStates["ReceivingAdvice"] = ReceivingAdvice;
    motionStates["Leaving"] = Leaving;
    motionStates["Left"] = Left;

    statusStates["None"] = None;
    statusStates["Rejected"] = Rejected;
    statusStates["Accepted"] = Accepted;
    statusStates["ReallySatisfied"] = ReallySatisfied;
    statusStates["Satisfied"] = Satisfied;
    statusStates["Unsatisfied"] = Unsatisfied;

    this->motionState = motionStates[motionState.toStdString()];
    this->statusState = statusStates[statusState.toStdString()];

    if(type==1){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 1;
        this->type =1;
    }
    else if(type==2){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 2;
        this->type =1;
    }else if(type==3){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 3;
        this->type = 1;
    }
    else if(type ==4){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 1;
        this->type = 2;
    } else if(type ==5){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 2;
        this->type = 2;
    } else if(type ==6){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 3;
        this->type = 2;
    } else if(type ==7){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 1;
        this->type = 3;
    } else if(type ==8){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 2;
        this->type = 3;
    } else if(type ==9){
        imageName = ":patient1";
        QPixmap *pic  = new QPixmap(imageName);
        setPixmap(pic->scaled(100,230));
        diff = 3;
        this->type = 3;
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

QString Patient::getMotionState() {
    for (std::unordered_map<std::string, MotionState>::const_iterator it = motionStates.begin(); it != motionStates.end(); ++it) {
        if (it->second == motionState) {
            return QString::fromStdString(it->first);
        }
    }
}

QString Patient::getStatusState() {
    for (std::unordered_map<std::string, StatusState>::const_iterator it = statusStates.begin(); it != statusStates.end(); ++it) {
        if (it->second == statusState) {
            return QString::fromStdString(it->first);
        }
    }
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
    if (x() + 30 > 1000) {
        motionState = Left;
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
            if (rotation() != 0) {
                setPos(600, 350);
                setRotation(0);
            } else {
                if (x() <= 650) {
                    setX(x() + 30);
                }
            }

            if (statusState == Rejected || statusState == Unsatisfied || statusState == Satisfied) {
                motionState = Leaving;
            } else if (statusState == ReallySatisfied) {
                if (x() > 650) {
                    motionState = ReadyForAdvice;
                }
            }
        }
    }
}
