#include <QApplication>
#include <mainwidget.hpp>
int main(int argc,char** argv) {
    QApplication app(argc,argv);
    MainWidget* mw = new MainWidget();
    mw->show();
    return app.exec();
}