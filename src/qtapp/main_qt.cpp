#include "MainWindow.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    
    std::cout << "RawrXD Agentic IDE starting...\n";
    
    RawrXD::MainWindow window;
    window.show();
    
    return app.exec();
}
