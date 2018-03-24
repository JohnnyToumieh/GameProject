#include <QApplication>
#include <QtWidgets>

#include "HomePage.h"
/**
 * \mainpage main
 * \author Hassan Fakih
 * \author Johnny Tomieh
 * \date   3-24-2018
 */
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QWidget *widget= new QWidget();
    HomePage *homepage = new HomePage(widget);
    widget->show();
    return app.exec();
}
