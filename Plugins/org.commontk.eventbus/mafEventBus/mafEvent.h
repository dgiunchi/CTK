/*
 *  mafEvent.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009-2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENT_H
#define MAFEVENT_H

#include <mafDictionary.h>
#include "mafEventDefinitions.h"

namespace mafEventBus {

 /**
Class name: mafEvent
This class defines the MAF3 Event which inherit from mafDictionary, and contains
constructor for rapid dictionary creation.
@sa mafDictionary
*/
class MAFEVENTBUSSHARED_EXPORT mafEvent : public mafCore::mafDictionary {

public:
    /// Object constructor.
    mafEvent(const mafString code_location = "");

    /// Overload object constructor.
    mafEvent(mafCore::mafId id, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, mafString signature, const mafString code_location = "");

    /// Allow to assign the event type: mafEventTypeLocal or mafEventTypeRemote.
    void setEventType(mafEventType et);

    /// Return the type of the event: mafEventTypeLocal or mafEventTypeRemote.
    mafEventType eventType() const;

    /// Check if the event is local or not.
    bool isEventLocal() const;

    /// Allow to set or modify the event ID
    void setEventId(mafCore::mafId id);

    /// Return the Id associated with the event.
    mafCore::mafId eventId() const;

    /// Return the name associated to the numeric Id.
    mafString eventIdName() const;
};

typedef mafEvent * mafEventPointer;

inline mafEventType mafEvent::eventType() const {
    return (mafEventType)entries()->value("EventType").toInt();
}

inline mafCore::mafId mafEvent::eventId() const {
    return (mafCore::mafId)entries()->value("EventId").toInt();
}

inline bool mafEvent::isEventLocal() const {
    int et = entries()->value("EventType").toInt();
    return et == mafEventTypeLocal;
}

inline void mafEvent::setEventType(mafEventType et) {
    entries()->insert("EventType", et);
}

inline void mafEvent::setEventId(mafCore::mafId id) {
    entries()->insert("EventId", (int)id);
}

inline mafString mafEvent::eventIdName() const {
    mafCore::mafId id = eventId();
    return mafCore::mafIdProvider::instance()->idName(id);
}

} // namespace mafEventBus

Q_DECLARE_METATYPE(mafEventBus::mafEventPointer);

#endif // MAFEVENT_H
