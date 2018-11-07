#include "mocks/MockFileVisualizer.h"
#include "mocks/MockLight.h"
#include "../src/tasks_runner/HomeTasksCoordinator.h"

class HomeTasksCoordinatorTest : public ::testing::Test {
protected:
    HomeTasksCoordinatorTest() : homeTasksCoordinator(mockLight, mockFileVisualizer) {

    }

    HomeTasksCoordinator homeTasksCoordinator;
    MockLight mockLight;
    MockFileVisualizer mockFileVisualizer;

};

TEST_F(HomeTasksCoordinatorTest, LightsTurnedOn) {
    // Setup expectations
    EXPECT_CALL(mockLight, TurnOn()).Times(1);

    // Call method
    homeTasksCoordinator.TurnOnHomeLight();

}

TEST_F(HomeTasksCoordinatorTest, LightsTurnedOff) {
    // Setup expectations
    EXPECT_CALL(mockLight, TurnOff()).Times(1);

    // Call method
    homeTasksCoordinator.TurnOffHomeLight();

}
TEST_F(HomeTasksCoordinatorTest, FileShowedOnScreen) {
    // Setup expectations
    EXPECT_CALL(mockFileVisualizer, ShowFile()).Times(1);

    // Call method
    homeTasksCoordinator.ShowFileOnScreen();

}
TEST_F(HomeTasksCoordinatorTest, FileRemovedFromScreen) {
    // Setup expectations
    EXPECT_CALL(mockFileVisualizer, CloseFile()).Times(1);

    // Call method
    homeTasksCoordinator.RemoveFileFromScreen();

}