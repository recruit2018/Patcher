#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include "device.h"
#include "patcherfunc.h"
#include "commandwind.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    QTranslator m_tranclator;

protected:
    void changeEvent(QEvent* event) override;
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void on_actionRussian_triggered();

    void on_actionEnglish_triggered();

    void on_But_add_device_clicked();

    void on_But_start_pach_clicked();

    void save_setting_device();

    void create_setting_device();

    void on_actionLoad_settings_triggered();

    void on_actionSave_sattings_triggered();

    void on_but_del_device_clicked();

signals:


private:
    Ui::MainWindow *ui;
};


