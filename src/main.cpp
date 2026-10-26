#include "main_widget.h"

#include <QApplication>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    spdlog::info("Starting Hojoki");
    MainWidget window;
    window.show();

    return app.exec();
}
