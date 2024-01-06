////////////  THIS IS THE CONTROLLER  ///////////////////


#include "windowstoolkitmain.h"
#include "./ui_windowstoolkitmain.h"
#include "portscanner_mod.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QListWidget>

PortScanner_mod* PortScanner;


// UI Setup
WindowsToolkitMain::WindowsToolkitMain(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::WindowsToolkitMain){

    ui->setupUi(this);

    // TODO: Functionality to get updated protocol mapping from https://svn.nmap.org/nmap/nmap-services
    // Make a dictionary to quickly lookup protocols

    PortScanner = new PortScanner_mod(this);// Assuming 'this' is an instance of MainWindow and 'portScanner' is an instance of PortScanner

    // Connect signals and slots
    connect(PortScanner, &PortScanner_mod::ipv6LoopbackUpdated, this, &WindowsToolkitMain::updateIPv6Loopback);
    connect(PortScanner, &PortScanner_mod::ipv4LoopbackUpdated, this, &WindowsToolkitMain::updateIPv4Loopback);
    connect(PortScanner, &PortScanner_mod::ipv6AllUpdated, this, &WindowsToolkitMain::updateIPv6All);
    connect(PortScanner, &PortScanner_mod::ipv4AllUpdated, this, &WindowsToolkitMain::updateIPv4All);
    connect(PortScanner, &PortScanner_mod::ipv6ExplicitUpdated, this, &WindowsToolkitMain::updateIPv6Explicit);
    connect(PortScanner, &PortScanner_mod::ipv4ExplicitUpdated, this, &WindowsToolkitMain::updateIPv4Explicit);


}


// Trash Collection of UI when closed.
WindowsToolkitMain::~WindowsToolkitMain(){
    delete ui;
}

// Function Associated with the Port Scan button. run when clicked by the user
void WindowsToolkitMain::on_btn_OpenPortScan_clicked()
{

    // Change textbox to say "Scanning Ports..."
    ui->tb_OpenPortsOutput->setPlainText("Scanning Ports...");

    // Clear Fields
    ui->lst_IPv6_Loopback->clear();
    ui->lst_IPv4_Loopback->clear();
    ui->lst_IPv6_All->clear();
    ui->lst_IPv4_All->clear();
    ui->lst_IPv4_Explicit->clear();
    ui->lst_IPv4_Explicit->clear();

    // Start Scanning Ports
    PortScanner->ScanOpenTCPPorts();

    // retrieve QHash maps and fill in UI Elements


}

void WindowsToolkitMain::updateIPv6Loopback(const QHash<QString, std::shared_ptr<portscanner_port>> &data) {
    // Update the UI with IPv6 Loopback data
    for (auto i = data.begin(); i  != data.end(); ++i) {
        QString sPortKey = i.key();
        std::shared_ptr<portscanner_port> oPort = i.value();
        QString sProtocol = oPort->sProtocol();
        QString sTransportLayer = oPort->sTransportLayer();


        ui->lst_IPv6_Loopback->addItem(QString("%1/%2 - %3").arg(sPortKey, sTransportLayer, sProtocol));
    }
}

void WindowsToolkitMain::updateIPv4Loopback(const QHash<QString, std::shared_ptr<portscanner_port>> &data) {
    // Update the UI with IPv4 Loopback data
    for (auto i = data.begin(); i  != data.end(); ++i) {
        QString sPortKey = i.key();
        std::shared_ptr<portscanner_port> oPort = i.value();
        QString sProtocol = oPort->sProtocol();
        QString sTransportLayer = oPort->sTransportLayer();


        ui->lst_IPv4_Loopback->addItem(QString("%1/%2 - %3").arg(sPortKey, sTransportLayer, sProtocol));
    }
}

void WindowsToolkitMain::updateIPv6All(const QHash<QString, std::shared_ptr<portscanner_port>> &data) {
    // Update the UI with IPv6 All data
    for (auto i = data.begin(); i  != data.end(); ++i) {
        QString sPortKey = i.key();
        std::shared_ptr<portscanner_port> oPort = i.value();
        QString sProtocol = oPort->sProtocol();
        QString sTransportLayer = oPort->sTransportLayer();


        ui->lst_IPv6_All->addItem(QString("%1/%2 - %3").arg(sPortKey, sTransportLayer, sProtocol));
    }
}

void WindowsToolkitMain::updateIPv4All(const QHash<QString, std::shared_ptr<portscanner_port>> &data) {
    // Update the UI with IPv4 All data
    for (auto i = data.begin(); i  != data.end(); ++i) {
        QString sPortKey = i.key();
        std::shared_ptr<portscanner_port> oPort = i.value();
        QString sProtocol = oPort->sProtocol();
        QString sTransportLayer = oPort->sTransportLayer();


        ui->lst_IPv4_All->addItem(QString("%1/%2 - %3").arg(sPortKey, sTransportLayer, sProtocol));
    }
}

void WindowsToolkitMain::updateIPv6Explicit(const QHash<QString, std::shared_ptr<portscanner_port>> &data) {
    // Update the UI with IPv6 Explicit data
    for (auto i = data.begin(); i  != data.end(); ++i) {
        QString sPortKey = i.key();
        std::shared_ptr<portscanner_port> oPort = i.value();
        QString sProtocol = oPort->sProtocol();
        QString sTransportLayer = oPort->sTransportLayer();

        QString sExplicitIP = oPort->sExplicitIP();


        ui->lst_IPv6_Explicit->addItem(QString("%4 : %1/%2 - %3").arg(sPortKey, sTransportLayer, sProtocol, sExplicitIP));
    }
}

void WindowsToolkitMain::updateIPv4Explicit(const QHash<QString, std::shared_ptr<portscanner_port>> &data) {
    // Update the UI with IPv4 Explicit data
    for (auto i = data.begin(); i  != data.end(); ++i) {
        QString sPortKey = i.key();
        std::shared_ptr<portscanner_port> oPort = i.value();
        QString sProtocol = oPort->sProtocol();
        QString sTransportLayer = oPort->sTransportLayer();
        QString sExplicitIP = oPort->sExplicitIP();


        ui->lst_IPv4_Explicit->addItem(QString("%4 : %1/%2 - %3").arg(sPortKey, sTransportLayer, sProtocol, sExplicitIP));
    }
}



