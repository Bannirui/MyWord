#include <QApplication>
#include "myword.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWord w;
    w.show();
    return QApplication::exec();
}
