#include "portscanner_port.h"
#include <QObject>
#include <QString>

portscanner_port::portscanner_port() : QObject(), m_iPort(0){ }

// ========= Port Get / Set =========
int portscanner_port::iPort() const {
    return m_iPort;
}

void portscanner_port::SetPort(const int &iPort) {
    if (iPort != m_iPort) {
        m_iPort = iPort;
    }
}


// ========= Protocol Get / Set =========
QString portscanner_port::sProtocol() const {
    return m_sProtocol;
}

void portscanner_port::SetProtocol(const QString &sProtocol) {
    if (sProtocol != m_sProtocol) {
        m_sProtocol = sProtocol;
    }
}


// ========= Service Get / Set =========
QString portscanner_port::sService() const {
    return m_sProtocol;
}

void portscanner_port::SetService(const QString &sService) {
    if (sService != m_sService) {
        m_sService = sService;
    }
}


// ========= Explicit Boolean Get / Set =========
bool portscanner_port::bIsExplicit() const {
    return m_bIsExplicit;
}

void portscanner_port::SetIsExplicit(const bool &bIsExplicit) {
    if (bIsExplicit != m_bIsExplicit) {
        m_bIsExplicit = bIsExplicit;
    }
}


// ========= Explicit IP Get / Set =========
QString portscanner_port::sExplicitIP() const {
    return m_sExplicitIP;
}

void portscanner_port::SetExplicitIP(const QString &sExplicitIP){
    if (sExplicitIP != m_sExplicitIP) {
        m_sExplicitIP = sExplicitIP;
    }
}


// ========= Transport Layer Protocol Get / Set =========
QString portscanner_port::sTransportLayer() const {
    return m_sTransportLayer;
}

void portscanner_port::SetTransportLayer(const QString &sTransportLayer) {
    if (sTransportLayer != m_sTransportLayer) {
        m_sTransportLayer = sTransportLayer;
    }
}
