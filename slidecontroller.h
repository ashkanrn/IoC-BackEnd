#ifndef SLIDECONTROLLER_H
#define SLIDECONTROLLER_H

#include "httprequesthandler.h"
#include <QMap>

class SlideController : public HttpRequestHandler {
    Q_OBJECT
    QVector<QByteArray> uploadedSlides;
    QVector<QString> uploadedSlidesName;

    int selected = -1;
    void resetController()
    {
        uploadedSlides.clear();
        selected = -1;
    }
    bool cacheFile(QTemporaryFile* file)
    {
        if (file)
        {
            QByteArray image;
            while (!file->atEnd() && !file->error())
            {
                QByteArray buffer=file->read(65536);
                image+=buffer;
            }
            if (!image.isEmpty())
            {
                uploadedSlides << image;
                return true;
            }
        }
        return false;
    }

public:
    SlideController( QObject* parent=0);
    virtual ~SlideController();
    virtual void service(HttpRequest& request, HttpResponse& response);
signals:
    void setImage(QString name,QByteArray ba);
};

#endif // SLIDECONTROLLER_H
