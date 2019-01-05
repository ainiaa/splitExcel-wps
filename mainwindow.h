#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include<QHashIterator>
#include <QDateTime>
#include<QErrorMessage>
#include<QThread>

#include "cwpstable.h"
#include "configsetting.h"
#include "config.h"
#include "emailsender.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString openFile();

    void doSplitXls(int groupby, QString dataSheetName, QString emailSheetName, QString savePath);

    QHash<QString, QList<QStringList>> readXls(int groupby, QString selectedSheetName);
    void writeXls(QHash<QString, QList<QStringList>> qhash, QString savePath);
    void sendemail(QHash<QString, QList<QStringList>> qhash, QString savePath, int total);
    void writeXlsHeader(CWPSTable *myExcel);

    void convertToColName(int data, QString &res);
    QString to26AlphabetString(int data);

    void loadConfig();

signals:
    void doSend();

private slots:
    void on_selectFilePushButton_clicked();

    void on_savePathPushButton_clicked();

    void on_submitPushButton_clicked();

    void changeGroupby(int index);

    void on_dataComboBox_currentTextChanged(const QString &arg1);

    void showConfigSetting();

    void receiveMessage(const int msgType, const QString &result);

private:
    Ui::MainWindow *ui;
    CWPSTable *myExcel;
    QStringList *header = new QStringList();
    ConfigSetting *configSetting = new ConfigSetting(nullptr,this);
    Config *cfg = new Config();
    QThread* mailsenderThread = nullptr;
    EmailSender *mailsender;

    void errorMessage(const QString &message);
};

#endif // MAINWINDOW_H
