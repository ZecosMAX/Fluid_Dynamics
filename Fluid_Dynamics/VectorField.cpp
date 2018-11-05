#pragma once
#include <cmath>
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Definitions.h"

#define GroundEquation 50  * cos(x/50) + 0.1f

namespace ZecosMAX
{
	namespace FluidDynamics
	{
		class VectorField
		{
		public:
			VectorField() {
			}
			~VectorField() {
			}

			static sf::Vector2f GetAccelerationVector(float x, float y)
			{
				//float len = sqrt(x * x + y * y);
				sf::Vector2f res(x,y);
				//res /= (len*len*len);
				//res *= -1.0f;
				//std::cout << GroundEquation << "\n";
				res = y >= GroundEquation - 0.4f ? sf::Vector2f(0.0f, -Gravity - (y * 0.1f)) : sf::Vector2f(0,0);

				return res;
			}
			static sf::Vector2f GetSpeedVector(float x, float y, sf::Vector2f speed)
			{
				sf::Vector2f res(x, y);
				//float len = sqrt(speed.x * speed.x + speed.y * speed.y);
				res = y >= GroundEquation ? speed * 0.0f : speed; //shortenLength(speed, len) : speed;

				return res;
			}

			static sf::Vector2f shortenLength(sf::Vector2f A, float reductionLength)
			{
				sf::Vector2f B = A;
				float len = sqrt(A.x * A.x + A.y * A.y);
				B *= (1 - reductionLength / len);
				return B;
			}
		private:
			
		};

		class Pressure
		{
		public:
			Pressure() {
			}
			~Pressure() {
			}

		private:

		};

		class Velocity
		{
		public:
			Velocity() {
			}
			~Velocity() {
			}

		private:

		};
	}
}