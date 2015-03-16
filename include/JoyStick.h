#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <GLFW/glfw3.h>

class JoyStick
{
    public:
        JoyStick();
        static void Setup();
        virtual ~JoyStick();
        static int buttonA, buttonB;
        static int xAxis, yAxis;
        static bool isJoyStick;
        static int GetButtons(int butt);
        static int GetButtonsOnce(int butt);
        static int GetAxes(int axis) {return axes[axis];};
        static void Update();
    protected:
    private:
        static int count, count2;
        static const unsigned char* inButtons;
        static const float* inAxes;
        static int *buttons;
        static int *downButtons;
        static int *axes;
};

#endif // JOYSTICK_H
