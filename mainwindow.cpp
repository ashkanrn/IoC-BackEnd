#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QGraphicsView(parent)
{
    graphic = new QGraphicsScene(this);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(graphic);

    pixmap = graphic->addPixmap(QPixmap(":/new/prefix1/photo_2016-11-06_12-50-18.jpg"));
}

void MainWindow::changeImagePos(int vx, int vy)
{
    if (vx==0&&vy==0)
    {
        pixmap->setPos(0,0);
    }
    else
    {
        pixmap->setPos(pixmap->pos().x()+vx,pixmap->pos().y()+vy);
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::setPage(QImage image)
{
    pixmap->setPixmap(QPixmap::fromImage(image));
}
