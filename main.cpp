#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "httplistener.h"
#include "staticfilecontroller.h"
#include "requestmapper.h"
#include "pagerenderer.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    QSettings* listenerSettings=new QSettings("setting.ini", QSettings::IniFormat, &a);
    QSettings* fileSettings=new QSettings("setting.ini", QSettings::IniFormat, &a);

    fileSettings->beginGroup("files");
    auto staticFileController=new StaticFileController(fileSettings,&a);

    listenerSettings->beginGroup("listener");

    auto mapper = new RequestMapper(staticFileController,&a);
    new HttpListener(listenerSettings, mapper, &a);

    MainWindow::getInstance()->show();

    return a.exec();
}
