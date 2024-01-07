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
    void updateIPv6Loopback(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void updateIPv4Loopback(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void updateIPv6All(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void updateIPv4All(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void updateIPv6Explicit(const QMap<int, std::shared_ptr<portscanner_port>> &data);
    void updateIPv4Explicit(const QMap<int, std::shared_ptr<portscanner_port>> &data);


    void on_lst_IPv6_All_customContextMenuRequested(const QPoint &pos);
};


#endif // WINDOWSTOOLKITMAIN_H


