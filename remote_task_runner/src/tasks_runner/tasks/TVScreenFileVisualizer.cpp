#include <iostream>
#include "TVScreenFileVisualizer.h"

void TVScreenFileVisualizer::ShowFile() {
    std::cout << "File was shown on TV" << std::endl;
}

void TVScreenFileVisualizer::CloseFile() {
    std::cout << "File was removed from the TV" << std::endl;
}
