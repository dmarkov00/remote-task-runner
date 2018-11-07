#ifndef MOCKFILEVISUALIZER_H
#define MOCKFILEVISUALIZER_H

#include "FileVisualizer.h"
#include <gmock/gmock.h>

class MockFileVisualizer : public FileVisualizer {
public:
    MOCK_METHOD0(ShowFile, void());

    MOCK_METHOD0(CloseFile, void());
};

#endif 
