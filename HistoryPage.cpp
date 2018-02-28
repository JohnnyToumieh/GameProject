#include "HistoryPage.h"
#include "GameOnePage.h"

HistoryPage::HistoryPage(QWidget *widget,int gameNumber)
{
    this->gameNumber=gameNumber;
    this->widget=widget;

    verticalLayout = new QVBoxLayout();

    back = new QPushButton("Back");

    setVerticalLayout();
    widget->setFixedSize(500,350);
    widget->setLayout(verticalLayout);

     QObject::connect(back, SIGNAL(clicked()), SLOT(backClicked()));
}

void HistoryPage::backClicked(){
    qDeleteAll(widget->children());
    GameOnePage *chooseGamePage = new GameOnePage(widget,gameNumber);

}

void HistoryPage::setVerticalLayout()
{
    verticalLayout->addWidget(back);
}
