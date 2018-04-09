#ifndef OFFICE_H
#define OFFICE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

/**
 *\file Office.h
 *@brief The office class, represents the Office with its main indicators
 * Including cleanliness,score, and time
 *
 */

class Office : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Office(int level, int currentReputation, int currentTime, int currentAquariumState, int score, QObject *parent = nullptr);
    void setUpLevels();

    void keyPressEvent(QKeyEvent *event);

    int level;//!<Integer member (1,2 or 3) that represents level of the game
    std::map<int, std::map<std::string, int>> levels;//!<<Map member that holds a string key and Integer value represnting defferent attributes of the level

    int currentReputation;//!<Integer member that represents the current cleanliness of the office
    int currentTime;//!<Integer member that represents the current Time
    int currentAquariumState;

    int score;//!<Integer member that represents the score

    int currentMiniGameScore;

    bool gamePaused;//!<Boolean member that state wether the game is paused or not
    bool requestForUnpause;//!<Boolean member that state wether there is a request to unpause or not
    bool inAMiniGame;

private:
    std::map<std::string, int> setUpLevelsHelper(int startTime, int endTime, int minuteInMilliSeconds,
                                                 int maxReputation, int incrementReputation,
                                                 int patientGenerationRate, int patientWeight1, int patientWeight2, int patientWeight3,
                                                 int dirtinessRate);//!<Member function sets up different levels according to passed parameters

signals:

public slots:
};

#endif // OFFICE_H
