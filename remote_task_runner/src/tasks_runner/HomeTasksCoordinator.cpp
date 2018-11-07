#include "HomeTasksCoordinator.h"



HomeTasksCoordinator::HomeTasksCoordinator(Light &light, FileVisualizer &fileVisualizer) : light(light),
                                                                                           fileVisualizer(
                                                                                                   fileVisualizer) {}


void HomeTasksCoordinator::TurnOnHomeLight() {
    light.TurnOn();
}

