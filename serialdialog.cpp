#include "serialdialog.h"
#include "ui_serialdialog.h"
#include <QTimer>
#include <QSerialPortInfo>

serialDialog::serialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialDialog)
{
    ui->setupUi(this);
    freqTimer = new QTimer(this);
    connect(freqTimer, SIGNAL(timeout()), SLOT(slupdateSerial()));

    freqTimer->setInterval( ui->spinFreq->value() * 1000 );
    freqTimer->start();

    connect( ui->spinFreq, SIGNAL(valueChanged(int)), SLOT(slSetfrep(int)));
    ui->textEdit->insertPlainText("bbq");

}

serialDialog::~serialDialog()
{
    delete ui;
}

void serialDialog::slupdateSerial()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if(info.description().contains("interface")){
            QString s = QObject::tr("Port: ") + info.portName() + "\n"
                        + QObject::tr("Location: ") + info.systemLocation() + "\n"
                        + QObject::tr("Description: ") + info.description() + "\n"
                        + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                        + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                        + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                        + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                        + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
            //ui->textEdit->insertPlainText(s);
            ui->textEdit->setPlainText(s);
        }
    }

}

void serialDialog::slSetfrep(int val)
{
    freqTimer->start(val * 1000);

}
