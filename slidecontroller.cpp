#include "slidecontroller.h"
#include <QJsonObject>
#include <QJsonDocument>
SlideController::SlideController(QObject* parent):HttpRequestHandler(parent)
{

}

SlideController::~SlideController()
{

}

void SlideController::service(HttpRequest &request, HttpResponse &response)
{
    response.setHeader("Content-Type", "application/json; charset=utf-8");

    QJsonObject reqjson = QJsonDocument::fromJson(request.getBody()).object();
    QJsonObject result;

    if (request.getParameter("action")=="uploadimage")
    {
        int i = 0;
        for (i = 1; i < 5; ++i) {
            QString name = QString::number(i);
            QTemporaryFile* file=request.getUploadedFile(name.toLatin1());
            if (!cacheFile(file))
                break;
            else
                uploadedSlidesName << request.getParameter(name.toLatin1());
        }

        result["success"] = (i>1);
    }
    else if (!reqjson.isEmpty())
    {
        bool ok = true;
        auto cmd = reqjson["cmd"].toString();
        if (cmd=="left")
        {
            if (selected<=0 || uploadedSlides.empty())
            {
                result["errornumber"] = 100; //out of index
                ok = false;
            }
            else
            {
                selected--;
                emit setImage(uploadedSlidesName[selected],uploadedSlides[selected]);
            }
        }
        else if (cmd=="right")
        {
            if (selected>=uploadedSlides.size()-1 || uploadedSlides.empty())
            {
                result["errornumber"] = 100;//out of index
                ok = false;
            }
            else
            {
                selected++;
                emit setImage(uploadedSlidesName[selected],uploadedSlides[selected]);
            }
        }
        else if (cmd=="reset")
        {
            resetController();
        }
        else
        {
            ok = false;
            result["errornumber"] = 103;
        }
        result["success"] = ok  ;

    }
    response.write(QJsonDocument(result).toJson(QJsonDocument::Compact),true);
}
