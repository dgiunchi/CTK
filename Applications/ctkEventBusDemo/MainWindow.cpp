#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <service/event/ctkEventBus.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(ctkEventBus *bus, QWidget *parent):
    QMainWindow(parent), m_EventBus(bus),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectEvents();
}


MainWindow::~MainWindow()
{
    delete handler;
    delete ui;
}

void MainWindow::connectEvents() {
    handler = new ctkEventDemo();
    connect(ui->btnSend, SIGNAL(released()), this, SLOT(sendEvent()));
    connect(handler, SIGNAL(updateMessageSignal(QString)), this, SLOT(updateMessage(QString)));
    connect(ui->connectButton, SIGNAL(released()), this, SLOT(connectClient()));

    m_EventBus->publishSignal(handler, "receiveEventSignal(mafList<mafVariant>)", "maf.remote.eventBus.comunication.receive.xmlrpc");
    m_EventBus->subscribeSlot(handler, "receiveEvent(mafList<mafVariant>)", "maf.remote.eventBus.comunication.receive.xmlrpc", ctkDictionary());
}

void MainWindow::sendEvent() {

    QString textToDisplay("Me: ");
    textToDisplay.append(ui->txtParameter->property("plainText").toString());
    ui->textBrowser->append(textToDisplay);

    // event bus starts here

    QList<QVariant> localEventList;
    localEventList.append("maf.remote.eventBus.comunication.receive.xmlrpc");

    QList<QVariant> dataList;
    dataList.append("myUser");
    dataList.append(ui->txtParameter->property("plainText").toString());

    ctkDictionary dic;

    dic.insert("localEvent",localEventList);
    dic.insert("localData",dataList);

    QString value = "maf.remote.eventBus.comunication.send.xmlrpc";
    ctkEvent event(value,dic);

    m_EventBus->sendEvent(event);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::updateMessage(QString message) {
    ui->textBrowser->append(message);
}

void MainWindow::connectClient() {
    m_EventBus->createServer("XMLRPC", ui->portLineEdit->text().toInt());
    m_EventBus->startListen();
    m_EventBus->createClient("XMLRPC", ui->hostLineEdit->text(), ui->portLineEdit->text().toInt());
}

void ctkEventDemo::receiveEvent(mafList<mafVariant> l) {
    QString value;
    value.append(l.at(0).toString());
    value.append(": ");
    value.append(l.at(1).toString());
    emit updateMessageSignal(value);
}
