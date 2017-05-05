#include "pagerenderer.h"


PageRenderer::PageRenderer(int w, int h, QObject* ptr):QObject(ptr),h(h),w(w)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(4); //equals to the num of cpu cores.
}
