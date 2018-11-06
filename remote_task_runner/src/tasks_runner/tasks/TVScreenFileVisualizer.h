#ifndef TVSCREENFILEVISUALIZER_H
#define TVSCREENFILEVISUALIZER_H

#include "tasks_runner/tasks/interfaces/FileVisualizer.h"
class TVScreenFileVisualizer: public FileVisualizer  {
public:
    void ShowFile() override;

    void CloseFile() override;
};


#endif