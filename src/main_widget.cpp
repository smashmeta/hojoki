#include "main_widget.h"

#include "board_item.h"

#include <QKeyEvent>
#include <QResizeEvent>

MainWidget::MainWidget(QWidget* parent) : QGraphicsView(parent) {
    setWindowTitle(QStringLiteral("Hojoki"));
    setRenderHint(QPainter::Antialiasing);
    setBackgroundBrush(QColor("#111111"));
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    board_ = new BoardItem;
    scene_.addItem(board_);
    scene_.setSceneRect(board_->boundingRect());
    setScene(&scene_);
    resize(1200, 760);
}

void MainWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape && board_->cancelDrag()) {
        event->accept();
        return;
    }
    QGraphicsView::keyPressEvent(event);
}

void MainWidget::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
