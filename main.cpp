#include "screenblackout.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenBlackOut sbo;
    sbo.show();

    return a.exec();
}
