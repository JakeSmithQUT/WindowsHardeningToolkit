#ifndef WINDOWSTOOLKITMAIN_H
#define WINDOWSTOOLKITMAIN_H
#include "portscanner_mod.h"
#include <QProcess> // used for running powershell scripts
#include <QPlainTextEdit>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class WindowsToolkitMain; }
QT_END_NAMESPACE

class WindowsToolkitMain : public QMainWindow
{
    Q_OBJECT

public:
    WindowsToolkitMain(QWidget *parent = nullptr);
    ~WindowsToolkitMain();

private:
    Ui::WindowsToolkitMain *ui;
    QPlainTextEdit *tb_OpenPortsOutput;
    PortScanner_mod *PortScanner;



// Slot Functions
private slots:
    void on_btn_OpenPortScan_clicked();

    // Update list for new hashes in dict
    void updateIPv6Loopback(const QHash<QString, std::shared_ptr<portscanner_port>> &data);
    void updateIPv4Loopback(const QHash<QString, std::shared_ptr<portscanner_port>> &data);
    void updateIPv6All(const QHash<QString, std::shared_ptr<portscanner_port>> &data);
    void updateIPv4All(const QHash<QString, std::shared_ptr<portscanner_port>> &data);
    void updateIPv6Explicit(const QHash<QString, std::shared_ptr<portscanner_port>> &data);
    void updateIPv4Explicit(const QHash<QString, std::shared_ptr<portscanner_port>> &data);


};


#endif // WINDOWSTOOLKITMAIN_H


