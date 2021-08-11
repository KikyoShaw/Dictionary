#include "Dictionary.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dictionary w;
    w.show();
    return a.exec();
}
