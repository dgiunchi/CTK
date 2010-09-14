/*
 *  mafEventDispatcher.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventDispatcher.h"
#include "mafEvent.h"
#include <mafContracts.h>

#define CALLBACK_SIGNATURE "1"
#define SIGNAL_SIGNATURE   "2"

using namespace mafEventBus;
using namespace mafCore;

mafEventDispatcher::mafEventDispatcher(const mafString code_location) : mafObjectBase(code_location) {

}

mafEventDispatcher::~mafEventDispatcher() {
    // delete all lists present into the hash.
    mafHash<mafCore::mafId, mafEvent *>::iterator i;
    for (i = m_CallbacksHash.begin(); i != m_CallbacksHash.end(); ++i) {
        mafDEL(i.value());
    }
    m_CallbacksHash.clear();

    for (i = m_SignalsHash.begin(); i != m_SignalsHash.end(); ++i) {
        mafDEL(i.value());
    }
    m_SignalsHash.clear();
}

void mafEventDispatcher::initializeGlobalEvents() {
    //events notified when remote communication is ok or failed
    mafId done_id = mafIdProvider::instance()->idValue("REMOTE_COMMUNICATION_DONE");
    if(done_id == -1) {
        done_id = mafIdProvider::instance()->createNewId("REMOTE_COMMUNICATION_DONE");
    }
    mafEvent *remote_done = mafNEW(mafEventBus::mafEvent);
    (*remote_done)["EventId"] = (int)done_id;
    (*remote_done)["EventType"] = mafEventTypeLocal;
    (*remote_done)["SignatureType"] = mafSignatureTypeSignal;
    mafVariant var;
    var.setValue((QObject*)this);
    (*remote_done)["ObjectPointer"] = var;
    (*remote_done)["Signature"] = "remoteCommunicationDone()";
    this->registerSignal(*remote_done);

    mafId fail_id = mafIdProvider::instance()->idValue("REMOTE_COMMUNICATION_FAILED");
    if(fail_id == -1) {
        fail_id = mafIdProvider::instance()->createNewId("REMOTE_COMMUNICATION_FAILED");
    }
    mafEvent *remote_failed = mafNEW(mafEventBus::mafEvent);
    (*remote_failed)["EventId"] = (int)fail_id;
    (*remote_failed)["EventType"] = mafEventTypeLocal;
    (*remote_failed)["SignatureType"] = mafSignatureTypeSignal;
    var.setValue((QObject*)this);
    (*remote_failed)["ObjectPointer"] = var;
    (*remote_failed)["Signature"] = "remoteCommunicationFailed()";
    this->registerSignal(*remote_failed);
}

bool mafEventDispatcher::isSignaturePresent(const mafEvent &props) const {
    mafId id = props["EventId"].toInt();
    mafEventItemListType itemEventPropList;
    mafEvent *itemEventProp;
    if(props["SignatureType"].toInt() == mafSignatureTypeCallback) {
        itemEventPropList = m_CallbacksHash.values(id);
    } else {
        itemEventPropList = m_SignalsHash.values(id);
    }

    QObject *objParameter = props["ObjectPointer"].value<QObject *>();
    foreach(itemEventProp, itemEventPropList) {
        QObject *objInList = (*itemEventProp)["ObjectPointer"].value<QObject *>();
        if(objInList == objParameter && (*itemEventProp)["Signature"].toString() == props["Signature"].toString()) {
            return true;
        }
    }
    return false;
}

bool mafEventDispatcher::disconnectSignal(const mafEvent &props) {
    mafEventItemListType observerIdList = m_CallbacksHash.values(props["EventId"].toInt());
    mafEvent *item;
    bool result = true;
    foreach(item, observerIdList) {
        result = result && disconnectCallback(*item);
    }
    return result;
}

bool mafEventDispatcher::disconnectCallback(const mafEvent &props) {
    //need to disconnect observer from the signal
    mafString observer_sig = CALLBACK_SIGNATURE;
    observer_sig.append(props["Signature"].toString());

    mafId v = props["EventId"].toInt();
    mafEvent *itemSignal = m_SignalsHash.value(v);
    mafString event_sig = SIGNAL_SIGNATURE;
    event_sig.append((*itemSignal)["Signature"].toString());

    QObject *objSignal = (*itemSignal)["ObjectPointer"].value<QObject *>();
    QObject *objSlot = props["ObjectPointer"].value<QObject *>();

    return disconnect(objSignal, event_sig.toAscii(), objSlot, observer_sig.toAscii());
}

bool mafEventDispatcher::removeEventItem(const mafEvent &props) {
    bool isDisconnected = false;
    bool isPresent = isSignaturePresent(props);
    if(isPresent == true) {
        //mafEventItemListType itemEventPropList;
        if(props["SignatureType"].toInt() == mafSignatureTypeCallback) {
            isDisconnected = disconnectCallback(props);
            //iterator for erasing hash entry
            mafEventsHashType::iterator i = m_CallbacksHash.find(props["EventId"].toInt());
            while (i != m_CallbacksHash.end() && i.key() == props["EventId"].toInt()) {
                QObject *obj = (*(i.value()))["ObjectPointer"].value<QObject *>();
                QObject *objCheck = props["ObjectPointer"].value<QObject *>();
                if (obj == objCheck && (*(i.value()))["Signature"].toString() == props["Signature"].toString()) {
                    mafDEL(i.value());
                    i = m_CallbacksHash.erase(i);
                } else {
                    ++i;
                }
            }
        } else {
            //itemEventPropList = m_SignalsHash.values();
            isDisconnected = disconnectSignal(props);
            mafEventsHashType::iterator i = m_CallbacksHash.find(props["EventId"].toInt());
            while (i != m_CallbacksHash.end() && i.key() == props["EventId"].toInt()) {
                mafDEL(i.value());
                i++;
            }
            i = m_SignalsHash.find(props["EventId"].toInt());
            while (i != m_SignalsHash.end() && i.key() == props["EventId"].toInt()) {
                mafDEL(i.value());
                i++;
            }
            m_SignalsHash.remove(props["EventId"].toInt()); //in signal hash the id is unique
            m_CallbacksHash.remove(props["EventId"].toInt()); //remove also all the id associated in callback
        }

        //itemEventPropList.removeAt(idx);
    }
    return isDisconnected;
}

bool mafEventDispatcher::addObserver(const mafEvent &props) {
    QObject *obj = props["ObjectPointer"].value<QObject *>();
    REQUIRE(obj != NULL);

    mafId id = props["EventId"].toInt();
    // check if the object has been already registered with the same signature to avoid duplicates.
    if(m_CallbacksHash.contains(id) && this->isSignaturePresent(props) == true) {
        return false;
    }

    mafEvent *itemEventProp;
    itemEventProp = m_SignalsHash.value(id);
    mafVariant sigVariant = (*itemEventProp)["Signature"];
    mafString sig = sigVariant.toString();
    if(sig.length() > 0) {
        mafString observer_sig = CALLBACK_SIGNATURE;
        observer_sig.append(props["Signature"].toString());

        mafString event_sig = SIGNAL_SIGNATURE;
        event_sig.append(sig);

        // Add the new observer to the Hash.
        mafEvent *dict = const_cast<mafEvent *>(&props);
        this->m_CallbacksHash.insertMulti(id, dict);
        QObject *objSignal = (*itemEventProp)["ObjectPointer"].value<QObject *>();
        QObject *objSlot = props["ObjectPointer"].value<QObject *>();
        return connect(objSignal, event_sig.toAscii(), objSlot, observer_sig.toAscii());
    }
    return false;
}

bool mafEventDispatcher::removeObserver(const mafEvent &props) {
    QObject *objSlot = props["ObjectPointer"].value<QObject *>();
    if (objSlot == NULL) {
        // remove all observer for that 'id'
        bool result = disconnectSignal(props);

        int num = m_CallbacksHash.remove(props["EventId"].toInt());
        return result && num > 0;
    }

    return removeEventItem(props);
}

bool mafEventDispatcher::registerSignal(const mafEvent &props) {
    // check if the object has been already registered with the same signature to avoid duplicates.
    if(props["Signature"].toString().length() == 0) {
        QVariant var;
        var.setValue((QObject *)this);
        props["ObjectPointer"] = var;
        props["Signature"] = "notifyDefaultEvent()";
    }

    mafId id = props["EventId"].toInt();
    // Check if a signal (corresponding to a mafID) already is present.
    if(m_SignalsHash.contains(id)) {// && (this->isSignaturePresent(signal_props) == true)) {
        // Only one signal for a given id can be registered!!
        QObject *obj = props["ObjectPointer"].value<QObject *>();
        mafMsgWarning("%s", mafTr("Object %1 is trying to register a signal with ID '%2' that has been already registered!!").arg(obj->metaObject()->className(), mafIdProvider::instance()->idName(id)).toAscii().data());
        return false;
    }

    // Add the new signal to the Hash.
    mafEvent *dict = const_cast<mafEvent *>(&props);
    this->m_SignalsHash.insert(id, dict);
    return true;
}

bool mafEventDispatcher::removeSignal(const mafEvent &props) {
    QObject *obj = props["ObjectPointer"].value<QObject *>();
    if(obj == NULL) {
        // Remove all events corresponding to particular id
        int num = m_SignalsHash.remove(props["Id"].toInt());
        return num > 0;
    }

    return removeEventItem(props);
}

void mafEventDispatcher::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    Q_UNUSED(event_dictionary);
    Q_UNUSED(argList);
    Q_UNUSED(returnArg);
}
