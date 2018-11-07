
#include "HomeTasksCoordinator.h"
#include "Light.h"


HomeTasksCoordinator::HomeTasksCoordinator(Light &light, FileVisualizer &fileVisualizer) : light(light),
                                                                                           fileVisualizer(
                                                                                                   fileVisualizer) {}


void HomeTasksCoordinator::TurnOnHomeLight() {
    light.TurnOn();
}

