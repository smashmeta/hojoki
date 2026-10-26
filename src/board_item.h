#pragma once

#include <QGraphicsObject>
#include <QVector>

class TaskItem;

class BoardItem final : public QGraphicsObject {
public:
    explicit BoardItem(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QVector<int> taskIds() const;
    TaskItem* taskById(int id) const;
    QPointF slotTopLeft(int priorityIndex) const;
    bool moveTask(int id, int destinationIndex);

    void beginDrag(TaskItem* task);
    void updateDrag(TaskItem* task, const QPointF& scenePos, const QPointF& grabOffset);
    void releaseDrag(TaskItem* task, const QPointF& scenePos);
    void endDrag(TaskItem* task);
    bool cancelDrag();

private:
    int nearestSlot(const QPointF& center) const;
    void animateTasks();

    QVector<TaskItem*> tasks_;
    QVector<TaskItem*> dragStartOrder_;
    TaskItem* dragged_ = nullptr;
};
