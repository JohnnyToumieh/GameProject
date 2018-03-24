#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <QObject>

/**
 *\file Aquarium.h
 *@brief The aquarium class, represents the Aquarium with its main indicators
 * Including cleanliness,score, and time
 *
 */

class Aquarium : public QObject
{
    Q_OBJECT
public:
    explicit Aquarium(int level, int currentCleanliness, int currentTime, int score, QObject *parent = nullptr);
    void setUpLevels();

    int level;//!<Integer member (1,2 or 3) that represents level of the game
    std::map<int, std::map<std::string, int>> levels;//!<<Map member that holds a string key and Integer value represnting defferent attributes of the level

    int currentCleanliness;//!<Integer member that represents the current cleanliness of the aquarium
    int currentTime;//!<Integer member that represents the current Time

    int score;//!<Integer member that represents the score

    bool gamePaused;//!<Boolean member that state wether the game is paused or not
    bool requestForUnpause;

private:
    std::map<std::string, int> setUpLevelsHelper(int maxTime,
                                                 int maxCleanliness, int incrementCleanliness,
                                                 int stepsPerImmunity,
                                                 int bacteriaGenerationRate, int bacteriaWeight1, int bacteriaWeight2, int bacteriaWeight3,
                                                 int bacteriaSpeed1, int bacteriaSpeed2, int bacteriaSpeed3,
                                                 int itemDropRate, int healthyItemWeight, int unhealthyItemWeight,
                                                 int healthyItemSpeed, int unhealthyItemSpeed,
                                                 int virusGenerationRate, int virusWeight1, int virusWeight2, int virusWeight3,
                                                 int virusSpeed1, int virusSpeed2, int virusSpeed3);

signals:

public slots:
};

#endif // AQUARIUM_H
