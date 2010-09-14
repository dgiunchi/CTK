/*
 *  mafEventDefinitions.h
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENTDEFINITIONS_H
#define MAFEVENTDEFINITIONS_H

#include "mafEventBus_global.h"
#include <mafObjectFactory.h>
#include <mafIdProvider.h>
#include <mafObjectBase.h>

//defines
#define mafSoapMessage QtSoapMessage
#define mafSoapHttpTransport QtSoapHttpTransport
#define mafSoapType QtSoapType
#define mafSoapSimpleType QtSoapSimpleType
#define mafSoapArray QtSoapArray
#define mafSoapName QtSoapName
#define mafSoapQName QtSoapQName

#define mafRegisterLocalSignal(id, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent((int)id, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeSignal, (QObject*)sender, signature, mafCodeLocation); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the signal registration with ID '%1'.").arg(mafCore::mafIdProvider::instance()->idName(id)).toAscii().data());\
            mafDEL(properties); \
        }\
    }

#define mafRegisterRemoteSignal(id, sender, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent((int)id, mafEventBus::mafEventTypeRemote, mafEventBus::mafSignatureTypeSignal, (QObject*)sender, signature, mafCodeLocation); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the signal registration with ID '%1'.").arg(mafCore::mafIdProvider::instance()->idName(id)).toAscii().data());\
            mafDEL(properties); \
        }\
    }

#define mafRegisterLocalCallback(id, observer, signature) \
    {\
        mafEventBus::mafEvent *properties = new mafEventBus::mafEvent((int)id, mafEventBus::mafEventTypeLocal, mafEventBus::mafSignatureTypeCallback, (QObject*)observer, signature, mafCodeLocation); \
        bool ok =  mafEventBus::mafEventBusManager::instance()->addEventProperty(*properties);\
        if(!ok) {\
            mafMsgWarning("%s", mafTr("Some problem occourred during the callback registration with ID '%1'.").arg(mafCore::mafIdProvider::instance()->idName(id)).toAscii().data());\
            mafDEL(properties); \
        }\
    }


namespace mafEventBus {

/// Hash table that associate the communication protocol with the corresponding network connector class (Eg. XMLRPC, mafEventBus::mafNetworkConnectorQXMLRPC)
typedef mafHash<mafString, mafString> mafNetworkConnectorHash;

///< Enum that identify the mafEventType's type: Local or Remote.
typedef enum {
    mafEventTypeLocal,
    mafEventTypeRemote
} mafEventType;

///< Enum that identify the mafSignatureType's type: Signal or Callback.
typedef enum {
    mafSignatureTypeSignal = 0,
    mafSignatureTypeCallback = 1
} mafSignatureType;

/// List of the arguments to be sent through the event bus.
typedef mafList<QGenericArgument> mafEventArgumentsList;
typedef mafEventArgumentsList * mafEventArgumentsListPointer;
#define mafEventArgument(type, data) QArgument<type >(#type, data)
#define mafGenericReturnArgument QGenericReturnArgument
#define mafEventReturnArgument(type, data) QReturnArgument<type >(#type, data)

class mafEvent;

/// Types definitions for events' hash (to be more readable).
typedef mafHash<mafCore::mafId, mafEvent *> mafEventsHashType;

/// type definition for observers' properties list to be stored into the event's hash.
typedef mafList<mafEvent *> mafEventItemListType;

/// map which represent list of function to be registered in the server, with parameters
typedef mafMap<mafString, mafList<mafVariant::Type> >  mafRegisterMethodsMap;

} // namespace mafEventBus


Q_DECLARE_METATYPE(mafEventBus::mafEventArgumentsListPointer);
Q_DECLARE_METATYPE(mafEventBus::mafRegisterMethodsMap);

#endif // MAFEVENTDEFINITIONS_H
