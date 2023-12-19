#include "windowstoolkitmain.h"
#include "./ui_windowstoolkitmain.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QListWidget>
#include <Qfile>
#include <QTextStream>
#include <QHash>
#include <QProcess> // used for running powershell scripts

QHash<QString, QString> hHashOfProtocols;


// UI Setup
WindowsToolkitMain::WindowsToolkitMain(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WindowsToolkitMain){

    ui->setupUi(this);

    // TODO: Functionality to get updated protocol mapping from https://svn.nmap.org/nmap/nmap-services
    // Make a dictionary to quickly lookup protocols
    initializeProtocolHash(hHashOfProtocols);

}

// Trash Collection of UI when closed.
WindowsToolkitMain::~WindowsToolkitMain(){
    delete ui;
}

// Runs a powershell script as given by a string input.
// Params: param1 - Powershell Script to be executed as a QString.
// Author: Jake Smith  - 15.12.23
void WindowsToolkitMain::runPowerShellScript(const QString &script) {
    // Declare a new process
    QProcess *process = new QProcess(this);

    // Command to run PowerShell script from a QString
    QString command = "powershell -Command \"" + script + "\"";

    // Start the command in the declared process.
    process->start(command);
}

// Initialises a protocol hash
// Params: param1 - takes the QHash variable as an input. Variable is manipulated directly.
// Author: Jake Smith  - 18.12.23
void WindowsToolkitMain::initializeProtocolHash(QHash<QString, QString> &protocolHash) {

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
QString WindowsToolkitMain::findProtocolDescription(const QHash<QString, QString> &protocolHash, const QString &port) {
    return protocolHash.value(port, "Unknown Protocol");
}

// Code which runs once a script has finished executing.
// Params: param1 - Powershell Script to be executed as a QString.
// Author: Jake Smith  - 15.12.23
QString WindowsToolkitMain::onScriptFinished(QProcess *process, QString &outputVariable) {
    // Check that the process exists.
    if (process) {
        // Write the output of a process to a QString Variable and return
        return(process->readAllStandardOutput());
    }
    // Return empty QString variable if no process.
    return QString();
}

// Function Associated with the Port Scan button. run when clicked by the user
void WindowsToolkitMain::on_btn_OpenPortScan_clicked()
{
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

    // Change textbox to say "Scanning Ports..."
    ui->tb_OpenPortsOutput->setPlainText("Scanning Ports...");

    // Test add item to list widget
    //ui->lst_IPv6_Loopback->addItem(new QListWidgetItem("Port Item Test"));

    // Connect to a new process and execute a function
    connect(process, &QProcess::finished, [this, process, &scriptOutput]() {
        // Process returned data.
        QString sScriptReturn = onScriptFinished(process, scriptOutput);

        // Qt string List
        QStringList sReturnedPorts = sScriptReturn.split('\n', Qt::SkipEmptyParts);

        // Send UI Update to the main thread
        QMetaObject::invokeMethod(this, "update_tb_OpenPortsOutput", Qt::QueuedConnection, Q_ARG(QString, sScriptReturn), Q_ARG(QStringList, sReturnedPorts));

    });
    // Construct full Windows command (In this case powershell)
    QString command = "powershell -Command \"" + sScript_TCP + "\"";
    // Execute command.
    process->start(command);
}

// Function called to execute on main thread. Simply updates text box info.
void WindowsToolkitMain::update_tb_OpenPortsOutput(const QString &text, const QStringList &sReturnedPorts) {
    // Set Raw Output
    ui->tb_OpenPortsOutput->setPlainText(text.trimmed());

    QString sPort;
    QString sProtocolDesc;

    // Loop through the list
    foreach (QString line, sReturnedPorts) {
        // If IPv6
        if (line.contains(":")){
            // Check if Loopback (::1)
            if (line.contains("::1")){
                // Add to loopback
                sPort = line.replace("::1","").trimmed()+"/tcp";
                sProtocolDesc = findProtocolDescription(hHashOfProtocols,sPort);
                ui->lst_IPv6_Loopback->insertItem(0, new QListWidgetItem(line.replace("::1","").trimmed() + " - [" + sProtocolDesc + "]"));

            // Else Check if Open (::)
            } else if (line.contains("::")){
                // Add to open list
                sPort = line.replace("::","").trimmed()+"/tcp";
                sProtocolDesc = findProtocolDescription(hHashOfProtocols,sPort);
                ui->lst_IPv6_All->insertItem(0, new QListWidgetItem(line.replace("::","").trimmed() + " - [" + sProtocolDesc + "]"));
            // Else Check if Explicit
            } else {
                // It is explicit so add to explicit list.

                // NEED TO IMPLEMENT
            }

        // Else if IPv4
        } else if (line.contains(".")){
            // Check if Loopback (127.0.0.x)

            // Else Check if Open (xx.xx.xx.xx)

            // Else Check if Explicit

        }
        // Else do nothing
    }


}


