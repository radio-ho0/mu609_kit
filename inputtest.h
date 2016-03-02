#ifndef INPUTTEST_H
#define INPUTTEST_H

#include <QDialog>
#include <QIntValidator>
#include <QSerialPort>


namespace Ui {
class InputTest;
}

class InputTest : public QDialog
{
    Q_OBJECT

public:
    explicit InputTest(QWidget *parent = 0);
    ~InputTest();

    void customEvent(QEvent *e);

private slots:
    void on_btClear_clicked();
    void slOk();
    void cleanStatus();
    void clearother();
    void readData();
    void on_btOpenSerial_clicked();

    void on_btStopSerial_clicked();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);
    void slExit();


    void on_btViewer_clicked();

    void on_btReadcfg_clicked();
    void slExecCmd(QString cmd);

    void on_btAt_clicked();

    void on_bt2g_clicked();

    void on_bt3g_clicked();

    void on_btUninet_clicked();

    void on_btUniwap_clicked();

    void on_btInit_clicked();

    void on_btInfo_clicked();

    void on_btCsq_clicked();

    void on_btNum_clicked();

    void on_btBase_clicked();

    void on_btcimi_clicked();

    void on_btSysinfo_clicked();

private:
    Ui::InputTest *ui;
    QString SerialName;
    QString baudRate;
    QSerialPort* serial;

    QIntValidator *intValidator;
    void fillPortsParameters();
    void fillPortsInfo();
    int oneTime;

};

#endif // INPUTTEST_H
