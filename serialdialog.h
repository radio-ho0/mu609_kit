#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>
class QTimer;
namespace Ui {
class serialDialog;
}

class serialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit serialDialog(QWidget *parent = 0);
    ~serialDialog();

private:
    Ui::serialDialog *ui;
    QTimer* freqTimer;
    QString strtarget;

private slots:
    void slupdateSerial();
    void slSetfrep(int val);
};

#endif // SERIALDIALOG_H
