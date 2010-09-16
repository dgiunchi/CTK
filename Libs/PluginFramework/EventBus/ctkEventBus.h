#ifndef CTKEVENTBUS_H
#define CTKEVENTBUS_H

#include "ctkEvent.h"


class CTK_PLUGINFW_EXPORT ctkEventBus {

public:

  typedef QMap<QString, QVariant> Properties;

  virtual ~ctkEventBus() {}

  virtual void postEvent(const ctkEvent& event) = 0;
  virtual void sendEvent(const ctkEvent& event) = 0;

  virtual void publishSignal(const QObject* publisher, const char* signal, const char* signal_topic) = 0;

  virtual void subscribeSlot(const QObject* subscriber, const char* member, const Properties& properties) = 0;

};


Q_DECLARE_INTERFACE(ctkEventBus, "org.commontk.core.ctkEventBus")

#endif // CTKEVENTBUS_H
