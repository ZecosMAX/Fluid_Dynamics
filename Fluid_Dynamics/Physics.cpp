#pragma once
#include <vector>
#include <iostream>

#include <SFML/System/Vector2.hpp>
#include "Particle.cpp"
#include "VectorField.cpp"

#include "Definitions.h"

namespace ZecosMAX
{
	namespace FluidDynamics
	{
		class Physics
		{
		public:
			Physics() {
			}
			Physics(std::vector<Particle> particles)
			{
				this->cpv = particles;
			}

			std::vector<Particle> tick()
			{
#ifdef DEBUG
				std::cout << "Size of an TMP (Tick): " << tmp.size() << std::endl;
				std::cout << "Size of an CPV (Tick): " << cpv.size() << std::endl;
#endif // DEBUG
				//Если на поле добавили частицы, то к следующему тику их уже нужно принимать в расчёт
				if (tmp.size())
				{
					for (size_t i = 0; i < tmp.size(); i++)
					{
						cpv.push_back(tmp[i]);
					}
					tmp.clear();
				}

				std::vector<Particle> res;
				for (size_t i = 0; i < cpv.size(); i++)
				{
#ifdef DEBUG
					std::cout << "\n\nParticle (physics): \n";
					std::cout << "\tPos: X: " << cpv[0].pos.x << " Y: " << cpv[0].pos.y << "\n";
					std::cout << "\tSpd: X: " << cpv[0].speed.x << " Y: " << cpv[0].speed.y << "\n";
					std::cout << "\tAcc: X: " << cpv[0].acceleration.x << " Y: " << cpv[0].acceleration.y << "\n\n";
#endif // DEBUG
					if (!cpv[i].isDead)
					{
						//TODO: Взаимодействия частиц с векторными полями
						cpv[i].acceleration += VectorField::GetAccelerationVector(cpv[i].pos.x, cpv[i].pos.y);
						cpv[i].acceleration += sf::Vector2f(0.0f, Gravity);

						cpv[i].speed = VectorField::GetSpeedVector(cpv[i].pos.x, cpv[i].pos.y, cpv[i].speed);

#ifdef Phys_Debug
						std::cout << "\n\nParticle (a_a): \n";
						std::cout << "\tPos: X: " << cpv[0].pos.x << " Y: " << cpv[0].pos.y << "\n";
						std::cout << "\tSpd: X: " << cpv[0].speed.x << " Y: " << cpv[0].speed.y << "\n";
						std::cout << "\tAcc: X: " << cpv[0].acceleration.x << " Y: " << cpv[0].acceleration.y << "\n\n";
#endif // Phys_Debug

						cpv[i].tick();	//Обновляем взаимодействия на частице, т.е. изменяем её скорость и позицию
						res.push_back(cpv[i]);	//Добавляем частицу в вектор, после того как на неё применились все изменения
					}
				}
				cpv = res;
				//Система гласит о том, что взаимодействия не выходят за пределы этого класса, и на экране только результат
				//Так что нынешний вектор мы меняем на изменённый

				return res;
			}
			void addParticles(std::vector<Particle> particles)
			{
				for (size_t i = 0; i < particles.size(); i++)
				{
					tmp.push_back(particles[i]);
				}
#ifdef DEBUG
				std::cout << "Size of an TMP (Add): " << tmp.size() << std::endl;
#endif // DEBUG
			}

		private:
			std::vector<Particle> tmp;
			std::vector<Particle> cpv;
			bool isInteracting = false;
			bool isAdding = false;
		};
	}
}