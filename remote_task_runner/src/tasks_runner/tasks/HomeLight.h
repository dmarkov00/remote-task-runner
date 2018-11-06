#ifndef HOMELIGHT_H
#define HOMELIGHT_H

#include "tasks_runner/tasks/interfaces/Light.h"

class HomeLight : public Light {
public:
    void TurnOn() override;

    void TurnOff() override;
};


#endif
