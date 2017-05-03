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

    Renderer(Poppler::Page* page):page(page)
    {
        mutex.lock();

        this->setAutoDelete(false);
    }

    void run()
    {
        auto size = page->pageSize();
        QRect rec = QApplication::desktop()->screenGeometry();

        auto wp = rec.width();
        auto hp = rec.height();

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
public:
    PageRenderer(QObject *ptr = 0);

    void addPage(int index ,Poppler::Page* p)
    {
        if (renderers.contains(index)) return;

        auto run = new Renderer(p);
        pool->start(run);

        renderers.insert(index,run);
    }

    QImage getImage(int index)
    {
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
};

#endif // PAGERENDERER_H
