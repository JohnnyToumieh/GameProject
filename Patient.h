#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPaintDevice>
#include <QTimer>
#include <QPainter>
#include <QGraphicsScene>
#include <QtMath>
#include <QPainter>
#include <QBrush>
#include <QWidget>
#include <QGraphicsScene>
#include <QList>
#include <QDebug>
#include <QTime>
#include "Office.h"
#include <unordered_map>

/**
 *\file Patient.h
 *@brief The Patient class, represents the moving patient
 *
 */

class Patient : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum MotionState {Arriving, Waiting, GettingReady, Ready, InProgress, Done, ReadyForAdvice, ReceivingAdvice, Leaving, Left};
    enum StatusState {None, Rejected, Accepted, ReallySatisfied, Satisfied, Unsatisfied};

    explicit Patient(int type, Office* office, QString motionState = "Arriving", QString statusState = "None", QObject *parent = nullptr);

    QString getMotionState();
    QString getStatusState();

    int type;//!<Integer member type (1,2 or 3) that represents type of the patient
    int speed;//!<Integer member speed that represents the speed of the patient
    int diff;

    std::unordered_map<std::string, MotionState> motionStates;
    std::unordered_map<std::string, StatusState> statusStates;

    MotionState motionState;
    StatusState statusState;

    QString imageName;

    bool toDelete;//!<Boolean member that state if the patient is to be deleted from the saving list

    Office* office;

    bool justPaused;//!<Boolean member that state if the game is just paused

    QTimer* checkGameStateTimer;//!<QTimer member trigger checking the game state
    QTimer* speedTimer;//!<QTimer member that trigger moving the patient

signals:

public slots:
    void update();//!<Member function updates the bactrium by moving/removing it
    void checkGameState();//!<Member function check the game state (paused,unpaused)
};

#endif // PATIENT_H
