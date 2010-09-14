/*
 *  mafEventBusRegistration.h
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENTBUSREGISTRATION_H
#define MAFEVENTBUSREGISTRATION_H

// Includes list
#include "mafEventDefinitions.h"
#include "mafEventDispatcher.h"
#include "mafEventDispatcherLocal.h"
#include "mafEventDispatcherRemote.h"
//#include "mafNetworkConnectorQXMLRPC.h"
#include "mafNetworkConnectorQtSoap.h"
#include "mafNetworkConnector.h"
#include <mafObjectFactory.h>

namespace mafEventBus {

/**
 Class name: mafEventBusRegistration
 This class allows object rigisteration for all the classes present in mafEventBus module.
 @sa mafObjectFactory
 */
class MAFEVENTBUSSHARED_EXPORT mafEventBusRegistration {
    public:
    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in objectFactory which needs as input the object type to instantiate as string.*/
    static void registerEventBusObjects() {
        qRegisterMetaType<mafRegisterMethodsMap>("mafRegisterMethodsMap");
        mafRegisterObject(mafEventBus::mafEventDispatcher);
        mafRegisterObject(mafEventBus::mafEventDispatcherLocal);
        mafRegisterObject(mafEventBus::mafEventDispatcherRemote);
        //mafRegisterObject(mafEventBus::mafNetworkConnectorQXMLRPC);
        mafRegisterObject(mafEventBus::mafNetworkConnectorQtSoap);
    }
};

} // namespace mafEventBus

#endif // MAFEVENTBUSREGISTRATION_H
