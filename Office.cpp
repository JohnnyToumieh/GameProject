#include "Office.h"
/**
 *\file Office.cpp
 *@brief contains Office class definition which represents office as an object holding main attributes of the game
 *
 * Office class set the level,currentReputation, time, score(with that of minigames), and game states.
 * Also it sets up each level with a specific attributes that define speed and frequency of object making an
 * overall difficulty of each level (increasing from 1 to 3).
 *
 */

/**
 * @brief Office::Office constructor of Office class
 *
 * A constructor that set different parameters (level,cleanliness,time and score)
 * @param level argument integer indicating the level
 * @param currentReputation argument integer indicating the reputation of the office
 * @param currentTime argument integer indicating the time of the game timer
 * @param currentAquariumState argument integer indicating the state of the aquarium
 * @param int score
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
    this->currentMiniGameState = 0;

    this->gamePaused = false;
    this->requestForUnpause = false;
    this->inAMiniGame = false;
}

/**
 * @brief Office::keyPressEvent member function: sets up the levels
 *
 * function that responed to pressing escape key
 * @param QKeyEvent *event
 */
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
    this->levels[1] = setUpLevelsHelper(480, 840, 200,
                                          10, 1,
                                          3000, 20, 10, 10,
                                          20, 10, 10,
                                          10, 0, 0,
                                          20000,1000);

    this->levels[2] = setUpLevelsHelper(480, 840, 200,
                                          10, 1,
                                          3000, 10, 10, 10,
                                          10, 15, 15,
                                          20, 5, 5,
                                          20000,2000);

    this->levels[3] = setUpLevelsHelper(480, 840, 200,
                                          10, 1,
                                          3000, 5, 10, 15,
                                          5, 10, 15,
                                          10, 15, 15,
                                          20000,3000);
}

/**
 * @brief Office::setUpLevelsHelper member function: help setting up the levels
 *
 * function take diffrenet attributes of the level and set them in a map of string key describing the attribute and integer value
 * equal to the passed parameters
 * @param startTime the start time of office day
 * @param endTime the end time of the office day
 * @param minuteInMilliSeconds minutes to complete one day in MilliSeconds
 * @param maxReputation the maximum reputation can be reached
 * @param incrementReputation steps by which reputation increase
 * @param patientGenerationRate
 * @param patientWeight1 wieght of patient level 1 diff 1
 * @param patientWeight2 wieght of patient level 1 diff 2
 * @param patientWeight3 wieght of patient level 1 diff 3
 * @param patientWeight4 wieght of patient level 2 diff 1
 * @param patientWeight5 wieght of patient level 2 diff 2
 * @param patientWeight6 wieght of patient level 2 diff 2
 * @param patientWeight7 wieght of patient level 3 diff 1
 * @param patientWeight8 wieght of patient level 3 diff 1
 * @param patientWeight9 wieght of patient level 3 diff 1
 * @param dirtinessRate rate at which the aquarium gets dirty
 * @param dailyGoal the goal patient should reach to go to next level
 */
std::map<std::string, int> Office::setUpLevelsHelper(int startTime, int endTime, int minuteInMilliSeconds,
                                     int maxReputation, int incrementReputation,
                                     int patientGenerationRate, int patientWeight1, int patientWeight2, int patientWeight3,
                                     int patientWeight4, int patientWeight5, int patientWeight6,
                                     int patientWeight7, int patientWeight8, int patientWeight9,
                                     int dirtinessRate,int dailyGoal) {
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

    level["patientWeight4"] = patientWeight4;
    level["patientWeight5"] = patientWeight5;
    level["patientWeight6"] = patientWeight6;

    level["patientWeight7"] = patientWeight7;
    level["patientWeight8"] = patientWeight8;
    level["patientWeight9"] = patientWeight9;

    level["dailyGoal"] = dailyGoal;

    level["dirtinessRate"] = dirtinessRate;

    level["levelState"] = 0;

    return level;
}
