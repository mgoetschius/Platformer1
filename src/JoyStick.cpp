#include "JoyStick.h"
#include <iostream>

int *JoyStick::buttons = NULL;
int *JoyStick::downButtons = NULL;
int *JoyStick::axes = NULL;
int JoyStick::xAxis;
int JoyStick::yAxis;
int JoyStick::buttonA;
int JoyStick::buttonB;
int JoyStick::count, JoyStick::count2;
const unsigned char* JoyStick::inButtons;
const float* JoyStick::inAxes;
bool JoyStick::isJoyStick = false;

JoyStick::JoyStick()
{

}

void JoyStick::Setup()
{
    buttonB = 3;
    buttonA = 2;
    xAxis = 0;
    yAxis = 1;
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (present)
    {
        inButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
        inAxes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count2);
        if(inButtons != NULL)
        {
            buttons = new int[count]();
            downButtons = new int[count]();
        }
        if(inAxes != NULL)
            axes = new int[count];

        isJoyStick = true;
    }
    else
        isJoyStick = false;
}

void JoyStick::Update()
{
    inButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
    for(int i = 0; i < count; i++)
    {
        buttons[i] = inButtons[i];
        if(!inButtons[i] && downButtons[i])
            downButtons[i] = inButtons[i];
    }
    inAxes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count2);
    for(int i = 0; i < count2; i++)
    {
        axes[i] = inAxes[i];
    }
}

int JoyStick::GetButtons(int butt)
{
    return buttons[butt];
}

int JoyStick::GetButtonsOnce(int butt)
{
    int temp = GetButtons(butt);
    if(temp && !downButtons[butt])
    {
        downButtons[butt] = 1;
        return 1;
    }
    else
        return 0;
}

JoyStick::~JoyStick()
{
    std::cout << "Clearing Joystick" << std::endl;
    delete buttons;
    delete downButtons;
    delete axes;
}
