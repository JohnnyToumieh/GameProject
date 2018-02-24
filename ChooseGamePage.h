#ifndef CHOOSEGAMEPAGE_H
#define CHOOSEGAMEPAGE_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

class ChooseGamePage : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit ChooseGamePage(QWidget *widget);

    QVBoxLayout* verticalLayout;
    QGridLayout* gridLayout;
    QGridLayout *grid;

    QWidget *widget;

    QPushButton *game1;
    QPushButton *game2;
    QPushButton *back;

    QLabel* profilePictureL;
    QLabel* nameL;
signals:

private:
    void setVerticalLayout();

public slots:
    void backToHomeClicked();
    void game1Clicked();
    void game2Clicked();
};

#endif // CHOOSEGAMEPAGE_H
