#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <poppler/qt5/poppler-qt5.h>
#include <QMutex>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QLabel
{

    Q_OBJECT
    QMutex mutex;

    explicit MainWindow(QWidget *parent = 0);
    static MainWindow* INSTANCE;
public:
    static MainWindow* getInstance()
    {
        if (INSTANCE==0)
        {
            INSTANCE = new MainWindow();
        }
        return INSTANCE;
    }
    void changeImagePos(int x , int y);

    ~MainWindow();

public slots:
    void setPage(const QImage &image);
};

#endif // MAINWINDOW_H
