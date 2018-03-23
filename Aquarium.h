#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <QObject>

class Aquarium : public QObject
{
    Q_OBJECT
public:
    explicit Aquarium(int level, int maxCleanliness, int incrementCleanliness, int currentCleanliness, int immunityFactor, int maxTime, int currentTime, int score, QObject *parent = nullptr);

    int level;

    /*Array for each level containing:
     * maxCleanliness
     * incrementCleanliness
     *
     * immunityDegree
     *
     * bacteriaGenerationRate
     * bacteriaWeight1
     * bacteriaWeight2
     * bacteriaWeight3
     *
     * itemsDropRate
     * healthyItemWeight
     * unhealthyItemWeight
     *
     * virusGenerationRate
     * virusWeight1
     * virusWeight2
     * virusWeight3
     */

    int maxCleanliness;
    int incrementCleanliness;
    int currentCleanliness;

    int immunityFactor;

    int maxTime;
    int currentTime;

    int score;

    bool gamePaused;
    bool requestForUnpause;
signals:

public slots:
};

#endif // AQUARIUM_H
