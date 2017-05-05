#ifndef SERVERCALL_H
#define SERVERCALL_H
#include "httprequesthandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QJsonArray>
#include "presenter.h"
#include "mainwindow.h"
#include <QBuffer>

using namespace stefanfrings;

class ServerCall: public QObject
{
    Q_OBJECT
    Presenter* presenter;

public:
    ServerCall(QObject* parent=0);


    void functionHandler(QJsonObject& json,QJsonObject& result,HttpResponse& response)
    {
        QString function = json["function"].toString().toLower();
        QJsonArray arg =  json["arg"].toArray();

        int status = 502;
        QString error = "undefined error";

        if (function=="setslide" && !arg.isEmpty())
        {
            result["return"] = SetSlide(arg[0].toInt(),status,error);
        }
        else if (function=="openfile" && !arg.isEmpty())
        {
            result["return"] = OpenFile(arg[0].toString(),status,error);
        }
        else if (function=="setpos" && !arg.isEmpty())
        {
            result["return"] = SetPos(arg[0].toInt(),arg[1].toInt(),status,error);
        }
        else if (function=="loadimage" && !arg.isEmpty())
        {
            result["return"] = LoadImage(arg[0].toInt(),status,error);
        }
        else
        {
            status = 501;
            error = "invalid function name or argument";
        }

        setError(result,status,error);
        writeJson(result,response);
    }

    void setError(QJsonObject& obj,int num = 0,QString error = QString())
    {
        obj["status"] =num;
        obj["error"] =error;
    }

    void writeJson(QJsonObject& obj,HttpResponse& response)
    {
        response.setHeader("Content-Type","application/json");
        response.setHeader("Access-Control-Allow-Origin","*");
        response.write(QJsonDocument(obj).toJson());
    }
public: //Server Functions
    QJsonArray SetSlide(int slide , int& status, QString& error)
    {
        QJsonArray result;

        status = presenter->setCurrentPage(slide);
        error  = presenterError(status);

        return result;
    }
    QJsonArray OpenFile(QString filename, int& status, QString& error)
    {
        QJsonArray result;

        status = presenter->loadPdf(QDir::currentPath() + "/" + filename);
        error = presenterError(status);
        if (!status){
            int pageCount = presenter->pageCount();
            result.append(pageCount);
        }
        return result;
    }
    QJsonArray SetPos(int x,int y, int& status, QString& error)
    {
        QJsonArray result;

        emit changePos(x,y);

        status = 0;
        error = QString();
        return result;
    }
    QJsonArray LoadImage(int num, int& status, QString& error)
    {
        //TODO:
        //Check empty image
        QJsonArray result;

        auto image = presenter->getImage(num);

        QByteArray data;
        QBuffer buff(&data);
        buff.open(QBuffer::WriteOnly);

        image.save(&buff, "jpg");

        result.append(QString(data.toBase64()));

        status = 0;
        error = QString();
        return result;
    }
    Presenter* getPresenter()
    {
        return presenter;
    }

private:
    QString presenterError(int err)
    {
        switch (err) {
        case -202:
            return "internal error file loading(path incorrect)";
        case -200:
            return "out of range";
        case -201:
            return "file not loaded";
        case -203:
            return "invalid document";
        case -204:
            return "invalid page";
        default:
            return QString();
        }
    }
signals:
    void changePos(int x,int y);
public slots:
    void service(HttpRequest& request, HttpResponse& response)
    {
        auto jsonDoc = QJsonDocument::fromJson(request.getParameter("callInformation"));
        QJsonObject json = jsonDoc.object();
        QJsonObject result;

        if (json.isEmpty())
        {
            setError(result,500,"invalid function call information");
            writeJson(result,response);
            return;
        }
        functionHandler(json,result,response);
    }
};

#endif // SERVERCALL_H
