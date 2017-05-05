#include "requestmapper.h"
#include <QDir>
#include <QString>
#include <QBuffer>

RequestMapper::RequestMapper(StaticFileController *sfc, QObject* parent)
    : HttpRequestHandler(parent),staticFileController(sfc) {

    window = new MainWindow();
    window->show();

    servercall = new ServerCall(this);

    connect(this,&RequestMapper::destroyed,window,&MainWindow::deleteLater);

    connect(servercall,&ServerCall::changePos,window,&MainWindow::changeImagePos,Qt::QueuedConnection);
    connect(servercall->getPresenter(),&Presenter::setPage,window,&MainWindow::setPage,Qt::QueuedConnection);

    connect(this,&RequestMapper::servercallfunc,servercall,&ServerCall::service,Qt::BlockingQueuedConnection);
    connect(this,&RequestMapper::upload,this,&RequestMapper::uploadService,Qt::BlockingQueuedConnection);
}

RequestMapper::~RequestMapper() {
    // empty
}

void RequestMapper::service(HttpRequest &request, HttpResponse &response) {
    response.setHeader("Cache-Control","no-store"); //force all response to not store

    auto path = request.getPath().toLower();
    if (path=="/upload")
    {
        emit uploadService(request,response);
        return;
    }
    if (path=="/servercall")
    {
        emit servercallfunc(request,response);
        return;
    }
    if (path.startsWith("/pageimage"))
    {

        QString page = path.replace("/pageimage/","");
        auto image = servercall->getPresenter()->getImage(page.toInt());

        QByteArray data;
        QBuffer buff(&data);
        buff.open(QBuffer::WriteOnly);

        image.save(&buff, "jpg");

        response.setHeader("Content-Type","image/jpg");
        response.write(data,true);

        return;
    }
    staticFileController->service(request,response);
}
