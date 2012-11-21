#include "imodule.h"
#include <QByteArray>
#include <QDebug>

/**
 * @brief processRequest Virtual function for Processing request from proxy/
 *        every module derived from IModule has to implement this function in it's own way
 * @param bus
 * @param req
 * @return  response Bytes
 */
QByteArray *IModule::processRequest(IBus *bus, IRequest *req)
{

    //get right action, also perserve other actions

    if(req->action()=="" || req->action()=="index" || req->action()=="show" || req->action()=="edit" || req->action()=="delete" || req->action()=="create")
    {

        //case index
        if( req->id() == -1 && req->requestType()== IRequest::GET){

            return index(bus, req);
        }

        //case show
        else if( req->requestType()== IRequest::GET  )
            return show(bus, req);

        //case create
        else if( req->requestType() == IRequest::POST )
            return create(bus, req);

        //case edit
        else if( req->requestType() == IRequest::PUT)
            return edit(bus, req);

        //case delete
        else if(req->requestType() == IRequest::DELETE)
             return del(bus, req);
    }
    //other actions TO DO
    return new QByteArray();
}


