#include "artificialrequest.h"

ArtificialRequest::ArtificialRequest(IRequest::RequestType requestType, const QString &module, const QString &action, int id, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_module(module),
    m_action(action),
    m_requestType(requestType)
{
}
