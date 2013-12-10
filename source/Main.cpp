#include <QApplication>
#include "MainWin.h"

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    MainWin w;
    w.show();

    return app.exec();
}
