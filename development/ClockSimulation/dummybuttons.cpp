#include "dummybuttons.hpp"

#include "hwbuttons.hpp"

#include <cassert>


CustomSimplePin pin0;
CustomSimplePin pin1;
CustomSimplePin pin2;
CustomSimplePin pin3;


void simplePinInitialize(void const * const instance)
{
    CustomSimplePin const * const instanceTyped = static_cast<CustomSimplePin const *>(instance);
    if (instanceTyped == &pin0)
    {
        simplePin0::initialize();
    }
    else if (instanceTyped == &pin1)
    {
        simplePin1::initialize();
    }
    else if (instanceTyped == &pin2)
    {
        simplePin2::initialize();
    }
    else if (instanceTyped == &pin3)
    {
        simplePin3::initialize();
    }
    else
    {
        assert(false);
    }
}

SimplePin::State getSimplePinIsDown(void const * const instance)
{
    CustomSimplePin const * const instanceTyped = static_cast<CustomSimplePin const *>(instance);
    if (instanceTyped == &pin0)
    {
        return simplePin0::get();
    }
    else if (instanceTyped == &pin1)
    {
        return simplePin1::get();
    }
    else if (instanceTyped == &pin2)
    {
        return simplePin2::get();
    }
    else if (instanceTyped == &pin3)
    {
        return simplePin3::get();
    }
    else
    {
        assert(false);
        return SimplePin::State::Zero;
    }
}

void setSimplePinIsDown(void const * const instance, SimplePin::State const state)
{
    CustomSimplePin const * const instanceTyped = static_cast<CustomSimplePin const *>(instance);
    if (instanceTyped == &pin0)
    {
        simplePin0::set(state);
    }
    else if (instanceTyped == &pin1)
    {
        simplePin1::set(state);
    }
    else if (instanceTyped == &pin2)
    {
        simplePin2::set(state);
    }
    else if (instanceTyped == &pin3)
    {
        simplePin3::set(state);
    }
    else
    {
        assert(false);
    }
}

void simplePinDeinitialize(void const * const instance)
{
    CustomSimplePin const * const instanceTyped = static_cast<CustomSimplePin const *>(instance);
    if (instanceTyped == &pin0)
    {
        simplePin0::deinitialize();
    }
    else if (instanceTyped == &pin1)
    {
        simplePin1::deinitialize();
    }
    else if (instanceTyped == &pin2)
    {
        simplePin2::deinitialize();
    }
    else if (instanceTyped == &pin3)
    {
        simplePin3::deinitialize();
    }
    else
    {
        assert(false);
    }
}





CustomButtomTimedMultiple buttonTimedMultiple0;
CustomButtomTimedMultiple buttonTimedMultiple1;
CustomButtomTimedMultiple buttonTimedMultiple2;
CustomButtomTimedMultiple buttonTimedMultiple3;


static void buttonInitialize(void const * const instance)
{
    CustomButtomTimedMultiple const * const instanceTyped = static_cast<CustomButtomTimedMultiple const *>(instance);
    if (instanceTyped == &buttonTimedMultiple0)
    {
        button0::initialize();
    }
    else if (instanceTyped == &buttonTimedMultiple1)
    {
        button1::initialize();
    }
    else if (instanceTyped == &buttonTimedMultiple2)
    {
        button2::initialize();
    }
    else if (instanceTyped == &buttonTimedMultiple3)
    {
        button3::initialize();
    }
    else
    {
        assert(false);
    }
}

static void buttonDeinitialize(void const * const instance)
{
    CustomButtomTimedMultiple const * const instanceTyped = static_cast<CustomButtomTimedMultiple const *>(instance);
    if (instanceTyped == &buttonTimedMultiple0)
    {
        button0::deinitialize();
    }
    else if (instanceTyped == &buttonTimedMultiple1)
    {
        button1::deinitialize();
    }
    else if (instanceTyped == &buttonTimedMultiple2)
    {
        button2::deinitialize();
    }
    else if (instanceTyped == &buttonTimedMultiple3)
    {
        button3::deinitialize();
    }
    else
    {
        assert(false);
    }
}

static bool buttonIsDown(void const * const instance)
{
    CustomButtomTimedMultiple const * const instanceTyped = static_cast<CustomButtomTimedMultiple const *>(instance);
    if (instanceTyped == &buttonTimedMultiple0)
    {
        return button0::isDown();
    }
    else if (instanceTyped == &buttonTimedMultiple1)
    {
        return button1::isDown();
    }
    else if (instanceTyped == &buttonTimedMultiple2)
    {
        return button2::isDown();
    }
    else if (instanceTyped == &buttonTimedMultiple3)
    {
        return button3::isDown();
    }
    else
    {
        assert(false);
        return false;
    }
}
