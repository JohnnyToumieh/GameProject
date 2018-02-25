#include <QApplication>
#include <QtWidgets>
#include "HomePage.h"
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QWidget *widget= new QWidget();
    HomePage *homepage = new HomePage(widget);
    widget->show();
    return app.exec();
}
