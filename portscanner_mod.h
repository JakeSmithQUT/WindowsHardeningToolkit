#ifndef PORTSCANNER_MOD_H
#define PORTSCANNER_MOD_H
#include "portscanner_port.h"
#include <QProcess> // used for running powershell scripts
#include <QPlainTextEdit>
#include <QPlainTextEdit>
#include <QMainWindow>

class PortScanner_mod : public QObject {
    Q_OBJECT

public:
    explicit PortScanner_mod(QObject *parent = nullptr);
    bool StartsWithRegexPattern(const QString& line, QString sRegexPattern);
    QString RemoveRegexPattern(const QString& originalString, QString sRegexPattern);
    void ScanOpenTCPPorts();
    void ScanOpenUDPPorts();
    QMap<int, std::shared_ptr<portscanner_port>> getIPv6Loopback() const;
    QMap<int, std::shared_ptr<portscanner_port>> getIPv4Loopback() const;
    QMap<int, std::shared_ptr<portscanner_port>> getIPv6All() const;
    QMap<int, std::shared_ptr<portscanner_port>> getIPv4All() const;


private:
    //void initializeProtocolHash(QMap<int, QString> &protocolHash);
    //QString findProtocolDescription(const QMap<int, QString> &protocolHash, const QString &port);

    void InitializeProtocolHash(QHash<QString, QString> &protocolHash);
    QString FindProtocolDescription(const QHash<QString, QString> &protocolHash, const QString &sPort, const QString &sTransportProtocol="tcp");
    void OrderOpenedPorts(const QStringList &sReturnedPorts, const QString &sTransportProtocol);
    void runPowerShellScript(const QString &script);
    QString onScriptFinished(QProcess *process, QString &outputVariable);


    // Helper functions to insert values into the QHash variables
    void insertIPv6Loopback(const QString& key, const QString& value);
    void insertIPv4Loopback(const QString& key, const QString& value);
    void insertIPv6All(const QString& key, const QString& value);
    void insertIPv4All(const QString& key, const QString& value);


    // Class Variables
    QHash<QString, QString> hHashOfProtocols;
    // Port, Protocol
    QMap<int, std::shared_ptr<portscanner_port>> lst_IPv6_Loopback;
    QMap<int, std::shared_ptr<portscanner_port>> lst_IPv4_Loopback;
    QMap<int, std::shared_ptr<portscanner_port>> lst_IPv6_All;
    QMap<int, std::shared_ptr<portscanner_port>> lst_IPv4_All;

    // Need to fix this
    QMap<int, std::shared_ptr<portscanner_port>> lst_IPv6_Explicit;
    QMap<int, std::shared_ptr<portscanner_port>> lst_IPv4_Explicit;

signals:
    // Called when the port scan is completed
    void ipv6LoopbackUpdated(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void ipv4LoopbackUpdated(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void ipv6AllUpdated(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void ipv4AllUpdated(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void ipv6ExplicitUpdated(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void ipv4ExplicitUpdated(const QMap<int, std::shared_ptr<portscanner_port>> &data);


};

// Global Variables

#endif // PORTSCANNER_MOD_H

