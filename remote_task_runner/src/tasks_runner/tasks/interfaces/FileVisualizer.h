#ifndef FILEVISUALIZER_H
#define FILEVISUALIZER_H

class FileVisualizer {

public:
    virtual ~FileVisualizer() {} // Needed for mocking

    virtual void ShowFile() = 0;

    virtual void CloseFile() = 0;

};

#endif
