#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>

class BoardItem;
class QKeyEvent;

class MainWidget final : public QGraphicsView {
public:
    explicit MainWidget(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene scene_;
    BoardItem* board_ = nullptr;
};
