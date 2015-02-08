/**
 * This file is part of SDLGamepad.
 *
 * SDLGamepad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SDLGamepad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Manuel Blanquett
 * mail.nalla@gmail.com
 */

/**********************************************************************/
#include "sdlgamepad.h"

#include <SDL/SDL.h>
// #undef main

class SDLGamepadPrivate {
public:
    SDLGamepadPrivate() : gamepad(0)
    {}

    /**
     * SDL_Joystick object.
     *
     * This represents the currently opened SDL_Joystick object.
     */
    SDL_Joystick *gamepad;
};

/**********************************************************************/
SDLGamepad::SDLGamepad()
{
    buttons = -1;
    axes    = -1;
    index   = -1;
    loop    = false;
    tick    = MIN_RATE;
    priv    = new SDLGamepadPrivate;
}

/**********************************************************************/
SDLGamepad::~SDLGamepad()
{
    loop = false;

    if (priv->gamepad) {
        SDL_JoystickClose(priv->gamepad);
    }

    SDL_Quit();

    delete priv;
}

/**********************************************************************/
bool SDLGamepad::init()
{
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        return false;
    }

    if (SDL_NumJoysticks() > 0) {
        emit gamepads(SDL_NumJoysticks());

        if (!setGamepad(0)) {
            return false;
        }

        for (qint8 i = 0; i < buttons; i++) {
            buttonStates.append(0);
        }
    } else {
        return false;
    }

    loop = true;
    return true;
}

/**********************************************************************/
void SDLGamepad::run()
{
    while (loop) {
        updateAxes();
        updateButtons();
        msleep(tick);
    }
}

/**********************************************************************/
bool SDLGamepad::setGamepad(qint16 index)
{
    if (index != this->index) {
        if (SDL_JoystickOpened(this->index)) {
            SDL_JoystickClose(priv->gamepad);
        }

        priv->gamepad = SDL_JoystickOpen(index);

        if (priv->gamepad) {
            buttons = SDL_JoystickNumButtons(priv->gamepad);
            axes    = SDL_JoystickNumAxes(priv->gamepad);

            if (axes >= 4) {
                this->index = index;
                return true;
            } else {
                buttons     = -1;
                axes        = -1;
                this->index = -1;
                qCritical("Gamepad has less than 4 axes");
            }
        } else {
            buttons     = -1;
            axes        = -1;
            this->index = -1;
            qCritical("Unable to open Gamepad!");
        }
    }

    return false;
}

/**********************************************************************/
void SDLGamepad::setTickRate(qint16 ms)
{
    tick = ms;
}

/**********************************************************************/
int SDLGamepad::mapAxes(int OPAxes)
{
    //input OP-GCS-ch  output Xbox-ch
    switch (OPAxes) {
        case 0: return 2; //roll 
        case 1: return 3; //pitch
        case 2: return 1; //throttle
        case 3: return 0; //yaw
        case 4: return 4;
        case 5: return 5;
        case 6: return 6;
        case 7: return 7;
        case 8: return 8;
        case 9: return 9; 
        default: return 4;       
    }

}
/**********************************************************************/
void SDLGamepad::updateAxes()
{
    if (priv->gamepad) {
        QListInt16 values;
        SDL_JoystickUpdate();

        for (qint8 i = 0; i < axes; i++) {
            //qint16 value = SDL_JoystickGetAxis(priv->gamepad, mapAxes(i));
            qint16 value = SDL_JoystickGetAxis(priv->gamepad, i);

            if (value > -NULL_RANGE && value < NULL_RANGE) {
                value = 0;
            }

            values.append(value);
        }

        emit axesValues(values);
    }
}


/**********************************************************************/
int SDLGamepad::mapButtons(int OPButtons)
{
    //input OP-GCS-ch  output Xbox-ch
    switch (OPButtons) {
        case 0: return 0; //up
        case 1: return 1; //down
        case 2: return 2; //left
        case 3: return 3; //right
        case 4: return 4; //start
        case 5: return 5; //select
        case 6: return 8; //top left
        case 7: return 9; //top right
        case 8: return 10; //xbox
        case 9: return 11; //A
        case 10: return 12; //B
        case 11: return 13; //X
        case 12: return 14; //Y
        default: return 15;      
    }

}
/**********************************************************************/
void SDLGamepad::updateButtons()
{
    if (priv->gamepad) {
        SDL_JoystickUpdate();

        for (qint8 i = 0; i < buttons; i++) {
            //qint16 state = SDL_JoystickGetButton(priv->gamepad, mapButtons(i));
            qint16 state = SDL_JoystickGetButton(priv->gamepad, i);

            if (buttonStates.at(i) != state) {
                if (state > 0) {
                    emit buttonState((ButtonNumber)i, true);
                } else {
                    emit buttonState((ButtonNumber)i, false);
                }

                buttonStates.replace(i, state);
            }
        }
    }
}

/**********************************************************************/
void SDLGamepad::quit()
{
    loop = false;
}

/**********************************************************************/
qint16 SDLGamepad::getAxes()
{
    return axes;
}

/**********************************************************************/
qint16 SDLGamepad::getButtons()
{
    return buttons;
}
