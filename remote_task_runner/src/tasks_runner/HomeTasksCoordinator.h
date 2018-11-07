#ifndef HOMETASKSCOORDINATOR_H
#define HOMETASKSCOORDINATOR_H

#include "Light.h"
#include "FileVisualizer.h"
#include "TasksCoordinator.h"

class HomeTasksCoordinator : public TasksCoordinator {

public:
    HomeTasksCoordinator(Light &light, FileVisualizer &fileVisualizer);

    void TurnOnHomeLight();

    void TurnOffHomeLight();

    void ShowFileOnScreen();

    void RemoveFileFromScreen();

private:
    Light &light;

    FileVisualizer &fileVisualizer;
};


#endif
