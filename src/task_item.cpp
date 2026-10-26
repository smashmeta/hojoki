#include "task_item.h"

#include "board_item.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>

namespace {
QColor fillFor(TaskState state) {
    switch (state) {
    case TaskState::Busy: return QColor("#ff8080");
    case TaskState::Pending: return QColor("#e6c458");
    case TaskState::Done: return QColor("#369b4d");
    }
    return QColor("#ff8080");
}

QString labelFor(TaskState state) {
    switch (state) {
    case TaskState::Busy: return QStringLiteral("busy");
    case TaskState::Pending: return QStringLiteral("pending");
    case TaskState::Done: return QStringLiteral("done");
    }
    return {};
}
} // namespace

TaskItem::TaskItem(int id, TaskState state, QGraphicsItem* parent)
    : QGraphicsObject(parent), id_(id), state_(state) {
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setCursor(Qt::OpenHandCursor);
    setZValue(1);
}

void TaskItem::setState(TaskState state) {
    if (state_ != state) {
        state_ = state;
        update();
    }
}

QRectF TaskItem::boundingRect() const {
    return QRectF(0, 0, 280, 180);
}

void TaskItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor("#dedede"), 4));
    painter->setBrush(fillFor(state_));
    painter->drawRoundedRect(boundingRect().adjusted(2, 2, -2, -2), 24, 24);

    painter->setPen(QColor("#e9e9e9"));
    QFont font = painter->font();
    font.setFamily(QStringLiteral("Segoe UI"));
    font.setPixelSize(30);
    painter->setFont(font);
    painter->drawText(QRectF(0, 20, 280, 54), Qt::AlignCenter, QString::number(id_));
    font.setPixelSize(23);
    painter->setFont(font);
    painter->drawText(QRectF(0, 85, 280, 45), Qt::AlignCenter, labelFor(state_));
}

void TaskItem::stopAnimation() {
    if (animation_) {
        animation_->stop();
        animation_->deleteLater();
        animation_ = nullptr;
    }
}

void TaskItem::animateTo(const QPointF& target) {
    stopAnimation();
    if (pos() == target) return;

    auto* animation = new QPropertyAnimation(this, "pos", this);
    animation_ = animation;
    animation->setDuration(240);
    animation->setStartValue(pos());
    animation->setEndValue(target);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    connect(animation, &QPropertyAnimation::finished, this, [this, animation] {
        if (animation_ == animation) animation_ = nullptr;
        animation->deleteLater();
    });
    animation->start();
}

void TaskItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        if (auto* board = dynamic_cast<BoardItem*>(parentItem())) board->endDrag(this);
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }
    grabOffset_ = event->pos();
    if (auto* board = dynamic_cast<BoardItem*>(parentItem())) board->beginDrag(this);
    setCursor(Qt::ClosedHandCursor);
    event->accept();
}

void TaskItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (auto* board = dynamic_cast<BoardItem*>(parentItem())) {
        board->updateDrag(this, event->scenePos(), grabOffset_);
    }
    event->accept();
}

void TaskItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (auto* board = dynamic_cast<BoardItem*>(parentItem())) {
            board->releaseDrag(this, event->scenePos());
        }
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        event->ignore();
    }
}

void TaskItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    if (auto* board = dynamic_cast<BoardItem*>(parentItem())) board->endDrag(this);
    setCursor(Qt::OpenHandCursor);
    QMenu menu;
    menu.setStyleSheet(QStringLiteral(
        "QMenu { background: #252525; color: #eeeeee; border: 1px solid #bbbbbb; padding: 5px; }"
        "QMenu::item { padding: 7px 20px 7px 10px; }"
        "QMenu::item:selected { background: #505050; }"));
    const TaskState states[] = {TaskState::Busy, TaskState::Pending, TaskState::Done};
    for (TaskState state : states) {
        QAction* action = menu.addAction(labelFor(state));
        QPixmap swatch(12, 12);
        swatch.fill(fillFor(state));
        action->setIcon(QIcon(swatch));
        action->setCheckable(true);
        action->setChecked(state_ == state);
        connect(action, &QAction::triggered, &menu, [this, state] { setState(state); });
    }
    menu.exec(event->screenPos());
    event->accept();
}
