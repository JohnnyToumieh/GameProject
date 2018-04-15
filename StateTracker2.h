#ifndef STATETRACKER2_H
#define STATETRACKER2_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

/**
 *\file StateTracker2.h
 *@brief The StateTracker2 class, represents the state tracker of game 2 with its main indicators
 *
 */

class StateTracker2 : public QObject,  public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit StateTracker2(int level, int difficulity, int currentTime,
                           int score, int points, int specialPoints,
                           int timeLimit, int specialTimeLimit,
                           QObject *parent = nullptr);
    void setUpLevels();

    void keyPressEvent(QKeyEvent *event);

    int level;//!<Integer member (1,2 or 3) that represents level of the game
    int difficulity;
    std::map<int, std::map<std::string, int>> levels;//!<<Map member that holds a string key and Integer value represnting defferent attributes of the level

    int currentTime;//!<Integer member that represents the current Time

    int score;//!<Integer member that represents the score

    int points;
    int specialPoints;

    int timeLimit;
    int specialTimeLimit;

    int width;
    int height;

    bool gamePaused;//!<Boolean member that state wether the game is paused or not
    bool requestForUnpause;//!<Boolean member that state wether there is a request to unpause or not

private:
    std::map<std::string, int> setUpLevelsHelper(int maxTime,
                                                 int difficulity1NumberOfTeeth, int difficulity1SpeedOfTeeth,
                                                 int difficulity2NumberOfTeeth, int difficulity2SpeedOfTeeth,
                                                 int difficulity3NumberOfTeeth, int difficulity3SpeedOfTeeth);//!<Member function sets up different levels according to passed parameters

signals:

public slots:
};

#endif // STATETRACKER2_H
