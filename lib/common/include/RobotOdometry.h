#pragma once

class RobotOdometry
{
private:
    float leftW, rightW;

public:
    RobotOdometry()
    {
        leftW = 0.0;
        rightW = 0.0;
    }

    float getLeftWInRad() const { return leftW; }

    float getRightWInRad() const { return rightW; }

    void updateFrom(
        float fl,
        float fr,
        float bl,
        float br)
    {
        leftW = (fl + bl) / 2.0f;
        rightW = (fr + br) / 2.0f;
    }
};
