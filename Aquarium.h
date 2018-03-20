#ifndef AQUARIUM_H
#define AQUARIUM_H

#include <QObject>

class Aquarium : public QObject
{
    Q_OBJECT
public:
    explicit Aquarium(int level, int maxCleanliness, int incrementCleanliness, int currentCleanliness, int immunityFactor, int maxTime, int currentTime, int score, QObject *parent = nullptr);

    int level;

    int maxCleanliness;
    int incrementCleanliness;
    int currentCleanliness;

    int immunityFactor;

    int maxTime;
    int currentTime;

    int score;

    bool gamePaused;
signals:

public slots:
};

#endif // AQUARIUM_H
