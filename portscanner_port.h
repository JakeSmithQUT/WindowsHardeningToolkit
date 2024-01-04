#ifndef PORTSCANNER_PORT_H
#define PORTSCANNER_PORT_H

#include <QObject>
#include <QString>

class portscanner_port : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int iPort READ iPort WRITE SetPort)

public:
    explicit portscanner_port(QObject *parent = nullptr);

    int iPort() const;
    QString sProtocol() const;
    QString sService() const;
    QString sIpAddress() const;
    bool bIsExplicit() const;

    // Set
    void SetPort(const int &iPort);
    void SetProtocol(const QString &sProtocol="tcp");
    void SetService(const QString &sService="");
    void SetIsExplicit(const bool &bIsExplicit=false);


    // Get
    void GetPort(const int &iPort);
    void GetProtocol(const QString &sProtocol);
    void GetService(const QString &sService);
    void GetIsExplicit(const bool &bIsExplicit);

private:
    int m_iPort;
    QString m_sProtocol="Unknown";
    QString m_sService="";
    bool m_bIsExplicit=false;
};

#endif // PORTSCANNER_PORT_H
