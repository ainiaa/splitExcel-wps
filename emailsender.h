#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QObject>
#include <QHash>
#include <QList>
#include <QString>
#include <QMessageBox>
#include <QFileInfo>

#include "smtpclient/src/SmtpMime"
#include "config.h"

class EmailSender: public QObject
{
    Q_OBJECT
public:
    EmailSender(QObject* parent = nullptr);
    ~EmailSender();
    static const int MsgTypeError  = 1;
    static const int MsgTypeWarn = 2;
    static const int MsgTypeInfo   = 3;

public slots:
    void doSend();
    void setSendData(Config *cfg, QHash<QString, QList<QStringList>> emailQhash, QString savePath, int total);
    void stop();

 signals:
    void message(const int msgType, const QString &result);
private:
    Config *cfg;
    QHash<QString, QList<QStringList>> emailQhash;
    QString savePath;
    int total;
    int m_total;
    int m_process;
    int m_success;
    int m_failure;
};
#endif // EMAILSENDER_H
