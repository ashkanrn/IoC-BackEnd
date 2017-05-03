#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H
#include "httprequesthandler.h"
#include "staticfilecontroller.h"
#include "servercall.h"

class RequestMapper : public HttpRequestHandler {
    Q_OBJECT
    StaticFileController* staticFileController;

    void uploadService(HttpRequest &request, HttpResponse &response)
    {
        //TODO:
        //multiple file support
        //return result in response
        response.setHeader("Content-Type","application/json");
        response.setHeader("Access-Control-Allow-Origin","*");
        auto file = request.getUploadedFile("0");
        if (file==nullptr)
        {
            response.setStatus(202);
            response.write("{\"error\":true}");
            return;
        }
        file->copy(QDir::currentPath()+"/"+request.getParameter("0"));
        response.setStatus(200);
        response.write("{}");
    }

public:
    RequestMapper(StaticFileController* sfc,QObject* parent=0);
    virtual ~RequestMapper();
    virtual void service(HttpRequest& request, HttpResponse& response);
};

#endif // REQUESTMAPPER_H
