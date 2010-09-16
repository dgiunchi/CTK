/*
 *  mafEventBusManager.h
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENTBUS_H
#define MAFEVENTBUS_H

// Includes list
#include "mafEventDefinitions.h"
#include "mafEventDispatcherLocal.h"
#include "mafEventDispatcherRemote.h"
#include "mafEvent.h"

namespace mafEventBus {

// Class forwarding list

/**
 Class name: mafEventBusManager
 This singletone provides the access point of the Communication Bus for MAF3 framework.
 mafEventBusManager defines also the mafId GLOBAL_UPDATE_EVENT to be used as generic update notification mechanism that the event bus can send to all the observers.
 */
class MAFEVENTBUSSHARED_EXPORT mafEventBusManager {
public:
    /// Return an instance of the event bus.
    static mafEventBusManager *instance();

    /// Add a new event property (observer or event) to the event bus hash.
    /** Return true if observer has beed added correctly, false otherwise.
    This method check before adding a new observer that it has not already been inserted into the events' Hash with the same id and callback signature.*/
    bool addEventProperty(const mafEvent &props) const;

    /// Remove the event property from the event bus hash.
    bool removeEventProperty(const mafEvent &props) const;

    /// Notify events associated to the given id locally to the application.
    void notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList = NULL, mafGenericReturnArgument *returnArg = NULL) const;

    /// Notify event associated to the given id locally to the application.
    //void notifyEvent(const mafString id, mafEventType ev_type = mafEventTypeLocal, mafEventArgumentsList *argList = NULL, mafGenericReturnArgument *returnArg = NULL) const;

    /// Notify event associated to the given id locally to the application.
    void notifyEvent(const mafString topic, mafEventType ev_type = mafEventTypeLocal, mafEventArgumentsList *argList = NULL, mafGenericReturnArgument *returnArg = NULL) const;

    /// Notify the event remotely by using the current active network connector given by the mafEventDispatcherRemote.
   //void notifyEventRemote(const mafString id, mafList<mafVariant> *argList = NULL) const;

    /// Enable/Disable event logging to allow dumping events notification into the selected logging output stream.
    void enableEventLogging(bool enable = true);

    /// When logging is enabled, allows logging events releted to specific id (require a valid topic).
    void logEventTopic(const mafString topic);

    /// When enabled, allows logging all events. It reset the value for m_LogEventId to -1 (the default)
    void logAllEvents();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// Retrieve if the signal has been registered previously.
    bool isSignalPresent(const mafString topic) const;

    /// Retrieve if the signal has been registered previously.
    //bool isSignalPresent(const mafString &id_name) const;

    /// Plug a new network connector into the connector hash for the given network protocol (protocol eg. "XMLRPC") (connector_type eg. "mafEventBus::mafNetworkConnectorQXMLRPC").
    void plugNetworkConnector(const mafString &protocol, mafNetworkConnector *connector);

    /// Create the server for remote communication according to the given protocol and listen port.
    bool createServer(const mafString &communication_protocol, unsigned int listen_port);

    /// Allow to start server listening.
    void startListen();

    /// Create the client for remote communication according to the given protocol, server host and port.
    bool createClient(const mafString &communication_protocol, const mafString &server_host, unsigned int port);

private:
    /// Object constructor.
    mafEventBusManager();

    /// Object destructor.
    ~mafEventBusManager();

    mafEventDispatcherLocal *m_LocalDispatcher; ///< Dispatcher class which dispatches events locally to the application.
    mafEventDispatcherRemote *m_RemoteDispatcher; ///< Dispatcher class dispatches events remotely to another applications or via network.

    bool m_EnableEventLogging; ///< Flag to enable/disable logging for event sent.
    mafString m_LogEventTopic; ///< Store the current Event_Id to track through the logger.
    mafNetworkConnectorHash m_NetworkConnectorHash; ///< Hash table used to store the association of network protocols and network connector types.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafEventBusManager::plugNetworkConnector(const mafString &protocol, mafNetworkConnector *connector) {
    m_NetworkConnectorHash.insert(protocol, connector);
}

inline bool mafEventBusManager::isSignalPresent(const mafString topic) const {
    return m_LocalDispatcher->isSignalPresent(topic);
}

/*inline bool mafEventBusManager::isSignalPresent(const mafString &id_name) const {
    return m_LocalDispatcher->isSignalPresent(id_name);
}*/

} // namespace mafEventBus

#endif // MAFEVENTBUS_H