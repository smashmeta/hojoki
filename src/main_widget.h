#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>

class MainWidget final : public QGraphicsView {
public:
    explicit MainWidget(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsScene scene_;
};
