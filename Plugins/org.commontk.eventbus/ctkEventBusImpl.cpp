#include "ctkEventBusImpl_p.h"

#include <QSetIterator>
#include "ctkEventHandlerWrapper_p.h"


//using namespace mafCore;
using namespace mafEventBus;

ctkEventBusImpl* ctkEventBusImpl::instance()
{
  static ctkEventBusImpl inst;
  return &inst;
}

ctkEventBusImpl::ctkEventBusImpl()
{

}

void ctkEventBusImpl::postEvent(const ctkEvent& event)
{
  dispatchEvent(event, true);
}

void ctkEventBusImpl::sendEvent(const ctkEvent& event)
{
  dispatchEvent(event, false);
}

void ctkEventBusImpl::publishSignal(const QObject* publisher, const char* signal, const QString& signal_topic, Qt::ConnectionType type) {
    // event bus addproperty Event
    //need a TOPIC
    mafEvent *event = new mafEvent(signal_topic, mafEventTypeLocal, mafSignatureTypeSignal, const_cast<QObject *>(publisher), signal);
    m_MafEventBusManager->addEventProperty(*event);
}

QString ctkEventBusImpl::subscribeSlot(const QObject* subscriber, const char* member, const ctkProperties& properties) {
    QStringList topicList;
    QVariant v = properties[EventConstants::EVENT_TOPIC];
    topicList = v.toStringList();

    QStringListIterator i(topicList);
    while (i.hasNext()) {
        mafEvent *mesbEvent = new mafEvent(i.next(), mafEventTypeLocal, mafSignatureTypeCallback, const_cast<QObject *>(subscriber), member);
        m_MafEventBusManager->addEventProperty(*mesbEvent);
    }

	// to be changed!!!!!!
	return QString("");
  // TODO check for duplicates

  /*ctkEventHandlerWrapper* wrapper = new ctkEventHandlerWrapper(subscriber, member, properties);
  if (wrapper->init())
  {
    bucket(wrapper);
  }*/
}

void ctkEventBusImpl::updateProperties(const QString& subsriptionId, const ctkProperties& properties) {
	
}

void ctkEventBusImpl::dispatchEvent(const ctkEvent& event, bool isAsync)
{
  Q_UNUSED(isAsync)



  /*QString topic = event.topic();

  QSet<ctkEventHandlerWrapper*> eventHandlers = this->handlers(topic);
  if (eventHandlers.empty()) return;

  QSetIterator<ctkEventHandlerWrapper*> iter(eventHandlers);
  while (iter.hasNext())
  {
    iter.next()->handleEvent(event);
  }*/

  QString topic = event.topic(); //may contains widlcards

  mafEvent *mebEvent = new mafEvent();
  mebEvent->setEventTopic(topic);
  mebEvent->setEventType(mafEventTypeLocal);
  mebEvent->setEventFilter(NULL);
  //need to think about the arguments inside the event
  m_MafEventBusManager->notifyEvent(*mebEvent);

}

void ctkEventBusImpl::bucket(ctkEventHandlerWrapper* wrapper)
{
  // TODO bucket logic
  globalWildcard.push_back(wrapper);
}

QSet<ctkEventHandlerWrapper*> ctkEventBusImpl::handlers(const QString& topic)
{
  Q_UNUSED(topic)

  // TODO
  return globalWildcard.toSet();
}

