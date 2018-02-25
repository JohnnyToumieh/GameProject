#ifndef GAMEONEPAGE_H
#define GAMEONEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

class GameOnePage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit GameOnePage(QWidget *widget);
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
signals:

private:
    void setVerticalLayout();

public slots:
    void backClicked();
};

#endif // GAMEONEPAGE_H
