#include <QApplication>
#include <QtSql>
#include "WxMain.h"
//---------------------------------
static bool createConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("server_sd");
    db.setUserName("oleg"); 
    db.setHostName("192.168.2.2"); 
    db.setPassword(""); 
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    return true;
}
//---------------------------------
int main( int argc, char *argv[] ){
    QApplication app( argc, argv );
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    if (!createConnection()){return -1;}
    QSqlQuery query;
//  ----------------
    WxMain gwx(query);
    gwx.setWindowTitle(QApplication::translate("MonGraf", "Monitor", 0, QApplication::UnicodeUTF8));
    gwx.show();
//  ----------------
return app.exec();
}
//----------------------------------------------------------------
