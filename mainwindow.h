#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <poppler/qt5/poppler-qt5.h>
#include <QMutex>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class MainWindow : public QGraphicsView
{

    Q_OBJECT
    QGraphicsScene* graphic;
    QGraphicsPixmapItem* pixmap;

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setPage(QImage image);

    void changeImagePos(int x , int y);
};

#endif // MAINWINDOW_H
