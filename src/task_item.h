#pragma once

#include <QGraphicsObject>
#include <QPointer>

class QPropertyAnimation;
class QGraphicsSceneContextMenuEvent;
class QGraphicsSceneMouseEvent;

enum class TaskState { Busy, Pending, Done };

class TaskItem final : public QGraphicsObject {
public:
    explicit TaskItem(int id, TaskState state, QGraphicsItem* parent = nullptr);

    int id() const { return id_; }
    TaskState state() const { return state_; }
    void setState(TaskState state);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void animateTo(const QPointF& target);
    void stopAnimation();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    int id_;
    TaskState state_;
    QPointF grabOffset_;
    QPointer<QPropertyAnimation> animation_;
};
