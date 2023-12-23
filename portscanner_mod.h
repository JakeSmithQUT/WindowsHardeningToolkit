#ifndef PORTSCANNER_MOD_H
#define PORTSCANNER_MOD_H
#include <QProcess> // used for running powershell scripts
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
    QHash<QString, QString> getIPv6Loopback() const;
    QHash<QString, QString> getIPv4Loopback() const;
    QHash<QString, QString> getIPv6All() const;
    QHash<QString, QString> getIPv4All() const;


private:
    //void initializeProtocolHash(QHash<QString, QString> &protocolHash);
    //QString findProtocolDescription(const QHash<QString, QString> &protocolHash, const QString &port);

    void InitializeProtocolHash(QHash<QString, QString> &protocolHash);
    QString FindProtocolDescription(const QHash<QString, QString> &protocolHash, const QString &port);
    void OrderOpenedPorts(const QString &text, const QStringList &sReturnedPorts);
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
    QHash<QString, QString> lst_IPv6_Loopback;
    QHash<QString, QString> lst_IPv4_Loopback;
    QHash<QString, QString> lst_IPv6_All;
    QHash<QString, QString> lst_IPv4_All;

    // Need to fix this
    QHash<QString, QString> lst_IPv6_Explicit;
    QHash<QString, QString> lst_IPv4_Explicit;


};

// Global Variables

#endif // PORTSCANNER_MOD_H

