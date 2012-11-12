#include "imodule.h"
#include <QByteArray>
#include <QDebug>

IModule::IModule(QObject *parent) :
    QObject(parent)
{
}

QByteArray* IModule::processRequest(IBus *bus, ProxyRequest *req)
{

    //get right action, also perserve other actions as REST

    if(req->action()=="" || req->action()=="index" || req->action()=="show" || req->action()=="edit" || req->action()=="delete" || req->action()=="create")
    {

        //case index
        if( req->id() == 0 && req->requestType()== ProxyRequest::GET){

            return index(bus, req);
        }

        //case show
        else if( req->requestType()== ProxyRequest::GET  )
            return show(bus, req);

        //case create
        else if( req->requestType() == ProxyRequest::POST )
            return create(bus, req);

        //case edit
        else if( req->requestType() == ProxyRequest::PUT)
            return edit(bus, req);

        //case delete
        else if(req->requestType() == ProxyRequest::DELETE)
             del(bus, req);
    }
    else
        //other actions TO DO
        return new QByteArray();

}


