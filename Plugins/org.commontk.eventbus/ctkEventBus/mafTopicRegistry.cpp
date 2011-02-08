/*
 *  ctkTopicRegistry.cpp
 *  mafEventBus
 *
 *  Created by Roberto Mucci on 26/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTopicRegistry.h"

using namespace mafEventBus;

ctkTopicRegistry* ctkTopicRegistry::instance() {
    static ctkTopicRegistry instanceTopicRegistry;
    return &instanceTopicRegistry;
}

ctkTopicRegistry::ctkTopicRegistry() {
}

void ctkTopicRegistry::shutdown() {
    m_TopicHash.clear();
}

bool ctkTopicRegistry::registerTopic(const mafString topic, const QObject *owner) {
    if(topic.isEmpty() || owner == NULL ){
        return false;
    }
    if(m_TopicHash.contains(topic)){
        //topic already registered
        const QObject *obj = m_TopicHash.value(topic,NULL);
        mafString className(obj->metaObject()->className());
        mafMsgWarning() << mafTr("Topic %1 already owned by %2").arg(topic, className);
        return false;
    }
    m_TopicHash.insert(topic,owner);
    return true;
}

bool ctkTopicRegistry::unregisterTopic(const mafString topic) {
    bool result = false;
    if(m_TopicHash.contains(topic)){
        if (m_TopicHash.remove(topic) > 0) {
            result = true;
        }
    }
    return result;
}

const QObject *ctkTopicRegistry::owner(const mafString topic) const {
    return m_TopicHash.value(topic,NULL);
}

bool ctkTopicRegistry::isTopicRegistered(const mafString topic) const {
    return m_TopicHash.contains(topic);
}

void ctkTopicRegistry::dump() {
    mafHash<mafString, const QObject*>::const_iterator i = m_TopicHash.constBegin();
    while (i != m_TopicHash.constEnd()) {
        const QObject *obj = i.value();
        mafString key =  i.key();
        mafString name = obj->metaObject()->className();
        mafMsgDebug() << "Topic: " << key << "   Owner: "  << name;
        ++i;
    }
}
