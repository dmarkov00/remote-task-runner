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