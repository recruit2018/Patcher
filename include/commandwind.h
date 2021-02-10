#pragma once

#include <QDialog>
#include <QStringList>

namespace Ui {
class CommandWind;
}

class CommandWind : public QDialog
{
    Q_OBJECT

    void addRow(const QString &);

public:
    explicit CommandWind(QWidget *parent = nullptr);
    ~CommandWind();

public slots:

void data_recived(const QStringList&);

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

signals:
    void send_command(const QStringList&);

private:
    Ui::CommandWind *ui;
};


