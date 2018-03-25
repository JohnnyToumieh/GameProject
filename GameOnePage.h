#ifndef GAMEONEPAGE_H
#define GAMEONEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

#include "ChooseGamePage.h"
#include "HistoryPage.h"
#include "Game1Scene.h"
#include "User.h"

/**
 *\file GameOnePage.h
 *@brief The GameOnePage class, represents the page that let user start or resume a game (and check history...)
 * it is a common page between game 1 and game 2
 *
 */

class GameOnePage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit GameOnePage(QWidget *widget, int gameNumber, User* user, QJsonObject usersFile);

    int gameNumber;//!<Integer member gameNumber (1,2) that represents number of the game chosen
    QVBoxLayout* verticalLayout;
    QGridLayout* gridLayout;
    QGridLayout *grid;

    QWidget *widget;

    QPushButton *newGame;
    QPushButton *resumeGame;
    QComboBox *selectLevel;
    QPushButton *checkRecords;
    QPushButton *back;
    QPushButton *description;
    QPushButton *checkHistory;

    QLabel* profilePictureL;
    QLabel* nameL;

    User* user;//!<User member that represents the signed in user
    QJsonObject usersFile;//!QJsonObject member usersFile that holds information of the user

signals:

private:
    void setVerticalLayout();
    bool resumeGameExists();

public slots:
    void backClicked();//!<Member function takes user to previous page
    void descriptionClicked();//!<Member function takes user to game description
    void checkHistoryClicked();//!<Member function takes user to check history page
    void startNewGameClicked();//!<Member function takes user to a new game
    void resumeGameClicked();//!<Member function allows user to resume and old game
};

#endif // GAMEONEPAGE_H
