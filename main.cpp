#include <QApplication>
#include "aes.h"
#include "hex.h"
#include "base64.h"
#include "test.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
