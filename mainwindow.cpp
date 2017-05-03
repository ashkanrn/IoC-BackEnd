#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "presenter.h"


MainWindow* MainWindow::INSTANCE = 0;

MainWindow::MainWindow(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("background-color:blue;");
}

void MainWindow::changeImagePos(int vx, int vy)
{
    //TODO:
    //race condition
    mutex.lock();
    if (vx==0&&vy==0)
    {
        move(0,0);
    }
    else
    {
        move(x()+vx,y()+vy);
    }
    update();
    mutex.unlock();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setPage(const /*Poppler::Page*/QImage& image)
{
    setPixmap(QPixmap::fromImage(image));
}
