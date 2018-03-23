#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <QObject>

class Aquarium : public QObject
{
    Q_OBJECT
public:
    explicit Aquarium(int level, int currentCleanliness, int currentTime, int score, QObject *parent = nullptr);
    void setUpLevels();

    int level;

    std::map<int, std::map<std::string, int>> levels;

    int currentCleanliness;
    int currentTime;

    int score;

    bool gamePaused;
    bool requestForUnpause;

private:
    std::map<std::string, int> setUpLevelsHelper(int maxTime,
                                                 int maxCleanliness, int incrementCleanliness,
                                                 int immunityDegree,
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
