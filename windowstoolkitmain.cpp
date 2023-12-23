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

    PortScanner = new PortScanner_mod(this);

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



