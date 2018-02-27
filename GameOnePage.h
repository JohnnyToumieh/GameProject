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
    explicit GameOnePage(QWidget *widget, User* user);
    QVBoxLayout* verticalLayout;
    QGridLayout* gridLayout;
    QGridLayout *grid;

    QWidget *widget;

    QPushButton *newGame;
    QPushButton *resumeGame;
    QComboBox *selectLevel;
    QPushButton *checkRecords;
    QPushButton *back;

    QLabel* profilePictureL;
    QLabel* nameL;

    User* user;
signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();
};

#endif // GAMEONEPAGE_H
