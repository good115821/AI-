#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("𝑫𝒓𝒆𝒂𝒎𝑾𝒉𝒂𝒍𝒆");
    w.show();
    return a.exec();
}
