#ifndef MOCKLIGHT_H
#define MOCKLIGHT_H

#include "Light.h"
#include <gmock/gmock.h>

class MockLight : public Light {
public:
    MOCK_METHOD0(TurnOn, void());

    MOCK_METHOD0(TurnOff, void());
};

#endif
