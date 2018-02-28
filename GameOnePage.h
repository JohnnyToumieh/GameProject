#ifndef GAMEONEPAGE_H
#define GAMEONEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>
#include "User.h"

class GameOnePage : public QVBoxLayout
{
    Q_OBJECT
public:
<<<<<<< HEAD
    explicit GameOnePage(QWidget *widget,int gameNumber);
    int gameNumber;
=======
    explicit GameOnePage(QWidget *widget, int gameNumber, User* user);

    int gameNumber;
    
>>>>>>> master
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

    User* user;

signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();
    void descriptionClicked();
};

#endif // GAMEONEPAGE_H
