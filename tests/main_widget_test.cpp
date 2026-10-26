#include "../src/main_widget.h"
#include "../src/board_item.h"
#include "../src/task_item.h"

#include <QApplication>
#include <QContextMenuEvent>
#include <QTest>
#include <QTimer>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(MainWidgetTest, HostsTheTaskBoard) {
    MainWidget window;
    EXPECT_THAT(window.windowTitle().toStdString(), testing::Eq("Hojoki"));
    ASSERT_NE(window.scene(), nullptr);
    EXPECT_EQ(window.scene()->items().size(), 10);
}

TEST(MainWidgetTest, DraggingTaskShiftsTheInterveningTasks) {
    MainWidget window;
    window.show();
    QApplication::processEvents();
    BoardItem* board = nullptr;
    for (QGraphicsItem* item : window.scene()->items()) {
        if (auto* candidate = dynamic_cast<BoardItem*>(item)) board = candidate;
    }
    ASSERT_NE(board, nullptr);

    TaskItem* task = board->taskById(1);
    const QPoint from = window.mapFromScene(task->mapToScene(task->boundingRect().center()));
    const QPoint to = window.mapFromScene(board->slotTopLeft(5) + task->boundingRect().center());
    QTest::mousePress(window.viewport(), Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(window.viewport(), to, 20);
    QTest::mouseRelease(window.viewport(), Qt::LeftButton, Qt::NoModifier, to);

    EXPECT_EQ(board->taskIds(), (QVector<int>{3, 2, 4, 6, 5, 1, 8, 9, 7}));
}

TEST(MainWidgetTest, RightClickDuringDragSnapsTheTaskToItsSlot) {
    MainWidget window;
    window.show();
    QApplication::processEvents();
    BoardItem* board = nullptr;
    for (QGraphicsItem* item : window.scene()->items()) {
        if (auto* candidate = dynamic_cast<BoardItem*>(item)) board = candidate;
    }
    ASSERT_NE(board, nullptr);

    TaskItem* task = board->taskById(1);
    const QPoint from = window.mapFromScene(task->mapToScene(task->boundingRect().center()));
    const QPoint to = window.mapFromScene(board->slotTopLeft(5)
        + task->boundingRect().center() + QPointF(35, 15));
    QTest::mousePress(window.viewport(), Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(window.viewport(), to, 20);
    ASSERT_NE(task->pos(), board->slotTopLeft(5));

    QTimer::singleShot(50, [] {
        if (QWidget* popup = QApplication::activePopupWidget()) popup->close();
    });
    QContextMenuEvent rightClick(QContextMenuEvent::Mouse, to,
        window.viewport()->mapToGlobal(to));
    QApplication::sendEvent(window.viewport(), &rightClick);
    QTest::qWait(350);

    EXPECT_EQ(task->pos(), board->slotTopLeft(5));
    EXPECT_EQ(task->zValue(), 1);
}

TEST(MainWidgetTest, DroppingOutsideBorderRestoresOrderFromDragStart) {
    MainWidget window;
    window.show();
    QApplication::processEvents();
    BoardItem* board = nullptr;
    for (QGraphicsItem* item : window.scene()->items()) {
        if (auto* candidate = dynamic_cast<BoardItem*>(item)) board = candidate;
    }
    ASSERT_NE(board, nullptr);

    TaskItem* task = board->taskById(1);
    const QVector<int> original = board->taskIds();
    const QPoint from = window.mapFromScene(task->mapToScene(task->boundingRect().center()));
    const QPoint middle = window.mapFromScene(board->slotTopLeft(5)
        + task->boundingRect().center());
    const QPoint outside = window.mapFromScene(QPointF(20, 500));
    QTest::mousePress(window.viewport(), Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(window.viewport(), middle, 20);
    ASSERT_NE(board->taskIds(), original);
    QTest::mouseMove(window.viewport(), outside, 20);
    QTest::mouseRelease(window.viewport(), Qt::LeftButton, Qt::NoModifier, outside);
    QTest::qWait(350);

    EXPECT_EQ(board->taskIds(), original);
    EXPECT_EQ(task->pos(), board->slotTopLeft(0));
    EXPECT_EQ(task->zValue(), 1);
    EXPECT_EQ(task->cursor().shape(), Qt::OpenHandCursor);
}

TEST(MainWidgetTest, EscapeDuringDragRestoresOrderFromDragStart) {
    MainWidget window;
    window.show();
    QApplication::processEvents();
    BoardItem* board = nullptr;
    for (QGraphicsItem* item : window.scene()->items()) {
        if (auto* candidate = dynamic_cast<BoardItem*>(item)) board = candidate;
    }
    ASSERT_NE(board, nullptr);

    TaskItem* task = board->taskById(1);
    const QVector<int> original = board->taskIds();
    const QPoint from = window.mapFromScene(task->mapToScene(task->boundingRect().center()));
    const QPoint middle = window.mapFromScene(board->slotTopLeft(5)
        + task->boundingRect().center());
    QTest::mousePress(window.viewport(), Qt::LeftButton, Qt::NoModifier, from);
    QTest::mouseMove(window.viewport(), middle, 20);
    ASSERT_NE(board->taskIds(), original);
    QTest::keyClick(window.viewport(), Qt::Key_Escape);
    EXPECT_EQ(task->cursor().shape(), Qt::OpenHandCursor);
    QTest::qWait(350);
    QTest::mouseRelease(window.viewport(), Qt::LeftButton, Qt::NoModifier, middle);

    EXPECT_EQ(board->taskIds(), original);
    EXPECT_EQ(task->pos(), board->slotTopLeft(0));
    EXPECT_EQ(task->zValue(), 1);
    EXPECT_EQ(task->cursor().shape(), Qt::OpenHandCursor);
}
