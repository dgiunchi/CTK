/*
 *  mafEventDispatcherLocal.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventDispatcherLocal.h"
#include "mafEvent.h"

#include <mafIdProvider.h>

using namespace mafEventBus;
using namespace mafCore;

mafEventDispatcherLocal::mafEventDispatcherLocal(const mafString code_location) : mafEventDispatcher(code_location) {
    this->initializeGlobalEvents();
}

mafEventDispatcherLocal::~mafEventDispatcherLocal() {

}

void mafEventDispatcherLocal::initializeGlobalEvents() {
    mafId id = mafIdProvider::instance()->idValue("GLOBAL_UPDATE_EVENT");
    mafEvent *properties = mafNEW(mafEventBus::mafEvent);
    (*properties)["EventId"] =  (int) id;
    (*properties)["EventType"] = mafEventTypeLocal;
    (*properties)["SignatureType"] = mafSignatureTypeSignal;
    mafVariant var;
    var.setValue((QObject*)this);
    (*properties)["ObjectPointer"] = var;
    (*properties)["Signature"] = "notifyDefaultEvent()";
    registerSignal(*properties);

    Superclass::initializeGlobalEvents();
}

void mafEventDispatcherLocal::notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList, mafGenericReturnArgument *returnArg) const {
    mafId id = (int)event_dictionary["EventId"].toInt();
    mafEventItemListType items = signalItemProperty(id);
    mafEvent *itemEventProp;
    foreach(itemEventProp, items) {
        if((*itemEventProp)["Signature"].toString().length() != 0) {
            mafString signal_to_emit = (*itemEventProp)["Signature"].toString().split("(")[0];
            QObject *obj = (*itemEventProp)["ObjectPointer"].value<QObject *>();
            if(argList != NULL) {
                if (returnArg == NULL || returnArg->data() == NULL) { //don't use return value
                    switch (argList->count()) {
                        case 1:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                            argList->at(0));
                            break;
                        case 2:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1));
                            break;
                        case 3:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2));
                            break;
                        case 4:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3));
                            break;
                        case 5:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4));
                            break;
                        case 6:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5));
                            break;
                        case 7:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6));
                            break;
                        case 8:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7));
                            break;
                        case 9:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8));
                            break;
                        case 10:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8), argList->at(9));
                            break;
                        default:
                            mafMsgWarning("%s", mafTr("Number of arguments not supported. Max 10 arguments").toAscii().data());
                    } //switch
                 } else { //use return value
                    switch (argList->count()) {
                        case 1:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg,\
                            argList->at(0));
                            break;
                        case 2:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1));
                            break;
                        case 3:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2));
                            break;
                        case 4:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3));
                            break;
                        case 5:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4));
                            break;
                        case 6:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), argList->at(5));
                            break;
                        case 7:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6));
                            break;
                        case 8:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7));
                            break;
                        case 9:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8));
                            break;
                        case 10:
                            this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg, \
                             argList->at(0), argList->at(1), argList->at(2), argList->at(3), argList->at(4), \
                             argList->at(5), argList->at(6), argList->at(7), argList->at(8), argList->at(9));
                            break;
                        default:
                            mafMsgWarning("%s", mafTr("Number of arguments not supported. Max 10 arguments").toAscii().data());
                    } //switch
                 }
            } else {
                if (returnArg == NULL || returnArg->data() == NULL) { //don't use return value
                    this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii());
                } else {
                    this->metaObject()->invokeMethod(obj, signal_to_emit.toAscii(), *returnArg);
                }

            }
        }
    }
}

