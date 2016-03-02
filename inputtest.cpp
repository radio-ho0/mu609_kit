#include "inputtest.h"
#include "ui_inputtest.h"
#include <QDebug>
#include <QDateTime>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QIcon>
#include <QLineEdit>
#include "serialdialog.h"

double strict_str2double(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

// From QT5 Terminal example

static const char blankString[] = QT_TRANSLATE_NOOP("input Test", "N/A");

InputTest::InputTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputTest)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/qt.png"));;
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    ui->txtInput->setPalette(p);
    //setPalette(p);
    serial = new QSerialPort();
    intValidator = new QIntValidator(0, 4000000, this);
    ui->txtInput->setFocus();
    connect(ui->btStart, SIGNAL(clicked(bool)), this, SLOT(slOk()));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));
    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomDevicePathPolicy(int)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(ui->btExit, SIGNAL(clicked(bool)), this, SLOT(slExit()));
    setWindowTitle(tr("GSM test - ho0"));

    fillPortsParameters();
    fillPortsInfo();
    oneTime = 0;

}

InputTest::~InputTest()
{
    if(serial->isOpen()){
        serial->close();
    }
    delete ui;

}

void InputTest::customEvent(QEvent *e)
{

    QDialog::customEvent(e);
}

void InputTest::on_btClear_clicked()
{
    ui->txtInput->clear();

    cleanStatus();
}

void InputTest::slOk()
{
    clearother();
    // 判断输入的 gps 语句
    QString gpsSent = ui->textEdit->toPlainText();
    QByteArray arr = gpsSent.toUtf8().append('\r');
    serial->write(arr);
}

void InputTest::cleanStatus()
{


}

void InputTest::clearother()
{

}

void InputTest::readData()
{
//    ui->txtInput->clear();
    QByteArray data = serial->readAll().trimmed();
//    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
//    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式


    ui->txtInput->insertPlainText(data);

    qDebug("%s", data.data());



    // 一直跟随最新尾部
    QScrollBar *bar = ui->txtInput->verticalScrollBar();
    bar->setValue(bar->maximum());

    if(data.startsWith("+CNUM: ")){

        QByteArrayList arrList = data.split(',');
        if(arrList.size() > 2){
            QString strNum = QString(arrList.at(1)).replace("\"", "");
            ui->leNum->setText(strNum);
        }

    }
}

void InputTest::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"),   QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"),  QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"),  QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));
}

void InputTest::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

#ifndef _INPUTTEST_TEST_

        ui->serialPortInfoListBox->addItem(info.portName());
#else
        description = info.description();
        if(description.contains("interface", Qt::CaseInsensitive)){
            ui->serialPortInfoListBox->addItem(info.portName());
        }
#endif

    }



    ui->serialPortInfoListBox->setCurrentIndex(0);
    ui->serialPortInfoListBox->addItem(tr("Custom"));

}

void InputTest::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void InputTest::checkCustomDevicePathPolicy(int idx)
{
//    bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
//    ui->serialPortInfoListBox->setEditable(isCustomPath);
//    if (isCustomPath)
//        ui->serialPortInfoListBox->clearEditText();
    Q_UNUSED(idx);
}

void InputTest::slExit()
{
    if(serial->isOpen()){
        serial->close();
    }
    this->close();
}


void InputTest::on_btOpenSerial_clicked()
{
    QString serialName = ui->serialPortInfoListBox->currentText();
    serial->setPortName(serialName);
//    QSerialPort::BaudRate Rate = static_cast<QSerialPort::BaudRate>(
//                                    ui->baudRateBox->itemData(ui->baudRateBox->
//                                            currentIndex()).toInt());
    QSerialPort::BaudRate baudRate;
    if (ui->baudRateBox->currentIndex() == 4) {
        baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->currentText().toInt());
    } else {
        baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    serial->setBaudRate(baudRate);
    qDebug() << "the selected serial name:: " << serialName << " \n "
             << "The selected baudrate:: " << baudRate << endl;
    if( serial->open(QIODevice::ReadWrite )){
        qDebug("open serial successfun\n");
         ui->serialPortInfoListBox->setEnabled(false);
         ui->baudRateBox->setEnabled(false);
         ui->btOpenSerial->setEnabled(false);

    }else{
         QMessageBox::critical(this, tr("Error"), serial->errorString());
    }

}

void InputTest::on_btStopSerial_clicked()
{
    if(serial->isOpen()){
        serial->close();
        ui->btStart->setEnabled(true);
        ui->serialPortInfoListBox->setEnabled(true);
        ui->baudRateBox->setEnabled(true);
        ui->btOpenSerial->setEnabled(true);
    }

}

void InputTest::on_btViewer_clicked()
{
    serialDialog* dlg = new serialDialog(this);
    dlg->show();
}

void InputTest::on_btReadcfg_clicked()
{
    slExecCmd("AT^SYSCFG?");
}

void InputTest::slExecCmd(QString cmd)
{
    serial->write(cmd.toUtf8().append("\r\n"));
}

void InputTest::on_btAt_clicked()
{
    slExecCmd("AT");
}

void InputTest::on_bt2g_clicked()
{
    slExecCmd("AT^SYSCFG=13,1,3FFFFFFF,1,2");
}

void InputTest::on_bt3g_clicked()
{
    slExecCmd("AT^SYSCFG=2,1,3FFFFFFF,1,2");
}

void InputTest::on_btUninet_clicked()
{
    slExecCmd("AT+CGDCONT=1,\"IP\",\"3gnet\"");
}

void InputTest::on_btUniwap_clicked()
{
    slExecCmd("AT+CGDCONT=1,\"IP\",\"3gwap\"");
}

void InputTest::on_btInit_clicked()
{
    slExecCmd("ATI");
}

void InputTest::on_btInfo_clicked()
{
    slExecCmd("AT+COPS?");
}

void InputTest::on_btCsq_clicked()
{
    slExecCmd("AT+CSQ");
}

void InputTest::on_btNum_clicked()
{
    slExecCmd("AT+CNUM");
}

void InputTest::on_btBase_clicked()
{
    slExecCmd("AT+CREG=2;+CREG?");
}

void InputTest::on_btcimi_clicked()
{
    slExecCmd("AT+CIMI");
}

void InputTest::on_btSysinfo_clicked()
{
    slExecCmd("AT^SYSINFO");
}
