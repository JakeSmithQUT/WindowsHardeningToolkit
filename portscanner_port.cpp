#include "portscanner_port.h"
#include <QObject>
#include <QString>

portscanner_port::portscanner_port() : QObject(), m_iPort(0){ }

int portscanner_port::iPort() const {
    return m_iPort;
}

void portscanner_port::SetPort(const int &iPort) {
    if (iPort != m_iPort) {
        m_iPort = iPort;
    }
}

QString portscanner_port::sProtocol() const {
    return m_sProtocol;
}

void portscanner_port::SetProtocol(const QString &sProtocol) {
    if (sProtocol != m_sProtocol) {
        m_sProtocol = sProtocol;
    }
}

QString portscanner_port::sService() const {
    return m_sProtocol;
}

void portscanner_port::SetService(const QString &sService) {
    if (sService != m_sService) {
        m_sService = sService;
    }
}

bool portscanner_port::bIsExplicit() const {
    return m_bIsExplicit;
}

void portscanner_port::SetIsExplicit(const bool &bIsExplicit) {
    if (bIsExplicit != m_bIsExplicit) {
        m_bIsExplicit = bIsExplicit;
    }
}


QString portscanner_port::sTransportLayer() const {
    return m_sTransportLayer;
}

void portscanner_port::SetTransportLayer(const QString &sTransportLayer) {
    if (sTransportLayer != m_sTransportLayer) {
        m_sTransportLayer = sTransportLayer;
    }
}
