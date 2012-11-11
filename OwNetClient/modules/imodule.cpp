#include "imodule.h"
#include <QByteArray>
#include <QDebug>

IModule::IModule(QObject *parent) :
    QObject(parent)
{
}

QByteArray* IModule::processRequest(IBus *bus, ProxyRequest *req)
{
    Action a = UNKNOWN;



    //get right action, also perserve other actions as REST

    if(req->action()=="" || req->action()=="index" || req->action()=="show" || req->action()=="edit" || req->action()=="delete" || req->action()=="create")
    {

        //case index
        if( req->id() == 0 && req->requestType()== ProxyRequest::GET){

            a = INDEX;
        }

        //case show
        else if( req->requestType()== ProxyRequest::GET  )
            a = SHOW;

        //case create
        else if( req->requestType() == ProxyRequest::POST )
            a = CREATE;

        //case edit
        else if( req->requestType() == ProxyRequest::PUT)
            a =  EDIT;

        //case delete
        else if(req->requestType() == ProxyRequest::DELETE)
            a = DELETE;
    }

    switch (a)
    {

    case INDEX:

            return index(bus, req);
            break;

    case SHOW:
            return show(bus, req);
            break;

    case EDIT:
            return edit(bus, req);
            break;

    case CREATE:
            return create(bus, req);
            break;

    case DELETE:
            del(bus, req);
            break;

    default:
            //other actions TO DO
            return new QByteArray();
            break;


    }


}


