#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

#include "GamePage.h"
#include "User.h"

/**
 *\file HistoryPage.h
 *@brief The HistoryPage class, represents the page that display for the user his history in the game
 *
 */

class HistoryPage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit HistoryPage(QWidget *widget,int gameNumber, User* user, QJsonObject dataFile);

    bool read(const QJsonObject &json);//!<Member function takes reads userfile

    int gameNumber;//!<Integer member gameNumber (1,2) that represents number of the game chosen

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

    User* user;//!<User member user represents the signed in user
    QJsonObject dataFile;//!QJsonObject member dataFile that holds information of the user

signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();//!<Member function takes user to previous page

};

#endif // HISTORYPAGE_H
