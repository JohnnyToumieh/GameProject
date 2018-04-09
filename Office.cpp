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
Office::Office(int level, int currentReputation, int currentTime, int currentAquariumState, int score, QObject *parent) : QObject(parent)
{
    this->level = level;

    setUpLevels();

    this->currentReputation = currentReputation;
    this->currentTime = currentTime;
    this->currentAquariumState = currentAquariumState;

    this->score = score;

    this->currentMiniGameScore = 0;

    this->gamePaused = false;
    this->requestForUnpause = false;
    this->inAMiniGame = false;
}

void Office::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape) {
        if (gamePaused && !requestForUnpause) {
            requestForUnpause = true;
        } else if (gamePaused && requestForUnpause) {
            requestForUnpause = false;
        } else {
            gamePaused = !gamePaused;
        }
    }
}

/**
 * @brief Office::setUpLevels member function: sets up the levels
 *
 * function that fill the array of the levels with 3 levels each with specified attributes.
 */
void Office::setUpLevels() {
    this->levels[1] = setUpLevelsHelper(480, 840, 1000,
                                          10, 1,
                                          3000, 50, 30, 20,
                                          5000);
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
std::map<std::string, int> Office::setUpLevelsHelper(int startTime, int endTime, int minuteInMilliSeconds,
                                     int maxReputation, int incrementReputation,
                                     int patientGenerationRate, int patientWeight1, int patientWeight2, int patientWeight3,
                                     int dirtinessRate) {
    std::map<std::string, int> level;

    level["startTime"] = startTime;
    level["endTime"] = endTime;
    level["minuteInMilliSeconds"] = minuteInMilliSeconds;

    level["maxReputation"] = maxReputation;
    level["incrementReputation"] = incrementReputation;

    level["patientGenerationRate"] = patientGenerationRate;
    level["patientWeight1"] = patientWeight1;
    level["patientWeight2"] = patientWeight2;
    level["patientWeight3"] = patientWeight3;

    level["dirtinessRate"] = dirtinessRate;

    return level;
}
