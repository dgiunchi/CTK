/*
 *  mafNetworkConnector.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFNETWORKCONNECTOR_H
#define MAFNETWORKCONNECTOR_H

//include list
#include "mafEventDefinitions.h"

namespace mafEventBus {

/**
 Class name: mafNetworkConnector
 This class is the interface class for client/server objects that works over network.
 */
class MAFEVENTBUSSHARED_EXPORT mafNetworkConnector : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// object constructor.
    mafNetworkConnector(const mafString code_location = "");

    /// create the unique instance of the client.
    virtual void createClient(const mafString hostName, const unsigned int port) = 0;

    /// create the unique instance of the server.
    virtual void createServer(const unsigned int port) = 0;

    /// Start the server.
    virtual void startListen() = 0;

    /// Allow to send a network request.
    virtual void send(const mafString &event_id, mafList<mafVariant> *params) = 0;
};

} //namespace mafEventBus

#endif // MAFNETWORKCONNECTOR_H
