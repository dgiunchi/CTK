#ifndef CTKEVENTBUSIMPL_H
#define CTKEVENTBUSIMPL_H

#include <EventBus/ctkEventBus.h>

#include "mafEventBus/mafEventBusManager.h"

#include <QList>
#include <QHash>
#include <QSet>

//class forward
class ctkEventHandlerWrapper;


class ctkEventBusImpl : public QObject,
                     public ctkEventBus
{
  Q_OBJECT
  Q_INTERFACES(ctkEventBus)

public:

  static ctkEventBusImpl* instance();

  void postEvent(const ctkEvent& event);
  void sendEvent(const ctkEvent& event);

  void updateProperties(const QString& subsriptionId, const ctkProperties& properties);
  
  void publishSignal(const QObject* publisher, const char* signal,
                             const QString& signal_topic, Qt::ConnectionType type = Qt::QueuedConnection);
//  void publishSignal(const QObject* publisher, const char* signal, const char* signal_topic);

  QString subscribeSlot(const QObject* subscriber, const char* member, const ctkProperties& properties);
//  void subscribeSlot(const QObject* subscriber, const char* member, const ctkProperties& properties);

protected:

  typedef QList<ctkEventHandlerWrapper*> HandlerList;

  HandlerList globalWildcard;

  QHash<QString, HandlerList> topicName;

  void dispatchEvent(const ctkEvent& event, bool isAsync);

  void bucket(ctkEventHandlerWrapper* wrapper);

  QSet<ctkEventHandlerWrapper*> handlers(const QString& topic);

private:

  ctkEventBusImpl();
  mafEventBus::mafEventBusManager *m_MafEventBusManager;
};

#endif // CTKEVENTBUSIMPL_H
