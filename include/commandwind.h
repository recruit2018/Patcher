#pragma once

#include <QDialog>
//#include <QString>

namespace Ui {
class CommandWind;
}

class CommandWind : public QDialog
{
    Q_OBJECT

public:
    explicit CommandWind(QWidget *parent = nullptr);
    ~CommandWind();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void data_recived(QString&);
signals:
    void send_command(const QString&);

private:
    Ui::CommandWind *ui;
};


