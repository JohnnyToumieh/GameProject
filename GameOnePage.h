#ifndef GAMEONEPAGE_H
#define GAMEONEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

class GameOnePage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit GameOnePage(QWidget *widget,int gameNumber);
    int gameNumber;
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

signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();
    void descriptionClicked();
    void checkHistoryClicked();
};

#endif // GAMEONEPAGE_H
