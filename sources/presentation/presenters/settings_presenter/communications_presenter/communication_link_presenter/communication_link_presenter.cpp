#include "communication_link_presenter.h"

// Qt
#include <QMap>
#include <QVariant>
#include <QSerialPortInfo>
#include <QDebug>

// Internal
#include "communication_manager.h"
#include "link_description.h"

using namespace presentation;

CommunicationLinkPresenter::CommunicationLinkPresenter(
        domain::CommunicationManager* manager,
        const data_source::LinkDescriptionPtr& description,
        QObject* parent):
    BasePresenter(parent),
    m_manager(manager),
    m_description(description)
{
    connect(m_manager, &domain::CommunicationManager::linkChanged, this,
            [this](const data_source::LinkDescriptionPtr& description) {
        if (m_description == description) this->updateView();
    });
}

data_source::LinkDescriptionPtr CommunicationLinkPresenter::description() const
{
    return m_description;
}

void CommunicationLinkPresenter::updateView()
{
    this->setViewSignalsEnbled(false);

    this->setViewProperty(PROPERTY(type), m_description->type());
    this->setViewProperty(PROPERTY(name), m_description->name());
    this->setViewProperty(PROPERTY(port), m_description->port());
    this->setViewProperty(PROPERTY(device), m_description->device());
    this->setViewProperty(PROPERTY(baudRate), m_description->baudRate());
    this->setViewProperty(PROPERTY(connected), m_description->isAutoConnect());

    this->setViewSignalsEnbled(true);
}

void CommunicationLinkPresenter::connectView(QObject* view)
{
    QStringList devices;
    for (const QSerialPortInfo& info: QSerialPortInfo::availablePorts())
        devices.append(info.portName());
    this->setViewProperty(PROPERTY(devices), devices);

    QVariantList baudRates;
    for (qint32 rate: QSerialPortInfo::standardBaudRates())
        baudRates.append(rate);
    this->setViewProperty(PROPERTY(baudRates), baudRates);

    this->updateView();
}

void CommunicationLinkPresenter::setViewSignalsEnbled(bool enabled)
{
    if (enabled)
    {
        connect(m_view, SIGNAL(setName(QString)), this, SLOT(onSetName(QString)));
        connect(m_view, SIGNAL(setPort(int)), this, SLOT(onSetPort(int)));
        connect(m_view, SIGNAL(setDevice(QString)), this, SLOT(onSetDevice(QString)));
        connect(m_view, SIGNAL(setBaudRate(int)), this, SLOT(onSetBaudRate(int)));
        connect(m_view, SIGNAL(setConnected(bool)), this, SLOT(onSetConnected(bool)));
        connect(m_view, SIGNAL(remove()), this, SLOT(onRemove()));
    }
    else
    {
        disconnect(m_view, 0, this, 0);
    }
}

void CommunicationLinkPresenter::onSetName(const QString& name)
{
    if (m_description->name() == name) return;

    m_description->setName(name);
    m_manager->saveLink(m_description);
}

void CommunicationLinkPresenter::onSetPort(int port)
{
    if (m_description->port() == port) return;

    m_description->setPort(port);
    m_manager->saveLink(m_description);
}

void CommunicationLinkPresenter::onSetDevice(const QString& device)
{
    if (m_description->device() == device) return;

    m_description->setDevice(device);
    m_manager->saveLink(m_description);
}

void CommunicationLinkPresenter::onSetBaudRate(int rate)
{
    if (m_description->baudRate() == rate) return;

    m_description->setBaudRate(rate);
    m_manager->saveLink(m_description);
}

void CommunicationLinkPresenter::onSetConnected(bool connected)
{
    if (m_description->isAutoConnect() == connected) return;

    m_description->setAutoConnect(connected);
    m_manager->saveLink(m_description);
}

void CommunicationLinkPresenter::onRemove()
{
    m_manager->removeLink(m_description);
}
