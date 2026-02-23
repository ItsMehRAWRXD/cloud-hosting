#include <QApplication>
#include "qtapp/MainWindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RawrXD-AgenticIDE");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("RawrXD");

    MainWindow window;
    window.show();

    return app.exec();
}
