#include "../src/main_widget.h"

#include <QApplication>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(MainWidgetTest, StartsAsAnEmptyWindow) {
    int argc = 1;
    char app_name[] = "hojoki_tests";
    char* argv[] = {app_name, nullptr};
    QApplication app(argc, argv);

    MainWidget window;
    EXPECT_THAT(window.windowTitle().toStdString(), testing::Eq("Hojoki"));
    EXPECT_TRUE(window.children().isEmpty());
}
