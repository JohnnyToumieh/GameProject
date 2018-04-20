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
 * @param office* spongeBob is the spongebob object of the active game level
 * @param motionState state of the patient (arriving,leaving...)
 * @param statusState state of the patient's status (satisfied,...)
 */

Patient::Patient(int type, Office* office,
                 QString motionState, QString statusState,
                 QString imageName, QObject *parent)
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

    int randomImage = (rand() % 6);

    if (imageName != NULL) {
        this->imageName = imageName;
    } else {
        this->imageName = ":patient" + QString::number(randomImage + (type / 3) * 3 + 1);
    }

    if (type == 1 || type == 2 || type == 3) {
        this->type = 1;
        this->diff = type;

        width = 140;
        height = 310;

        QPixmap *pic  = new QPixmap(this->imageName);
        setPixmap(pic->scaled(width, height));

        setPos(900, 280);
    } else if (type == 4 || type == 5 || type == 6) {
        this->type = 2;
        this->diff = type - 3;

        width = 120;
        height = 290;

        QPixmap *pic  = new QPixmap(this->imageName);
        setPixmap(pic->scaled(width, height));

        setPos(900, 300);
    } else if (type == 7 || type == 8 || type == 9) {
        this->type = 3;
        this->diff = type - 6;

        width = 100;
        height = 230;

        QPixmap *pic  = new QPixmap(this->imageName);
        setPixmap(pic->scaled(width, height));

        setPos(900, 360);
    }

    this->speed = 300;

    speedTimer = new QTimer(this);
    connect(speedTimer, SIGNAL(timeout()), this, SLOT(update()));
    speedTimer->start(this->speed);

    checkGameStateTimer = new QTimer(this);
    connect(checkGameStateTimer, SIGNAL(timeout()), this, SLOT(checkGameState()));
    checkGameStateTimer->start(100);
}

/**
 * @brief Patient::getMotionState
 *
 * A member function that returns the motion state of the patient
 * @return QString the motion state of the patient
 */
QString Patient::getMotionState() {
    for (std::unordered_map<std::string, MotionState>::const_iterator it = motionStates.begin(); it != motionStates.end(); ++it) {
        if (it->second == motionState) {
            return QString::fromStdString(it->first);
        }
    }
    return NULL;
}

/**
 * @brief Patient::getStatusState
 *
 * A member function that returns the status of the patient
 * @return QString the status of the patient
 */
QString Patient::getStatusState() {
    for (std::unordered_map<std::string, StatusState>::const_iterator it = statusStates.begin(); it != statusStates.end(); ++it) {
        if (it->second == statusState) {
            return QString::fromStdString(it->first);
        }
    }
    return NULL;
}

/**
 * @brief Patient::checkGameState triggered on checkGameStateTimer to check the game state and update
 * accordingly
 *
 * A function that check if the game is paused to stop moving the patient
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
 * update() function move the patient in and out of the office
 *
 */

void Patient::update(){
    if ((!transform().isScaling() && x() + 30 > 1000) || (transform().isScaling() && x() + 30 - width > 1000)) {
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
            if (!transform().isScaling()) {
                setTransform(QTransform::fromScale(-1, 1));
                setX(x() + width);
            }

            setX(x() + 30);
        }

        if (motionState == GettingReady) {
            if (x() > 600) {
                setX(x() - 30);
            } else {
                setPos(250, y() - 40);
                setRotation(60);

                setTransform(QTransform::fromScale(-1, 1));
                setX(x() + width);

                motionState = Ready;
            }
        }

        if (motionState == Done) {
            if (rotation() != 0) {
                setPos(600, y() + 40);
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
