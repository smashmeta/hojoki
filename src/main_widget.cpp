#include "main_widget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    setWindowTitle(QStringLiteral("Hojoki"));
    resize(960, 640);
}
