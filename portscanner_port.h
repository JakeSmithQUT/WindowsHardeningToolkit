#ifndef PORTSCANNER_PORT_H
#define PORTSCANNER_PORT_H

#include <QObject>
#include <QString>

class portscanner_port : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(int iPort READ iPort WRITE SetPort)

public:
    explicit portscanner_port();

    int iPort() const;
    QString sProtocol() const;
    QString sService() const;
    QString sIpAddress() const;
    bool bIsExplicit() const;
    QString sExplicitIP() const;
    QString sTransportLayer() const;
    bool bTCPOpen() const;
    bool bUDPOpen() const;

    // Set
    void SetPort(const int &iPort);
    void SetProtocol(const QString &sProtocol="tcp");
    void SetService(const QString &sService="");
    void SetIsExplicit(const bool &bIsExplicit=false);
    void SetExplicitIP(const QString &sExplicitIP="");
    void SetTransportLayer(const QString &sTransportLayer="");
    void SetTCPOpen(const bool &bTCPOpen=false);
    void SetUDPOpen(const bool &bUDPOpen=false);


    // Get
//    QString GetPort();
//    QString GetProtocol();
//    QString GetService();
//    QString GetIsExplicit();

private:
    int m_iPort;
    QString m_sProtocol="Unknown";
    QString m_sService="";
    bool m_bIsExplicit=false;
    QString m_sExplicitIP="";
    QString m_sTransportLayer="";
    bool m_bTCPOpen=false;
    bool m_bUDPOpen=false;
};

#endif // PORTSCANNER_PORT_H
