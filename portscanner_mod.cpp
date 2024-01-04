#include "portscanner_mod.h"
#include "portscanner_port.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QListWidget>
#include <Qfile>
#include <QTextStream>
#include <QHash>
#include <QRegularExpression>
#include <QProcess> // used for running powershell scripts


//Class Variables
QHash<QString, QString> hHashOfProtocols;
// Port, Protocol
QHash<QString, const portscanner_port> lst_IPv6_Loopback;
QHash<QString, const portscanner_port> lst_IPv4_Loopback;
QHash<QString, const portscanner_port> lst_IPv6_All;
QHash<QString, const portscanner_port> lst_IPv4_All;

// Need to fix this
QHash<QString, const portscanner_port> lst_IPv6_Explicit;
QHash<QString, const portscanner_port> lst_IPv4_Explicit;

// PortScanner mod Constructor
PortScanner_mod::PortScanner_mod(QObject *parent)
    : QObject(parent) {
    // TODO: Code to fetch latest nmap data to replace txt file.

    // Load protocol Descriptions from text file
    PortScanner_mod::InitializeProtocolHash(hHashOfProtocols);

}

// Run a powershell script to scan open TCP Ports
void PortScanner_mod::ScanOpenTCPPorts(){
    // DECLARATIONS
    QProcess *process = new QProcess(this);
    QString scriptOutput;

    // Powershell script used to get all open ports.
    QString sScript_TCP = R"(# Get all TCP connections
            $tcpConnections = Get-NetTCPConnection

            # Filter for 'Listen' state
            $listeningPorts = $tcpConnections | Where-Object { $_.State -eq 'Listen' }

            # If there are listening ports, display them
            if ($listeningPorts) {
                $listeningPorts | Format-Table -Property LocalAddress, LocalPort
            } else {
                Write-Host "No open ports in 'Listen' state found."
            }
            )";

    // Test add item to list widget
    //ui->lst_IPv6_Loopback->addItem(new QListWidgetItem("Port Item Test"));

    // Connect to a new process and execute a function
    QAction::connect(process, &QProcess::finished, [this, process, &scriptOutput]() {
        // Process returned data.
        QString sScriptReturn = onScriptFinished(process, scriptOutput);

        // Qt string List
        QStringList sReturnedPorts = sScriptReturn.split('\n', Qt::SkipEmptyParts);

        // Send UI Update to the main thread
        //QMetaObject::invokeMethod(this, "update_tb_OpenPortsOutput", Qt::QueuedConnection, Q_ARG(QString, sScriptReturn), Q_ARG(QStringList, sReturnedPorts));

    });
    // Construct full Windows command (In this case powershell)
    QString command = "powershell -Command \"" + sScript_TCP + "\"";
    // Execute command.
    process->start(command);
}

// Uses regex to see if a Qstring starts with an inputted Regex pattern
// Params: param1 - Inputted string to check
// Author: Jake Smith  - 19.12.23
bool PortScanner_mod::StartsWithRegexPattern(const QString& line, QString sRegexPattern) {
    // Regular expression to match loopback addresses (127.0.0.0 to 127.255.255.255)
    QRegularExpression regex(sRegexPattern);

    bool bRegexMatch = regex.match(line).hasMatch();

    // Check if the line starts with a loopback address
    return bRegexMatch;
}

// Uses regex to remove IPv4 Loopback address
// Params: param1 - Inputted string to remove address from
// Author: Jake Smith  - 19.12.23
QString PortScanner_mod::RemoveRegexPattern(const QString& originalString, QString sRegexPattern) {
    // Define a regular expression to match loopback addresses
    QRegularExpression regex(sRegexPattern);

    // Replace all occurrences of loopback addresses with an empty string
    QString stringWithoutLoopbacks = originalString;
    stringWithoutLoopbacks.replace(regex, "");
    stringWithoutLoopbacks = stringWithoutLoopbacks.trimmed();
    return stringWithoutLoopbacks;
}


// Initialises a protocol hash
// Params: param1 - takes the QHash variable as an input. Variable is manipulated directly.
// Author: Jake Smith  - 18.12.23
void PortScanner_mod::InitializeProtocolHash(QHash<QString, QString> &protocolHash) {

    // Get nmap-services.txt as a Qstring
    QString sFileContents;
    QStringList slFileContents;

    // Add protocol text to ouput for test
    QFile file(":/Protocols/nmap-services.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QTextStream in(&file);
         sFileContents = in.readAll();
        file.close();
    } else {
        // Handle the error, for example, log it
        qDebug() << "Error opening embedded file:" << file.errorString();
    }

    // Split file string into lines
    slFileContents = sFileContents.split("\n");
    // Iterate through all lines in the file.
    QStringList slFileLine;
    for (const QString& sFileLine : slFileContents){
        // If the line doesnt start with a #
        if(!sFileLine.trimmed().isEmpty() && sFileLine.at( 0 ) != '#'){
            // Split the line based on the tab delimiter
            slFileLine = sFileLine.split("\t");
            // Verify that the split has more than 2 strings in array
            if(slFileLine.length() > 2){
                // Add the first item in the array as the value, add the second as the string
                protocolHash.insert(slFileLine[1].trimmed(), slFileLine[0].trimmed());
            }
        }
    }
}


//  Finds the protocol name based on inputted portnum/TransportLayer
// Params: param1 - takes the QHash variable as an input.
// Params: param2 - portnum/TransportLayer as a string
// Author: Jake Smith  - 18.12.23
QString PortScanner_mod::FindProtocolDescription(const QHash<QString, QString> &protocolHash, const QString &port) {
    return protocolHash.value(port, "Unknown Protocol");
}



// Function called to execute on main thread. Simply updates text box info.
void PortScanner_mod::OrderOpenedPorts(const QString &text, const QStringList &sReturnedPorts) {
    // Declare Local Variables
    QString sPort;
    QString sProtocolDesc;

    // Loop through the list
    foreach (QString line, sReturnedPorts) {
        // If IPv6
        if (line.contains(":")){
            // Check if Loopback (::1)
            if (line.contains("::1")){
                // Add to loopback
                sPort = line.replace("::1","").trimmed();//+"/tcp";
                sProtocolDesc = FindProtocolDescription(hHashOfProtocols,sPort);
                // Create new port object
                portscanner_port *oPort = new portscanner_port();
                // set Properties
                oPort->SetPort(sPort.toInt());
                oPort->SetProtocol(sProtocolDesc);
                //TODO: Find service keeping port open

                // Add to iPv6 list
                lst_IPv6_Loopback.insert(sPort, *oPort);

            // Else Check if Open (::)
            } else if (line.contains("::")){
                // Add to open list
                sPort = line.replace("::","").trimmed()+"/tcp";
                sProtocolDesc = FindProtocolDescription(hHashOfProtocols,sPort);
                // Create new port object
                portscanner_port *oPort = new portscanner_port();

                oPort->SetPort(sPort.toInt());
                oPort->SetProtocol(sProtocolDesc);
                //TODO: Find service keeping port open

                // Add to iPv6 list
                lst_IPv6_All.insert(sPort, *oPort);
            // Else Check if Explicit
            } else if(StartsWithRegexPattern(line,"^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}")) { // Else Check if Explicit
                sPort = RemoveRegexPattern(line,"^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}")+"/tcp";
                sProtocolDesc = FindProtocolDescription(hHashOfProtocols,sPort);
                //ui->lst_IPv4_Explicit->insertItem(0, new QListWidgetItem(sPort + " - [" + sProtocolDesc + "]"));
                // NEED TO IMPLEMENT
            }

        // Else if IPv4
        } else if (line.contains(".")){
            if(line.startsWith("0.0.0.0")){ // Else Check if Open (0.0.0.0)
                sPort = line.replace("0.0.0.0","").trimmed()+"/tcp";
                sProtocolDesc = FindProtocolDescription(hHashOfProtocols,sPort);
                //ui->lst_IPv4_All->insertItem(0, new QListWidgetItem(sPort + " - [" + sProtocolDesc + "]"));
                // Declare new portscanner object
                portscanner_port *oPort = new portscanner_port;
                // Define port and convert to integer
                oPort->SetPort(sPort.toInt());
                oPort->SetProtocol(sProtocolDesc);
                //TODO: Find service keeping port open

                // Add to iPv4 List
                lst_IPv4_All.insert(sPort, *oPort);

            } else if (StartsWithRegexPattern(line,"^127\\.(\\d+)\\.(\\d+)\\.(\\d+)")){ // Check if Loopback (127.0.0.x)
                sPort = RemoveRegexPattern(line,"^127\\.(\\d+)\\.(\\d+)\\.(\\d+)")+"/tcp";
                sProtocolDesc = FindProtocolDescription(hHashOfProtocols,sPort);
                //ui->lst_IPv4_Loopback->insertItem(0, new QListWidgetItem(sPort + " - [" + sProtocolDesc + "]"));
                // Declare new portscanner object
                portscanner_port *oPort = new portscanner_port;
                // Define port and convert to integer
                oPort->SetPort(sPort.toInt());
                oPort->SetProtocol(sProtocolDesc);
                //TODO: Find service keeping port open

                // Add to iPv4 list
                lst_IPv4_Loopback.insert(sPort, *oPort);

            } else if(StartsWithRegexPattern(line,"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)")) { // Else Check if Explicit
                sPort = RemoveRegexPattern(line,"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)")+"/tcp";
                sProtocolDesc = FindProtocolDescription(hHashOfProtocols,sPort);
                //ui->lst_IPv4_Explicit->insertItem(0, new QListWidgetItem(line.trimmed().replace(QRegularExpression("\\s+"),":") + " - [" + sProtocolDesc + "]"));
                //lst_IPv4_Explicit.insert(sPort, sProtocolDesc);

            }

        }
        // Else do nothing
    }


}

// Runs a powershell script as given by a string input.
// Params: param1 - Powershell Script to be executed as a QString.
// Author: Jake Smith  - 15.12.23
void PortScanner_mod::runPowerShellScript(const QString &script) {
    // Declare a new process
    QProcess *process = new QProcess(this);

    // Command to run PowerShell script from a QString
    QString command = "powershell -Command \"" + script + "\"";

    // Start the command in the declared process.
    process->start(command);
}

// Code which runs once a script has finished executing.
// Params: param1 - Powershell Script to be executed as a QString.
// Author: Jake Smith  - 15.12.23
QString PortScanner_mod::onScriptFinished(QProcess *process, QString &outputVariable) {
    // Check that the process exists.
    if (process) {
        // Write the output of a process to a QString Variable and return
        return(process->readAllStandardOutput());
    }
    // Return empty QString variable if no process.
    return QString();
}


// Helper Functions

QHash<QString, portscanner_port> PortScanner_mod::getIPv6Loopback() const {
    return lst_IPv6_Loopback;
}

QHash<QString, portscanner_port> PortScanner_mod::getIPv4Loopback() const {
    return lst_IPv4_Loopback;
}

QHash<QString, portscanner_port> PortScanner_mod::getIPv6All() const {
    return lst_IPv6_All;
}

QHash<QString, portscanner_port> PortScanner_mod::getIPv4All() const {
    return lst_IPv4_All;
}





