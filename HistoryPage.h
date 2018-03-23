#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

#include "GameOnePage.h"
#include "User.h"

class HistoryPage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit HistoryPage(QWidget *widget,int gameNumber, User* user, QJsonObject usersFile);

    bool read(const QJsonObject &json);

    int gameNumber;

    QListWidget *scores;

    QString topUser;
    QString topScore;
    QString* top10Scores;

    QLabel *top10ScoresL;
    QLabel *topUserL;
    QLabel *topScoreL;

    QWidget *widget;

    QVBoxLayout* verticalLayout;

    QPushButton *back;

    User* user;
    QJsonObject usersFile;

signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();

};

#endif // HISTORYPAGE_H
