#ifndef PAGERENDERER_H
#define PAGERENDERER_H
#include <QObject>
#include <QThreadPool>
#include <QRunnable>
#include <poppler/qt5/poppler-qt5.h>
#include <QMutex>
#include <QApplication>
#include <QImage>
#include <QDesktopWidget>
#include <QMap>

class Renderer: public QRunnable
{
    Poppler::Page* page;
    QImage image;
    QMutex mutex;

public:
    Q_DISABLE_COPY(Renderer)

    int wp,hp;

    Renderer(int w,int h,Poppler::Page* page):page(page),wp(w),hp(h)
    {
        mutex.lock();

        this->setAutoDelete(false);
    }

    void run()
    {
        auto size = page->pageSize();

        auto ws = size.width();
        auto hs = size.height();

        auto aw = wp/double(ws);
        auto ah = hp/double(hs);

        auto deltah = hp-hs;
        auto deltaw = wp-ws;

        if (deltah<deltaw)
        {
            image = page->renderToImage(72*ah,72*ah,-1,-1,ws*ah,hs*ah);
        }
        else
        {
            image = page->renderToImage(72*aw,72*aw,-1,-1,ws*aw,hs*aw);
        }

        mutex.unlock();
    }

    QImage getImage()
    {
        QMutexLocker mlocker (&mutex);
        return image;
    }

    ~Renderer()
    {
        delete page;
    }
};

class PageRenderer: public QObject
{
    Q_OBJECT

    QThreadPool* pool;

    QMap<int,Renderer*> renderers;

    int w,h;
public:
    PageRenderer(int w=0, int h=0,QObject *ptr = 0);

    void addPage(int index ,Poppler::Page* p)
    {
        if (renderers.contains(index)) return;

        auto run = new Renderer(w,h,p);
        pool->start(run);

        renderers.insert(index,run);
    }

    QImage getImage(int index)
    {
        if (!renderers.contains(index)) return QImage();
        return renderers[index]->getImage();
    }

    ~PageRenderer()
    {
        pool->clear();
        pool->waitForDone();
        for (auto p:renderers.values())
        {
            delete p;
        }
    }
    void setWidth(int n)
    {
        w=n;
    }
    void setHeight(int n)
    {
        h=n;
    }
};

#endif // PAGERENDERER_H
