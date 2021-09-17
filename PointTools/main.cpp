#include "PointTools.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qtextstream.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PointTools w;
    w.setFixedSize(880,680);

    //Include Theme
    QFile f(":/themes/style.qss");

    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    }

    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    w.show();
    return a.exec();
}
