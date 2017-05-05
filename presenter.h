#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include<QString>
#include<QProcess>
#include<QFile>
#include<QDebug>
#include<QTemporaryDir>
#include<QImage>
#include <poppler/qt5/poppler-qt5.h>
#include "mainwindow.h"
#include "pagerenderer.h"
/*
  -200 out of range
  -201 file not loaded
  -202 internal error file loading(path incorrect)
  -203 invalid document
  -204 invalid page

 */
class Presenter : public QObject
{
    Q_OBJECT


public:
    explicit Presenter(QObject *parent = 0);

    bool convertPPT(QString filePath,QString saveFileName);
    int loadPdf(QString path);

    void endPresent();
    int setCurrentPage(int pageNumber);

    QImage getImage(int slide)
    {
        if (!pr) return QImage();

        return pr->getImage(slide-1);
    }
    int pageCount();

    PageRenderer* getRenderer()
    {
        return pr;
    }

private:
    Poppler::Document *mainDocument=0;
    PageRenderer* pr=0;

    int current_page;
signals:
    void setPage(QImage img);
};

#endif // PRESENTER_H
