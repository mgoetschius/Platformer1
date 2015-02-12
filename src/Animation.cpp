#include "Animation.h"

Animation::Animation()
{

}

void Animation::Setup(int first, int last, int cur, double t, double d)
{
    firstFrame = first;
    lastFrame = last;
    curFrame = cur;
    time = t;
    delay = d;
}

void Animation::Update(double dt)
{

    time += .1;

    if(time > delay)
    {
        curFrame++;
        time = 0;
    }
    if(curFrame > lastFrame)
    {
        curFrame = firstFrame;
    }
}

Animation::~Animation()
{
    //dtor
}
