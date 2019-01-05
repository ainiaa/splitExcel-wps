#include "emailsender.h"

EmailSender::EmailSender(QObject *parent):QObject (parent){}
EmailSender::~EmailSender(){}

void EmailSender::setSendData(Config *cfg, QHash<QString, QList<QStringList>> emailQhash, QString savePath, int total)
{
    this->cfg = cfg;
    this->emailQhash = emailQhash;
    this->savePath = savePath;
    this->total = total;
}
//发送email
void EmailSender::doSend()
{
    qDebug("sendemail");
    QHashIterator<QString,QList<QStringList>> it(emailQhash);

    QString user(cfg->Get("email","userName").toString());
    QString password(cfg->Get("email","password").toString());
    QString server(cfg->Get("email","server").toString());
    QString defaultSender(cfg->Get("email","defaultSender").toString());
    if (user.isEmpty() || password.isEmpty() || server.isEmpty() || defaultSender.isEmpty())
    {
        emit message(EmailSender::MsgTypeError, "请先正确设置配置信息");
        return;
    }
    emit message(EmailSender::MsgTypeInfo, "准备发送邮件...");

    SmtpClient smtp(server);
    smtp.setUser(user);
    smtp.setPassword(password);

    if(!smtp.connectToHost())
    {
        emit message(EmailSender::MsgTypeError, "邮件服务器连接失败！！");
        return;
    }

    if (!smtp.login(user, password))
    {
        emit message(EmailSender::MsgTypeError, "邮件服务器认证失败（邮件用户名或者密码错误）");
        return;
    }
    emit message(EmailSender::MsgTypeInfo, "开始发送邮件...");
    QString msg("发送邮件。。 已处理：%3封 成功：%1封  失败：%2封 /共：");
    msg.append(QString::number(total)).append("封");
    int failureCount =0;
    int successCount = 0;
    int totalCount = 0;
    QString noticeStart("开始发送邮件 %1");
    QString noticeEnd("完成发送邮件 %1 %2");
    while (it.hasNext()) {
        it.next();
        MimeMessage mineMsg;

        //防止中文乱码
        mineMsg.setHeaderEncoding(MimePart::Encoding::Base64);
        mineMsg.setSender(new EmailAddress(defaultSender, defaultSender));

        QString key = it.key();
        QList<QStringList> content = it.value();
        qDebug(noticeStart.arg(totalCount).arg(key).toUtf8());

        QString attementsName;
        attementsName.append(savePath).append("\\").append(key).append(".xlsx");

        // Add an another attachment
        QFileInfo file(attementsName);
        if (file.exists())
        {
            MimeAttachment *attachment = new MimeAttachment(new QFile(attementsName));
            attachment->setEncoding(MimePart::Encoding::Base64);
            attachment->setContentName("attachment.xlsx");
            mineMsg.addPart(attachment);
        }
        else
        {
            qDebug("file not exists:") ;
            qDebug(attementsName.toUtf8());
        }

        //只取一行数据
        QStringList emailData = content.at(0);

        if (emailData.size() < 4)
        {
            emit message(EmailSender::MsgTypeError, "Email sheet 数据错误。数据列必须为4行 分别为 站，email地址，email标题，email内容");
            return;
        }
        //email的数据顺序为  （站，email,title,content）
        EmailAddress * email = new EmailAddress(emailData.at(1), emailData.at(1));
        mineMsg.addRecipient(email);
        mineMsg.setSubject(emailData.at(2));

        MimeText text;
        text.setText(emailData.at(3));
        mineMsg.addPart(&text);

        if (!smtp.sendMail(mineMsg))
        {
            failureCount++;
        }
        else
        {
            successCount++;
        }
        totalCount++;
        qDebug(noticeEnd.arg(totalCount).arg(key).toUtf8());
        emit message(EmailSender::MsgTypeInfo, msg.arg(successCount).arg(failureCount).arg(totalCount));
    }
    smtp.quit();
    emit message(EmailSender::MsgTypeInfo, "处理完毕！");
    qDebug("处理完毕！:") ;
}

//停止发送email
void EmailSender::stop()
{

}
