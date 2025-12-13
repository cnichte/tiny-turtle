/**
 * @file motion/motion.cpp
 * @brief Implementierung der Bewegungsfunktionen
 */

#include "motion.h"
#include "../core/config.h"
#include "../core/globals.h"
#include "../hal/stepper.h"
#include "../hal/servo.h"
#include "../hal/sensors.h"
#include "../hal/gpio_hal.h"
#include <cmath>

namespace tiny_turtle
{
    namespace motion
    {

        using namespace config;

        bool move(float distanceMm, bool bounceAtObstacle)
        {
            uint16_t targetSteps = static_cast<uint16_t>(STEPS_PER_MM * std::abs(distanceMm));
            uint16_t halfTarget = targetSteps / 2;
            delayValue = 2000;

            for (uint16_t i = 0; i < targetSteps; i++)
            {
                hal::stepMotor(1, direction);
                hal::stepMotor(2, direction);

                if (i < halfTarget)
                {
                    delayValue -= RAMP_VALUE;
                }
                else
                {
                    delayValue += RAMP_VALUE;
                }
                delayMicroseconds(constrain(delayValue, MIN_STEP_DELAY_US, MAX_STEP_DELAY_US));

                if (bounceAtObstacle)
                    bounce();
            }
            hal::stopMotors();
            return true;
        }

        void bounce()
        {
            bool penState = drawing;

            auto bumpers = hal::readBumpers();
            if (bumpers.front)
                direction = -1;
            if (bumpers.back)
                direction = 1;

            while (hal::isBumperPressed())
            {
                hal::penUp();
                int randomDir = random(2) ? -1 : 1;

                while (hal::isBumperPressed())
                {
                    turn(randomDir);
                }
                turn(randomDir * random(5, 45));
            }

            if (penState)
                hal::penDown();
        }

        void forward(float distanceMm)
        {
            direction = 1;
            move(distanceMm);
        }

        void backward(float distanceMm)
        {
            direction = -1;
            move(distanceMm);
        }

        void turn(float degrees, int turningDirection)
        {
            if (degrees < 0)
            {
                degrees = -degrees;
                turningDirection *= -1;
            }

            while (degrees >= 360)
                degrees -= 360;

            uint16_t targetSteps = static_cast<uint16_t>(degrees * STEPS_PER_360_ROTATION / 360 + 0.5f);
            uint16_t halfTarget = targetSteps / 2;
            delayValue = 2000;

            for (uint16_t i = 0; i < targetSteps; i++)
            {
                hal::stepMotor(1, -turningDirection);
                hal::stepMotor(2, turningDirection);

                if (i < halfTarget)
                {
                    delayValue -= RAMP_VALUE;
                }
                else
                {
                    delayValue += RAMP_VALUE;
                }
                delayMicroseconds(constrain(delayValue, MIN_STEP_DELAY_US, MAX_STEP_DELAY_US));
            }
        }

        void smartTurn(float angle)
        {
            while (angle > 180)
                angle -= 360;
            while (angle <= -180)
                angle += 360;

            float optimized = angle;

            if (optimized > 90)
            {
                direction *= -1;
                optimized -= 180;
            }
            else if (optimized < -90)
            {
                direction *= -1;
                optimized += 180;
            }
            else if (optimized == 180 || optimized == -180)
            {
                direction *= -1;
                optimized = 0;
            }

            turn(optimized, 1);
        }

    } // namespace motion
} // namespace tiny_turtle

// Legacy-Kompatibilität
void forward(float distance) { tiny_turtle::motion::forward(distance); }
void back(float distance) { tiny_turtle::motion::backward(distance); }
bool move(float distance, bool bounce) { return tiny_turtle::motion::move(distance, bounce); }
void turn(float degrees, int dir) { tiny_turtle::motion::turn(degrees, dir); }
void smartTurn(float angle) { tiny_turtle::motion::smartTurn(angle); }
void bounce() { tiny_turtle::motion::bounce(); }
