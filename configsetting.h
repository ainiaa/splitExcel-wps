#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H

#include <QMainWindow>
#include <QMessageBox>

#include "config.h"
namespace Ui {
class ConfigSetting;
}

class ConfigSetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigSetting(QWidget *parent = nullptr,QMainWindow *mainWindow = nullptr);
    ~ConfigSetting();

    void loadConfig();
    void writeConfig();

private slots:
    void on_submitPushButton_clicked();

    void on_gobackPushButton_clicked();

private:
    Ui::ConfigSetting *ui;
    Config *cfg = new Config();
    QMainWindow *mainWindow;
};

#endif // CONFIGSETTING_H
