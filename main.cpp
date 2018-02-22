#include <QApplication>
#include <QtWidgets>
#include "HomePage.h"
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    //QPushButton button ("Hello world !");
    //button.show();

    HomePage homepage;
    homepage.show();
    return app.exec();
}
