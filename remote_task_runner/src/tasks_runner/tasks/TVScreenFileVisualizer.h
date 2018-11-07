#ifndef TVSCREENFILEVISUALIZER_H
#define TVSCREENFILEVISUALIZER_H

#include "FileVisualizer.h"
class TVScreenFileVisualizer: public FileVisualizer  {
public:
    void ShowFile() override;

    void CloseFile() override;
};


#endif