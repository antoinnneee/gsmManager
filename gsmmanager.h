#ifndef GSMMANAGER_H
#define GSMMANAGER_H
#include <QObject>

#ifdef Q_OS_ANDROID
    #include <QAndroidJniObject>
#endif

class GsmManager : public QObject
{

    Q_OBJECT
public:
    explicit GsmManager(bool graphicWarning = false, QString number = "", QString defaultText = "" ,QObject *parent = nullptr);

signals:

public slots:
    void sendSms (QString number = "", QString text = "");
    void setTargetNumber (QString number);
    QString getTargetNumber();
    void setDefaultText (QString text);
    QString getDefaultText();

private:
 bool m_graphicWarning;
#ifdef Q_OS_ANDROID
 QAndroidJniObject m_activity;
 QAndroidJniObject m_smsManager;
#endif
 QString m_targetNumber;
 QString m_defaultText;

};

#endif // GSMMANAGER_H
