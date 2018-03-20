#include "Aquarium.h"

Aquarium::Aquarium(int level, int maxCleanliness, int incrementCleanliness, int currentCleanliness, int immunityFactor, int maxTime, int currentTime, int score, QObject *parent) : QObject(parent)
{
    this->level = level;

    this->maxCleanliness = maxCleanliness;
    this->incrementCleanliness = incrementCleanliness;
    this->currentCleanliness = currentCleanliness;
    this->immunityFactor = immunityFactor;
    this->maxTime = maxTime;
    this->currentTime = currentTime;
    this->score = score;
}
