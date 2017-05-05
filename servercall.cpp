#include "servercall.h"

ServerCall::ServerCall(QObject *parent):QObject(parent){
    presenter = new Presenter(this);
}
