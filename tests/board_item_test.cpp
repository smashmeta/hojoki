#include "../src/board_item.h"
#include "../src/task_item.h"

#include <QEventLoop>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <gtest/gtest.h>

TEST(BoardItemTest, StartsInSnakePriorityOrderWithStableIds) {
    BoardItem board;
    EXPECT_EQ(board.taskIds(), (QVector<int>{1, 3, 2, 4, 6, 5, 8, 9, 7}));
    EXPECT_NE(board.taskById(1), nullptr);
    EXPECT_NE(board.taskById(9), nullptr);
}

TEST(BoardItemTest, MovingForwardShiftsOnlyInterveningTasks) {
    BoardItem board;
    TaskItem* original = board.taskById(1);

    EXPECT_TRUE(board.moveTask(1, 5));
    EXPECT_EQ(board.taskIds(), (QVector<int>{3, 2, 4, 6, 5, 1, 8, 9, 7}));
    EXPECT_EQ(board.taskById(1), original);
}

TEST(BoardItemTest, MovingBackwardAcrossRowsPreservesAllIds) {
    BoardItem board;
    EXPECT_TRUE(board.moveTask(8, 1));
    EXPECT_EQ(board.taskIds(), (QVector<int>{1, 8, 3, 2, 4, 6, 5, 9, 7}));
    EXPECT_FALSE(board.moveTask(8, -1));
    EXPECT_FALSE(board.moveTask(99, 0));
}

TEST(BoardItemTest, SlotsFollowAlternatingSnakeDirection) {
    BoardItem board;
    EXPECT_LT(board.slotTopLeft(0).x(), board.slotTopLeft(1).x());
    EXPECT_LT(board.slotTopLeft(1).x(), board.slotTopLeft(2).x());
    EXPECT_GT(board.slotTopLeft(3).x(), board.slotTopLeft(4).x());
    EXPECT_GT(board.slotTopLeft(4).x(), board.slotTopLeft(5).x());
    EXPECT_LT(board.slotTopLeft(6).x(), board.slotTopLeft(7).x());
    EXPECT_LT(board.slotTopLeft(7).x(), board.slotTopLeft(8).x());
    EXPECT_LT(board.slotTopLeft(0).y(), board.slotTopLeft(3).y());
    EXPECT_LT(board.slotTopLeft(3).y(), board.slotTopLeft(6).y());
}

TEST(BoardItemTest, ReorderedTasksAnimateToTheirNewSlots) {
    BoardItem board;
    ASSERT_TRUE(board.moveTask(1, 5));

    QEventLoop loop;
    QTimer::singleShot(350, &loop, &QEventLoop::quit);
    loop.exec();

    EXPECT_EQ(board.taskById(1)->pos(), board.slotTopLeft(5));
    EXPECT_EQ(board.taskById(3)->pos(), board.slotTopLeft(0));
}

TEST(TaskItemTest, PendingStatePaintsYellow) {
    QGraphicsScene scene;
    auto* task = new TaskItem(42, TaskState::Busy);
    scene.addItem(task);
    task->setState(TaskState::Pending);

    QImage image(280, 180, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    scene.render(&painter, QRectF(0, 0, 280, 180), QRectF(0, 0, 280, 180));
    painter.end();

    EXPECT_EQ(image.pixelColor(20, 90), QColor("#e6c458"));
}

TEST(BoardItemTest, RendersTheReferencePalette) {
    QGraphicsScene scene;
    auto* board = new BoardItem;
    scene.addItem(board);
    scene.setSceneRect(board->boundingRect());
    QImage image(1600, 1000, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    scene.render(&painter, QRectF(0, 0, 1600, 1000), board->boundingRect());
    painter.end();
    EXPECT_EQ(image.pixelColor(350, 290), QColor("#ff8080"));
    EXPECT_EQ(image.pixelColor(690, 290), QColor("#369b4d"));
    EXPECT_EQ(image.pixelColor(100, 100), QColor("#111111"));
}
