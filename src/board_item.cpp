#include "board_item.h"

#include "task_item.h"

#include <QCursor>
#include <QPainter>
#include <QPainterPath>
#include <limits>

namespace {
QPainterPath boardOutline() {
    QPainterPath path;
    path.addRoundedRect(QRectF(35, 35, 1530, 930), 28, 28);
    return path;
}
} // namespace

BoardItem::BoardItem(QGraphicsItem* parent) : QGraphicsObject(parent) {
    const int ids[] = {1, 3, 2, 4, 6, 5, 8, 9, 7};
    for (int index = 0; index < 9; ++index) {
        const int id = ids[index];
        const TaskState state = (id == 1 || id == 5 || id == 8)
            ? TaskState::Busy : TaskState::Done;
        auto* task = new TaskItem(id, state, this);
        task->setPos(slotTopLeft(index));
        tasks_.append(task);
    }
}

QRectF BoardItem::boundingRect() const {
    return QRectF(0, 0, 1600, 1000);
}

void BoardItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillRect(boundingRect(), QColor("#111111"));
    painter->setPen(QPen(QColor("#d8d8d8"), 3));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(boardOutline());

    QFont font = painter->font();
    font.setFamily(QStringLiteral("Segoe UI"));
    font.setPixelSize(28);
    painter->setFont(font);
    painter->drawText(QRectF(0, 55, 1600, 45), Qt::AlignCenter, QStringLiteral("TASKS"));
    font.setPixelSize(21);
    painter->setFont(font);
    painter->drawText(QRectF(170, 185, 120, 35), Qt::AlignCenter, QStringLiteral("start"));
    painter->drawText(QRectF(1370, 690, 100, 35), Qt::AlignCenter, QStringLiteral("end"));

    QPainterPath path;
    path.moveTo(160, 250);
    path.lineTo(1410, 250);
    path.lineTo(1410, 500);
    path.lineTo(185, 500);
    path.lineTo(185, 750);
    path.lineTo(1430, 750);
    painter->drawPath(path);

    const QPolygonF rightArrow{QPointF(1430, 750), QPointF(1416, 742), QPointF(1416, 758)};
    painter->drawPolyline(rightArrow);
    const QPolygonF turnRight{QPointF(1410, 250), QPointF(1396, 242), QPointF(1396, 258)};
    painter->drawPolyline(turnRight);
    const QPolygonF turnLeft{QPointF(185, 500), QPointF(199, 492), QPointF(199, 508)};
    painter->drawPolyline(turnLeft);
}

QVector<int> BoardItem::taskIds() const {
    QVector<int> ids;
    ids.reserve(tasks_.size());
    for (const TaskItem* task : tasks_) ids.append(task->id());
    return ids;
}

TaskItem* BoardItem::taskById(int id) const {
    for (TaskItem* task : tasks_) {
        if (task->id() == id) return task;
    }
    return nullptr;
}

QPointF BoardItem::slotTopLeft(int priorityIndex) const {
    const int row = priorityIndex / 3;
    const int column = priorityIndex % 3;
    const int visualColumn = row % 2 == 0 ? column : 2 - column;
    return QPointF(320 + 340 * visualColumn, 160 + 250 * row);
}

bool BoardItem::moveTask(int id, int destinationIndex) {
    if (destinationIndex < 0 || destinationIndex >= tasks_.size()) return false;
    TaskItem* task = taskById(id);
    if (!task) return false;
    const int sourceIndex = tasks_.indexOf(task);
    if (sourceIndex == destinationIndex) return false;
    tasks_.move(sourceIndex, destinationIndex);
    animateTasks();
    return true;
}

void BoardItem::beginDrag(TaskItem* task) {
    if (dragged_ || !tasks_.contains(task)) return;
    dragStartOrder_ = tasks_;
    dragged_ = task;
    task->stopAnimation();
    task->setZValue(10);
}

void BoardItem::updateDrag(TaskItem* task, const QPointF& scenePos, const QPointF& grabOffset) {
    if (dragged_ != task) return;
    task->setPos(mapFromScene(scenePos) - grabOffset);
    const QPointF center = task->pos() + task->boundingRect().center();
    moveTask(task->id(), nearestSlot(center));
}

void BoardItem::releaseDrag(TaskItem* task, const QPointF& scenePos) {
    if (dragged_ != task) return;
    if (boardOutline().contains(mapFromScene(scenePos))) {
        endDrag(task);
    } else {
        cancelDrag();
    }
}

void BoardItem::endDrag(TaskItem* task) {
    if (dragged_ != task) return;
    dragged_ = nullptr;
    dragStartOrder_.clear();
    task->setZValue(1);
    task->animateTo(slotTopLeft(tasks_.indexOf(task)));
}

bool BoardItem::cancelDrag() {
    if (!dragged_) return false;
    dragged_->setCursor(Qt::OpenHandCursor);
    dragged_->setZValue(1);
    dragged_ = nullptr;
    tasks_ = dragStartOrder_;
    dragStartOrder_.clear();
    animateTasks();
    return true;
}

int BoardItem::nearestSlot(const QPointF& center) const {
    int closest = 0;
    qreal closestDistance = std::numeric_limits<qreal>::max();
    for (int index = 0; index < tasks_.size(); ++index) {
        const QPointF delta = center - (slotTopLeft(index) + QPointF(140, 90));
        const qreal distance = delta.x() * delta.x() + delta.y() * delta.y();
        if (distance < closestDistance) {
            closestDistance = distance;
            closest = index;
        }
    }
    return closest;
}

void BoardItem::animateTasks() {
    for (int index = 0; index < tasks_.size(); ++index) {
        TaskItem* task = tasks_[index];
        if (task != dragged_) task->animateTo(slotTopLeft(index));
    }
}
