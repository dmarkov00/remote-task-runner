
#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:
    virtual ~Light() {} // Needed for mocking

    virtual void TurnOn() = 0;

    virtual void TurnOff() = 0;
};

#endif
