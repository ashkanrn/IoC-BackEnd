#include "pagerenderer.h"


PageRenderer::PageRenderer(QObject* ptr):QObject(ptr)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(4); //equals to the num of cpu cores.
}
