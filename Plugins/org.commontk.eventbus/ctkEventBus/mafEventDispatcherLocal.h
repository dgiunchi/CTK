/*
 *  ctkEventDispatcherLocal.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 11/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef CTKEVENTDISPATCHERLOCAL_H
#define CTKEVENTDISPATCHERLOCAL_H

#include "mafEventDefinitions.h"
#include "mafEventDispatcher.h"

namespace mafEventBus {

/**
 Class name: ctkEventDispatcherLocal
 This allows dispatching events coming from local application to attached observers.
 */
class CTKEVENTBUSSHARED_EXPORT ctkEventDispatcherLocal : public ctkEventDispatcher {
    Q_OBJECT

public:
    /// object constructor.
    ctkEventDispatcherLocal();

    /// Emit event corresponding to the given id locally to the application.
    virtual void notifyEvent(const mafEvent &event_dictionary, mafEventArgumentsList *argList = NULL, mafGenericReturnArgument *returnArg = NULL) const;

protected:
    /// Register CTK global events
    /*virtual*/ void initializeGlobalEvents();

private:
};

}

#endif // CTKEVENTDISPATCHERLOCAL_H
