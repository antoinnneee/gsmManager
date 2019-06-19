#include "gsmmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <gsmmanager.h>
#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras>
    #include <QAndroidJniObject>
#endif
/*Déclaration de la fct constructeur GsmMa..(en bleu), elle intègre 4 paramètres.
 * Assignation des valeurs par défaut dans les variables membres. ex : m_activity = varirable membre
 *
 */
GsmManager::GsmManager(bool graphicWarning, QString number, QString defaultText, QObject *parent)
    :QObject(parent), m_graphicWarning(graphicWarning), m_targetNumber(number), m_defaultText(defaultText)
{
#ifdef Q_OS_ANDROID
    // variable qui serivra à recuperer les objets Java du sdk (software developemment kit) androids
    m_activity =  QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",
                                                            "activity",
                                                            "()Landroid/app/Activity;");
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.SEND_SMS");
    //condition if si la permission est denied
    if (r == QtAndroid::PermissionResult::Denied){

        // Envoie une requete à l'utilisateur afin d'envoyer des SMS
        QtAndroid::requestPermissionsSync(QStringList()<<"android.permission.SEND_SMS");
        //Vérification des droits
        r = QtAndroid::checkPermission("android.permission.SEND_SMS");
        //Si pas d'autorisation retourne false
        if (r == QtAndroid::PermissionResult::Denied){
            QMessageBox d;
            d.setText("Veulliez acceptez les droit pour utilisé l'application");
            //Bloque le fil d'execution
            d.exec();
        }
    }

    if (m_activity.isValid()){

        // get the default SmsManager
         m_smsManager = QAndroidJniObject::callStaticObjectMethod("android/telephony/SmsManager",
                                                                                    "getDefault",
                                                                                    "()Landroid/telephony/SmsManager;");

         if (m_smsManager.isValid() == false){
             if (m_graphicWarning == true){
                 QMessageBox d;
                 d.setText("Problème de récuperation de SmsManager");
                 //Bloque le fil d'execution
                 d.exec();
             }
             else {
                qWarning()<< Q_FUNC_INFO << "Problème de récuperation de SmsManager";
             }
         }
    }
    else {
        if (m_graphicWarning == true){
            QMessageBox d;
            d.setText("Problème de récuperation de l'activité Android");
            //Bloque le fil d'execution
            d.exec();
        }
        else {
            qWarning()<< Q_FUNC_INFO << "Problème de récuperation de l'activité Android";
        }
    }
#endif
}

// Déclaration de la variable setTarg.. comprenant 2 paramètres. Celle-ci fait partie de la classe GsmMa..
void GsmManager::setTargetNumber(QString number){
  m_targetNumber = number;
}

QString GsmManager::getTargetNumber(){
    return m_targetNumber;
}

void GsmManager::setDefaultText (QString text){
    m_defaultText = text;
}

QString GsmManager::getDefaultText(){
    return m_defaultText;
}

void GsmManager::sendSms(QString number, QString text){
    if (number.isEmpty() == true){
        number = m_targetNumber;
    }
    if (text.isEmpty() == true){
        text = m_defaultText;
    }

#ifdef Q_OS_ANDROID
    // get phone number & text from UI and convert to Java String
    QAndroidJniObject myPhoneNumber = QAndroidJniObject::fromString(number);
    QAndroidJniObject myTextMessage = QAndroidJniObject::fromString(text);
    QAndroidJniObject scAddress = NULL;
    QAndroidJniObject sentIntent = NULL;
    QAndroidJniObject deliveryIntent = NULL;

   m_smsManager.callMethod<void>("sendTextMessage",
                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Landroid/app/PendingIntent;Landroid/app/PendingIntent;)V",
                                   myPhoneNumber.object<jstring>(),
                                   scAddress.object<jstring>(),
                                   myTextMessage.object<jstring>(), NULL, NULL );
#endif
}





