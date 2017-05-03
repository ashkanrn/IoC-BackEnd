#include "requestmapper.h"
#include <QDir>
#include <QString>
#include <QBuffer>

RequestMapper::RequestMapper(StaticFileController *sfc, QObject* parent)
    : HttpRequestHandler(parent),staticFileController(sfc) {
    // empty
    qDebug()<<QDir::currentPath();
}

RequestMapper::~RequestMapper() {
    // empty
}

void RequestMapper::service(HttpRequest &request, HttpResponse &response) {
    auto path = request.getPath().toLower();
    if (path=="/upload")
    {
        uploadService(request,response);
        return;
    }
    if (path=="/servercall")
    {
        ServerCall::getInstance()->service(request,response);
        return;
    }
    if (path.startsWith("/pageimage"))
    {
        //TODO:
        //Control page number
        //control file loaded
        //check image not null
        QString page = path.replace("/pageimage/","");
        Presenter* presenter = Presenter::getInstance();
        QImage image = presenter->getImage(page.toInt());
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "jpg");

        response.setHeader("Content-Type","image/jpg");
        response.write(data,true);
        return;
    }
    staticFileController->service(request,response);
}
