#include <QApplication>
#include <QtWidgets>
#include "homePage.h"
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    //QPushButton button ("Hello world !");
    //button.show();

    homePage homepage;
    homepage.show();
    return app.exec();
}
