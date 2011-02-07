/*
 *  mafEventDispatcherRemote.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef CTKEVENTDISPATCHERREMOTE_H
#define CTKEVENTDISPATCHERREMOTE_H

//include list
#include "ctkEventDefinitions.h"
#include "ctkEventDispatcher.h"

namespace mafEventBus {

class ctkNetworkConnector;

/**
 Class name: mafEventDispatcherRemote
 This allows dispatching events coming from local application to attached observers.
 */
class CTKEVENTBUSSHARED_EXPORT mafEventDispatcherRemote : public mafEventDispatcher {
    Q_OBJECT

public:
    /// object constructor.
    mafEventDispatcherRemote();

    /// object destructor.
    /*virtual*/ ~mafEventDispatcherRemote();

    /// Emit event corresponding to the given id (present into the event_dictionary) locally to the application.
    /*virtual*/ void notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList = NULL, mafGenericReturnArgument *returnArg = NULL) const;

    /// Set the active server side network connector type.
    void setNetworkConnectorServer(ctkNetworkConnector *connector);

    /// return the current instantiated server side network connector.
    ctkNetworkConnector *networkConnectorServer();

    /// Set the active client side network connector type.
    void setNetworkConnectorClient(ctkNetworkConnector *connector);

    /// return the current instantiated client side network connector.
    ctkNetworkConnector *networkConnectorClient();

protected:
    /// Register CTK global events
    /*virtual*/ void initializeGlobalEvents();

private:
    ctkNetworkConnector *m_NetworkConnectorServer; ///< Class used to manage the communication with the remote event bus.
    ctkNetworkConnector *m_NetworkConnectorClient; ///< Class used to manage the communication with the remote event bus.
};


} //namespace mafEventBus

Q_DECLARE_METATYPE(QVariantList);

#endif // CTKEVENTDISPATCHERREMOTE_H
