#include <utils.hpp>
#include <QString>
#include <stdlib.h>
#include <QDebug>
Utils::Utils() {

}

Utils::~Utils() {

}

QString Utils::readFile(const char* fileName) {
    FILE* fp = fopen(fileName,"r");
    assert(fp != NULL);
    char buf[4092];
    QString s;
    while (!feof(fp)) {
        memset(buf,sizeof(buf),0);
        fgets(buf,sizeof(buf),fp);
        s += QString(buf);
    }
    return s;
}