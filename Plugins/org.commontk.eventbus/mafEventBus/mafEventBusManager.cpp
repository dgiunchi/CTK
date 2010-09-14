/*
 *  mafEventBusManager.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventBusManager.h"
#include "mafEventBusRegistration.h"
#include <mafIdProvider.h>
#include <mafObjectFactory.h>
#include <mafContracts.h>

using namespace mafEventBus;
using namespace mafCore;


mafEventBusManager::mafEventBusManager() : m_EnableEventLogging(false), m_LogEventId(-1) {
    // register all object's module.
    mafEventBusRegistration::registerEventBusObjects();

    // Create ID for global notification event.
    mafIdProvider::instance()->createNewId("GLOBAL_UPDATE_EVENT");

    // Create local event dispatcher.
    m_LocalDispatcher = mafNEW(mafEventBus::mafEventDispatcherLocal);
    m_LocalDispatcher->setObjectName("Local Event Dispatcher");

    // Create the remote event dispatcher.
    m_RemoteDispatcher = mafNEW(mafEventBus::mafEventDispatcherRemote);
    m_RemoteDispatcher->setObjectName("Remote Event Dispatcher");

    // register default remote connection protocol.
    plugNetworkConnector("XMLRPC", "mafEventBus::mafNetworkConnectorQXMLRPC");
    plugNetworkConnector("SOAP", "mafEventBus::mafNetworkConnectorQtSoap");
}

mafEventBusManager::~mafEventBusManager() {
    mafDEL(m_LocalDispatcher);
    mafDEL(m_RemoteDispatcher);
}

mafEventBusManager* mafEventBusManager::instance() {
    static mafEventBusManager instanceEventBus;
    return &instanceEventBus;
}

void mafEventBusManager::shutdown() {
}

bool mafEventBusManager::addEventProperty(const mafEvent &props) const {
    // Design by contract condition.
    REQUIRE(props["EventId"].toInt() >= 0);

    if(props["EventType"].toInt() == mafEventTypeLocal) {
        // Local event dispatching.
        if(props["SignatureType"].toInt() == mafSignatureTypeCallback) {
            return m_LocalDispatcher->addObserver(props);
        } else {
            return m_LocalDispatcher->registerSignal(props);
        }
    } else {
        // Remote event dispatching.
        if(props["SignatureType"].toInt() == mafSignatureTypeCallback) {
            mafMsgWarning("%s", mafTr("Local Observer can't register in Remote dispatcher").toAscii().data());
            return false;
        } else {
            return m_RemoteDispatcher->registerSignal(props);
        }
    }
    return false;
}

bool mafEventBusManager::removeEventProperty(const mafEvent &props) const {
    // Design by contract condition.
    REQUIRE(props.eventId() >= 0);

    if(props.eventType() == mafEventTypeLocal) {
        // Local event dispatching.
        if(props["SignatureType"].toInt() == mafSignatureTypeCallback) {
            return m_LocalDispatcher->removeObserver(props);
        } else {
            return m_LocalDispatcher->removeSignal(props);
        }
    } else {
        // Remote event dispatching.
        if(props["SignatureType"].toInt() == mafSignatureTypeCallback) {
            return m_RemoteDispatcher->removeObserver(props);
        } else {
            return m_RemoteDispatcher->removeSignal(props);
        }
    }
    return false;
}

void mafEventBusManager::notifyEvent(const mafString id, mafEventType ev_type, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    mafId numeric_id = mafIdProvider::instance()->idValue(id);
    if(numeric_id != -1) {
        notifyEvent(numeric_id, ev_type, argList, returnArg);
    } else {
        mafMsgWarning("%s", mafTr("ID named '%1' is not mapped into the mafIdProvider!!").arg(id).toAscii().data());
    }
}

void mafEventBusManager::notifyEvent(const mafId id, mafEventType ev_type, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    // Design by contract condition.
    REQUIRE(id >= 0);

    if(m_EnableEventLogging) {
        if(m_LogEventId == -1 || m_LogEventId == id) {
            mafMsgDebug() << mafTr("Event notification for ID: ") << id << mafTr(" named: ") << mafIdProvider::instance()->idName(id) << "\n";
        }
    }

    //event dispatched in local channel
    mafEvent *event_dic = mafNEW(mafEventBus::mafEvent);
    (*event_dic)["EventId"] = (int)id;
    (*event_dic)["EventType"] = ev_type;
    notifyEvent(*event_dic, argList, returnArg);
    mafDEL(event_dic);
}

void mafEventBusManager::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    //event dispatched in remote channel
    if(event_dictionary["EventType"].toInt() == mafEventTypeLocal) {
        m_LocalDispatcher->notifyEvent(event_dictionary, argList, returnArg);
    } else {
        m_RemoteDispatcher->notifyEvent(event_dictionary, argList);
    }
}

void mafEventBusManager::enableEventLogging(bool enable) {
    m_EnableEventLogging = enable;
}

void mafEventBusManager::logEventId(const mafId id) {
    // Design by contract condition.
    REQUIRE(id >= 0);

    m_LogEventId = id;
}

void mafEventBusManager::logAllEvents() {
    m_LogEventId = -1;
}

bool mafEventBusManager::createServer(const mafString &communication_protocol, unsigned int listen_port) {
    bool res(m_NetworkConnectorHash.contains(communication_protocol));
    if(res) {
        mafString connector_type = m_NetworkConnectorHash.value(communication_protocol);
        m_RemoteDispatcher->setNetworkConnectorServer(connector_type);
        mafNetworkConnector *connector = m_RemoteDispatcher->networkConnectorServer();
        res = connector != NULL;
        connector->createServer(listen_port);
    }
    return res;
}

void mafEventBusManager::startListen() {
    mafNetworkConnector *connector = m_RemoteDispatcher->networkConnectorServer();
    if(connector) {
        connector->startListen();
    } else {
        mafMsgWarning("%s", mafTr("Server can not start. Create it first, then call startListen again!!").toAscii().data());
    }
}

bool mafEventBusManager::createClient(const mafString &communication_protocol, const mafString &server_host, unsigned int port) {
    bool res(m_NetworkConnectorHash.contains(communication_protocol));
    if(res) {
        mafString connector_type = m_NetworkConnectorHash.value(communication_protocol);
        m_RemoteDispatcher->setNetworkConnectorClient(connector_type);
        mafNetworkConnector *connector = m_RemoteDispatcher->networkConnectorClient();
        res = connector != NULL;
        connector->createClient(server_host, port);
    }
    return res;
}
