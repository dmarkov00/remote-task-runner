#include "HomeTasksCoordinator.h"


HomeTasksCoordinator::HomeTasksCoordinator(Light &light, FileVisualizer &fileVisualizer) : light(light),
                                                                                           fileVisualizer(
                                                                                                   fileVisualizer) {}
void HomeTasksCoordinator::TurnOnHomeLight() {
    light.TurnOn();
}

void HomeTasksCoordinator::TurnOffHomeLight() {
    light.TurnOff();
}

void HomeTasksCoordinator::ShowFileOnScreen() {
    fileVisualizer.ShowFile();
}

void HomeTasksCoordinator::RemoveFileFromScreen() {
    fileVisualizer.CloseFile();
}

