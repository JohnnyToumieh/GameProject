#ifndef OFFICE_H
#define OFFICE_H

#include <QObject>

/**
 *\file Office.h
 *@brief The office class, represents the Office with its main indicators
 * Including cleanliness,score, and time
 *
 */

class Office : public QObject
{
    Q_OBJECT
public:
    explicit Office(int level, int currentReputation, int currentTime, int score, QObject *parent = nullptr);
    void setUpLevels();

    int level;//!<Integer member (1,2 or 3) that represents level of the game
    std::map<int, std::map<std::string, int>> levels;//!<<Map member that holds a string key and Integer value represnting defferent attributes of the level

    int currentReputation;//!<Integer member that represents the current cleanliness of the office
    int currentTime;//!<Integer member that represents the current Time

    int score;//!<Integer member that represents the score

    bool gamePaused;//!<Boolean member that state wether the game is paused or not
    bool requestForUnpause;//!<Boolean member that state wether there is a request to unpause or not

private:
    std::map<std::string, int> setUpLevelsHelper(int maxTime,
                                                 int maxReputation, int incrementReputation,
                                                 int stepsPerImmunity,
                                                 int bacteriaGenerationRate, int bacteriaWeight1, int bacteriaWeight2, int bacteriaWeight3,
                                                 int bacteriaSpeed1, int bacteriaSpeed2, int bacteriaSpeed3,
                                                 int itemDropRate, int healthyItemWeight, int unhealthyItemWeight,
                                                 int healthyItemSpeed, int unhealthyItemSpeed,
                                                 int virusGenerationRate, int virusWeight1, int virusWeight2, int virusWeight3,
                                                 int virusSpeed1, int virusSpeed2, int virusSpeed3);//!<Member function sets up different levels according to passed parameters

signals:

public slots:
};

#endif // OFFICE_H
