/*
 *  ctkTopicRegistry.h
 *  mafEventBus
 *
 *  Created by Roberto Mucci on 26/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef CTKTOPICREGISTRY_H
#define CTKTOPICREGISTRY_H

// Includes list
#include "mafEventDefinitions.h"

namespace mafEventBus {

/**
  Class name: ctkTopicRegistry
  This singletone provides the registration of topic and topic owner in a hash.
*/
class CTKEVENTBUSSHARED_EXPORT ctkTopicRegistry {
public:
    /// Return an instance of the event bus.
    static ctkTopicRegistry *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// Register a new topic.
    bool registerTopic(const mafString topic, const QObject *owner);

    /// Unregister a topic.
    bool unregisterTopic(const mafString topic);

    /// Return the owner of a topic.
    const QObject *owner(const mafString topic) const;

    /// Check if a topic is present in the topic hash.
    bool isTopicRegistered(const mafString topic) const;

    /// Dump of the topic hash.
    void dump();

private:
    /// Object constructor.
    ctkTopicRegistry();

    mafHash<mafString, const QObject*> m_TopicHash; ///< Hash containing pairs (topic,owner).
};

} //nameSpace mafEventBus

#endif // CTKTOPICREGISTRY_H
