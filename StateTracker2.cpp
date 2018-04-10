#include "StateTracker2.h"
/**
 *\file StateTracker2.cpp
 *@brief contains StateTracker2 class definition which represents the state tracker as an object holding main attributes of the game
 *
 * StateTracker2 class set the level,cleanliness,time and score.
 * Also it sets up each level with a specific attributes that define speed and frequency of object making an
 * overall difficulty of each level (increasing from 1 to 3).
 *
 */

/**
 * @brief StateTracker2::StateTracker2 constructor of Aquarium class
 *
 * A constructor that set different parameters (level,cleanliness,time and score)
 * @param level argument integer indicating the level
 * @param currentCleanliness argument integer indicating the Cleanliness
 * @param currentTime argument integer indicating the time of the game timer
 * @param score argument integer indicating the score
 */
StateTracker2::StateTracker2(int level, int currentTime, int score, QObject *parent) : QObject(parent)
{
    this->level = level;

    setUpLevels();

    this->currentTime = currentTime;

    this->score = score;

    this->gamePaused = false;
    this->requestForUnpause = false;
}

/**
 * @brief StateTracker2::setUpLevels member function: sets up the levels
 *
 * function that fill the array of the levels with 3 levels each with specified attributes.
 */
void StateTracker2::setUpLevels() {
    this->levels[1] = setUpLevelsHelper(300000);

    this->levels[2] = setUpLevelsHelper(300000);

    this->levels[3] = setUpLevelsHelper(300000);
}

/**
 * @brief StateTracker2::setUpLevelsHelper member function: help setting up the levels
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
std::map<std::string, int> StateTracker2::setUpLevelsHelper(int maxTime) {
    std::map<std::string, int> level;

    level["maxTime"] = maxTime;

    level["levelState"] = 0;

    return level;
}
