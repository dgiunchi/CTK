/*
 *  mafEventDispatcherRemote.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventDispatcherRemote.h"
#include "mafEvent.h"
#include "mafNetworkConnector.h"
#include <mafIdProvider.h>
#include <mafObjectFactory.h>

using namespace mafEventBus;
using namespace mafCore;

mafEventDispatcherRemote::mafEventDispatcherRemote(const mafString code_location) : mafEventDispatcher(code_location), m_NetworkConnectorServer(NULL), m_NetworkConnectorClient(NULL) {
    this->initializeGlobalEvents();
}

mafEventDispatcherRemote::~mafEventDispatcherRemote() {
    mafDEL(m_NetworkConnectorServer);
    mafDEL(m_NetworkConnectorClient);
}

void mafEventDispatcherRemote::initializeGlobalEvents() {
    mafEvent *properties = mafNEW(mafEventBus::mafEvent);
    (*properties)["EventId"] = (int) mafIdProvider::instance()->idValue("GLOBAL_UPDATE_EVENT");
    (*properties)["EventType"] = mafEventTypeRemote;
    (*properties)["SignatureType"] = mafSignatureTypeSignal;
    (*properties)["Signature"] = "notifyDefaultEvent()";
    this->registerSignal(*properties);

    Superclass::initializeGlobalEvents();
}

void mafEventDispatcherRemote::setNetworkConnectorServer(const mafString &connector_type) {
    if(m_NetworkConnectorServer != NULL) {
        mafString obj_type = m_NetworkConnectorServer->metaObject()->className();
        if(obj_type != connector_type) {
            mafNetworkConnector *newNC = (mafNetworkConnector *)mafNEWFromString(connector_type);
            if(newNC != NULL) {
                mafDEL(m_NetworkConnectorServer);
                m_NetworkConnectorServer = newNC;
            }
        }
    } else {
        m_NetworkConnectorServer = (mafNetworkConnector *)mafNEWFromString(connector_type);
    }
}

void mafEventDispatcherRemote::setNetworkConnectorClient(const mafString &connector_type) {
    if(m_NetworkConnectorClient != NULL) {
        mafString obj_type = m_NetworkConnectorClient->metaObject()->className();
        if(obj_type != connector_type) {
            mafNetworkConnector *newNC = (mafNetworkConnector *)mafNEWFromString(connector_type);
            if(newNC != NULL) {
                mafDEL(m_NetworkConnectorClient);
                m_NetworkConnectorClient = newNC;
            }
        }
    } else {
        m_NetworkConnectorClient = (mafNetworkConnector *)mafNEWFromString(connector_type);
    }
}

void mafEventDispatcherRemote::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    //Q_UNUSED(event_dictionary);
    //Q_UNUSED(argList);
    Q_UNUSED(returnArg);

    // Call the notifyEventRemote converting the arguments...

    mafList<mafVariant> *vl = NULL;
    if(argList != NULL) {
        vl = new mafList<mafVariant>();

        int i=0, size = argList->count();
        for(;i<size;i++) {
            mafString typeArgument;
            typeArgument = argList->at(i).name();
            if(typeArgument != "mafList<mafVariant>") {
                mafMsgWarning("%s", mafTr("Remote Dispatcher need to have arguments that are mafList<mafVariant>").toAscii().data());
                mafDEL(vl);
                return;
            }

            void *vp = argList->at(i).data();
            mafList<mafVariant> *l;
            l = (mafList<mafVariant> *)vp;
            mafVariant var;
            var.setValue(*l);

            vl->push_back(var); //only the first parameter represent the whole list of arguments
        }
        if(size == 0) {
            mafMsgWarning("%s", mafTr("Remote Dispatcher need to have at least one argument that is a mafList<mafVariant>").toAscii().data());
            return;
        }
    }

    this->notifyEventRemote(event_dictionary, vl);
    mafDEL(vl);
}

void mafEventDispatcherRemote::notifyEventRemote(const mafEvent &event_dictionary, mafList<mafVariant> *argList) const {
    REQUIRE(m_NetworkConnectorClient != NULL);
    mafId id_val = event_dictionary["EventId"].toInt();
    mafString id = mafIdProvider::instance()->idName(id_val);
    m_NetworkConnectorClient->send(id, argList);
}

