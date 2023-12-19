#ifndef WINDOWSTOOLKITMAIN_H
#define WINDOWSTOOLKITMAIN_H
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
    void runPowerShellScript(const QString &script);
    QString onScriptFinished(QProcess *process, QString &outputVariable);
    void initializeProtocolHash(QHash<QString, QString> &protocolHash);
    QString findProtocolDescription(const QHash<QString, QString> &protocolHash, const QString &port);


// Slot Functions
private slots:
    void on_btn_OpenPortScan_clicked();
    void update_tb_OpenPortsOutput(const QString &text, const QStringList &sReturnedPorts);

};

// Global Variables
extern QHash<QString, QString> hHashOfProtocols;

#endif // WINDOWSTOOLKITMAIN_H
