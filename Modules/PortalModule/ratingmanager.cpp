#include "ratingmanager.h"

RatingManager::RatingManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_activityManager = new ActivityManager(proxyConnection);
}

 bool RatingManager::createRating()
 {
     Activity *ac = new Activity();
     ac->activity_type = Activity::RATING;

     m_activityManager->createActivity(ac);
 }
