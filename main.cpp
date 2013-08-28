#include <QtWidgets>
#include "./mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    //初始化Qt资源文件
    Q_INIT_RESOURCE(antisubmarine);
    //设置随机种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow gameClient;
    gameClient.show();

    return app.exec();
}
