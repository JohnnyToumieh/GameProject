#include "Office.h"
/**
 *\file Office.cpp
 *@brief contains Office class definition which represents office as an object holding main attributes of the game
 *
 * Office class set the level,cleanliness,time and score.
 * Also it sets up each level with a specific attributes that define speed and frequency of object making an
 * overall difficulty of each level (increasing from 1 to 3).
 *
 */

/**
 * @brief Office::Office constructor of Office class
 *
 * A constructor that set different parameters (level,cleanliness,time and score)
 * @param level argument integer indicating the level
 * @param currentCleanliness argument integer indicating the Cleanliness
 * @param currentTime argument integer indicating the time of the game timer
 * @param score argument integer indicating the score
 */
Office::Office(int level, int currentReputation, int currentTime, int score, QObject *parent) : QObject(parent)
{
    this->level = level;

    setUpLevels();

    this->currentReputation = currentReputation;
    this->currentTime = currentTime;

    this->score = score;

    this->gamePaused = false;
    this->requestForUnpause = false;
}

/**
 * @brief Office::setUpLevels member function: sets up the levels
 *
 * function that fill the array of the levels with 3 levels each with specified attributes.
 */
void Office::setUpLevels() {
    this->levels[1] = setUpLevelsHelper(300000,
                                          10, 1,
                                          4,
                                          3000, 33, 33, 33, 200, 200, 200,
                                          3000, 50, 50, 400, 400,
                                          0, 0, 0, 0, 0, 0, 0);

    this->levels[2] = setUpLevelsHelper(300000,
                                          20, 1,
                                          8,
                                          2500, 33, 33, 33, 200, 200, 200,
                                          2000, 50, 50, 300, 400,
                                          15000, 100, 0, 0, 200, 0, 0);

    this->levels[3] = setUpLevelsHelper(300000,
                                          30, 1,
                                          8,
                                          2000, 33, 33, 33, 150, 150, 150,
                                          2000, 50, 50, 300, 300,
                                          10000, 50, 30, 5, 200, 200, 50);
}

/**
 * @brief Office::setUpLevelsHelper member function: help setting up the levels
 *
 * function take diffrenet attributes of the level and set them in a map of string key describing the attribute and integer value
 * equal to the passed parameters
 * @param maxTime integer that indicates the maximum time allowed to complete the level
 * @param maxCleanliness ineteger that indicates the cleanliness needed to be reached in order to complete the level
 * @param incrementCleanliness integer indicating how much the cleanliness will increment when a bacterium is killed
 * @param stepsPerImmunity integer indicates how much step it takes to go from one immunity to another
 * @param bacteriaGenerationRate integer indicates the rate at which are bacteria being generated
 * @param bacteriaWeight1 integer indicates the weight of bacteria 1
 * @param bacteriaWeight2 integer indicates the weight of bacteria 2
 * @param bacteriaWeight3 integer indicates the weight of bacteria 3
 * @param bacteriaSpeed1 integer indicate the timer at which the bacteria 1 move (its like a period so the actual "speed" is inversly proprotional to it).
 * @param bacteriaSpeed2 integer indicate the timer at which the bacteria 2 move (its like a period so the actual "speed" is inversly proprotional to it).
 * @param bacteriaSpeed3 integer indicate the timer at which the bacteria 3 move (its like a period so the actual "speed" is inversly proprotional to it).
 * @param itemDropRate integer indicates the rate at which are items being generated
 * @param healthyItemWeight integer indicates the weight of healthy items
 * @param unhealthyItemWeight integer indicates the weight of unhealthy items
 * @param healthyItemSpeed integer indicates the speed of healthy items
 * @param unhealthyItemSpeed integer indicates the speed of unhealthy items
 * @param virusGenerationRate integer indicates the rate at which are viruses being generated
 * @param virusWeight1 integer indicates the weight of virus 1
 * @param virusWeight2 integer indicates the weight of virus 2
 * @param virusWeight3 integer indicates the weight of virus 3
 * @param virusSpeed1  integer indicates the speed of virus 1
 * @param virusSpeed2  integer indicates the speed of virus 2
 * @param virusSpeed3  integer indicates the speed of virus 3
 * @return std::map<std::string, int> a map between attributes discriptions and values
 */
std::map<std::string, int> Office::setUpLevelsHelper(int maxTime,
                                     int maxReputation, int incrementReputation,
                                     int stepsPerImmunity,
                                     int bacteriaGenerationRate, int bacteriaWeight1, int bacteriaWeight2, int bacteriaWeight3,
                                     int bacteriaSpeed1, int bacteriaSpeed2, int bacteriaSpeed3,
                                     int itemDropRate, int healthyItemWeight, int unhealthyItemWeight,
                                     int healthyItemSpeed, int unhealthyItemSpeed,
                                     int virusGenerationRate, int virusWeight1, int virusWeight2, int virusWeight3,
                                     int virusSpeed1, int virusSpeed2, int virusSpeed3) {
    std::map<std::string, int> level;

    level["maxTime"] = maxTime;

    level["maxCleanliness"] = maxReputation;
    level["incrementCleanliness"] = incrementReputation;

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
