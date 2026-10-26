#include "main_widget.h"

#include "board_item.h"

#include <QResizeEvent>

MainWidget::MainWidget(QWidget* parent) : QGraphicsView(parent) {
    setWindowTitle(QStringLiteral("Hojoki"));
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QColor("#111111"));
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto* board = new BoardItem;
    scene_.addItem(board);
    scene_.setSceneRect(board->boundingRect());
    setScene(&scene_);
    resize(1200, 760);
}

void MainWidget::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
