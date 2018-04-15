#include "StateTracker2.h"
/**
 *\file StateTracker2.cpp
 *@brief contains StateTracker2 class definition which represents the state tracker as an object holding main attributes of the game
 *
 * StateTracker2 class set the level,difficulity,time, score, and points.
 * Also it sets up each level with a specific attributes that define speed and frequency of object making an
 * overall difficulty of each level (increasing from 1 to 3).
 *
 */

/**
 * @brief StateTracker2::StateTracker2 constructor of StateTracker2 class
 *
 * A constructor that set different parameters (level,difficulity,time and score)
 * @param level argument integer indicating the level
 * @param difficulity specify diffuclity of guessing minigame
 * @param currentTime the time of the office
 * @param score argument integer indicating the score
 * @param points points awarded for the minigame
 * @param specialPoints  bonus points awarded for the minigam when finished early
 * @param timeLimit to finish the minigam
 * @param specialTimeLimit to finish the minigame with bonus
 */
StateTracker2::StateTracker2(int level, int difficulity, int currentTime,
                             int score, int points, int specialPoints,
                             int timeLimit, int specialTimeLimit, QObject *parent) : QObject(parent)
{
    this->level = level;
    this->difficulity = difficulity;

    setUpLevels();

    this->currentTime = currentTime;

    this->score = score;
    this->points = points;
    this->specialPoints = specialPoints;
    this->timeLimit = timeLimit;
    this->specialTimeLimit = specialTimeLimit;

    this->gamePaused = false;
    this->requestForUnpause = false;
}

/**
 * @brief StateTracker2::keyPressEvent
 *
 * function that responed to pressing escape key
 * @param QKeyEvent *event
 */
void StateTracker2::keyPressEvent(QKeyEvent *event){
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
 * @brief StateTracker2::setUpLevels member function: sets up the levels
 *
 * function that fill the array of the levels with 3 levels each with specified attributes.
 */
void StateTracker2::setUpLevels() {
    this->levels[1] = setUpLevelsHelper(300000,
                                        3, 5000,
                                        4, 4000,
                                        5, 3000);

    this->levels[2] = setUpLevelsHelper(300000,
                                        3, 3000,
                                        5, 2000,
                                        7, 1000);

    this->levels[3] = setUpLevelsHelper(300000,
                                        3, 3000,
                                        5, 2000,
                                        7, 1000);
}

/**
 * @brief StateTracker2::setUpLevelsHelper member function: help setting up the levels
 *
 * function take diffrenet attributes of the level and set them in a map of string key describing the attribute and integer value
 * equal to the passed parameters
 * @param maxTime integer that indicates the maximum time allowed to complete the level
 * @param difficulity1NumberOfTeeth number of infected teeth for diff 1
 * @param difficulity1SpeedOfTeeth  speed of showing infected teeth for diff 1
 * @param difficulity2NumberOfTeeth number of infected teeth for diff 2
 * @param difficulity2SpeedOfTeeth  speed of showing infected teeth for diff 2
 * @param difficulity3NumberOfTeeth number of infected teeth for diff 3
 * @param difficulity3SpeedOfTeeth  speed of showing infected teeth for diff 3
 */
std::map<std::string, int> StateTracker2::setUpLevelsHelper(int maxTime,
                                                            int difficulity1NumberOfTeeth, int difficulity1SpeedOfTeeth,
                                                            int difficulity2NumberOfTeeth, int difficulity2SpeedOfTeeth,
                                                            int difficulity3NumberOfTeeth, int difficulity3SpeedOfTeeth) {
    std::map<std::string, int> level;

    level["maxTime"] = maxTime;

    level["difficulity1NumberOfTeeth"] = difficulity1NumberOfTeeth;
    level["difficulity1SpeedOfTeeth"] = difficulity1SpeedOfTeeth;

    level["difficulity2NumberOfTeeth"] = difficulity2NumberOfTeeth;
    level["difficulity2SpeedOfTeeth"] = difficulity2SpeedOfTeeth;

    level["difficulity3NumberOfTeeth"] = difficulity3NumberOfTeeth;
    level["difficulity3SpeedOfTeeth"] = difficulity3SpeedOfTeeth;

    level["levelState"] = 0;

    return level;
}
