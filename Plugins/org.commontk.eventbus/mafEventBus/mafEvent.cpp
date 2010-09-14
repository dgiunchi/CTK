/*
 *  mafEvent.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEvent.h"

using namespace mafCore;
using namespace mafEventBus;

mafEvent::mafEvent(const mafString code_location) : mafDictionary(code_location) {
}

/// Overload object constructor.
mafEvent::mafEvent(mafCore::mafId id, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, mafString signature, const mafString code_location) : mafDictionary(code_location) {
    entries()->insert("EventId", (int) id);
    entries()->insert("EventType", event_type);
    entries()->insert("SignatureType", signature_type);
    mafVariant var;
    var.setValue(objectPointer);
    entries()->insert("ObjectPointer", var);
    entries()->insert("Signature", signature);
}
