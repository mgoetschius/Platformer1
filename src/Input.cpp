#include "Input.h"
#include <iostream>

Input::Input()
{

}

bool Input::getKey(int key)
{
    return keys[key];
}

bool Input::getKeyOnce(int key)
{
    bool val =  keys[key] && !keyonce[key];
    if(keys[key])
        keyonce[key] = true;
    return val;
}

void Input::setKeyTrue(int key)
{
    keys[key] = true;
}

void Input::setKeyFalse(int key)
{
    keys[key] = false;
    keyonce[key] = false;
}

Input::~Input()
{
    //dtor
}
