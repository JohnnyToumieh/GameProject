#include "Aquarium.h"

Aquarium::Aquarium(int level, int currentCleanliness, int currentTime, int score, QObject *parent) : QObject(parent)
{
    this->level = level;

    setUpLevels();

    this->currentCleanliness = currentCleanliness;
    this->currentTime = currentTime;

    this->score = score;

    this->gamePaused = false;
    this->requestForUnpause = false;
}

void Aquarium::setUpLevels() {
    this->levels[1] = setUpLevelsHelper(300000,
                                          10, 1,
                                          2,
                                          3000, 33, 33, 33, 500, 500, 500,
                                          3000, 50, 50, 500, 500,
                                          0, 0, 0, 0, 0, 0, 0);

    this->levels[2] = setUpLevelsHelper(300000,
                                          20, 1,
                                          8,
                                          2000, 33, 33, 33, 400, 400, 400,
                                          3000, 50, 50, 400, 500,
                                          15000, 100, 0, 0, 200, 0, 0);

    this->levels[3] = setUpLevelsHelper(300000,
                                          30, 1,
                                          8,
                                          2000, 33, 33, 33, 300, 300, 300,
                                          3000, 50, 50, 300, 500,
                                          10000, 50, 30, 5, 200, 200, 50);
}

std::map<std::string, int> Aquarium::setUpLevelsHelper(int maxTime,
                                     int maxCleanliness, int incrementCleanliness,
                                     int stepsPerImmunity,
                                     int bacteriaGenerationRate, int bacteriaWeight1, int bacteriaWeight2, int bacteriaWeight3,
                                     int bacteriaSpeed1, int bacteriaSpeed2, int bacteriaSpeed3,
                                     int itemDropRate, int healthyItemWeight, int unhealthyItemWeight,
                                     int healthyItemSpeed, int unhealthyItemSpeed,
                                     int virusGenerationRate, int virusWeight1, int virusWeight2, int virusWeight3,
                                     int virusSpeed1, int virusSpeed2, int virusSpeed3) {
    std::map<std::string, int> level;

    level["maxTime"] = maxTime;

    level["maxCleanliness"] = maxCleanliness;
    level["incrementCleanliness"] = incrementCleanliness;

    level["stepsPerImmunity"] = stepsPerImmunity;

    level["bacteriaGenerationRate"] = bacteriaGenerationRate;
    level["bacteriaWeight1"] = bacteriaWeight1;
    level["bacteriaWeight2"] = bacteriaWeight2;
    level["bacteriaWeight3"] = bacteriaWeight3;
    level["bacteriaSpeed1"] = bacteriaSpeed1;
    level["bacteriaSpeed2"] = bacteriaSpeed2;
    level["bacteriaSpeed3"] = bacteriaSpeed3;

    level["itemDropRate"] = itemDropRate;
    level["healthyItemWeight"] = healthyItemWeight;
    level["unhealthyItemWeight"] = unhealthyItemWeight;
    level["healthyItemSpeed"] = healthyItemSpeed;
    level["unhealthyItemSpeed"] = unhealthyItemSpeed;

    level["virusGenerationRate"] = virusGenerationRate;
    level["virusWeight1"] = virusWeight1;
    level["virusWeight2"] = virusWeight2;
    level["virusWeight3"] = virusWeight3;
    level["virusSpeed1"] = virusSpeed1;
    level["virusSpeed2"] = virusSpeed2;
    level["virusSpeed3"] = virusSpeed3;

    return level;
}
