#include "widget.h"
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pid;
    if((pid = fork()) == 0){
        QApplication a(argc,argv);
        execv("./Monitor",argv);
        a.exec();
        exit(0);
    }
    if((pid = fork()) == 0){
        QApplication a(argc,argv);
        execv("./4",argv);
        a.exec();
        exit(0);
    }
    QApplication a(argc,argv);
    Widget w;
    w.setWindowTitle("No.1");
    w.show();
    return a.exec();
}
