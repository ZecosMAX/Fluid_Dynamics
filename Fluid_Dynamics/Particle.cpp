#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>

#include "Definitions.h"

using namespace std::chrono;

namespace ZecosMAX
{
	namespace FluidDynamics
	{
		class Particle
		{
		private:
			long long lastTimeMeasure = 0;
			void Timer()
			{
					std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::system_clock::now().time_since_epoch()
						);
					lastTimeMeasure = lastTimeMeasure ? lastTimeMeasure : ms.count();
					//std::cout << ms.count() - lastTimeMeasure << std::endl;
					if (ms.count() - lastTimeMeasure > lifeSpan)
					{
						isDead = true;
						//std::cout << "ded";
					}
					//std::this_thread::sleep_for(1s);
			}

		protected:
			float someOffset = 50.0f;
			float kelvinTemperature; //probably not, t of a particle ~ it's speed
			float lifeSpan = 10000.0f; 

		public:
			bool isDead = false;

			sf::Vector2f pos;
			sf::Vector2f speed;
			sf::Vector2f acceleration;

			sf::CircleShape shape; //probably not
			sf::Vertex vertex[4];
			sf::Color color = sf::Color(0, 0, 20);

			Particle() {
				//std::thread th(&Particle::Timer, this);
			}
			Particle(sf::Vector2f pos, bool random) {

			}
			Particle(sf::Vector2f pos, sf::Vector2f speed, sf::Vector2f acc) {
				
				shape = *new sf::CircleShape(20.0f);
				
				shape.setFillColor(color);
				this->pos = pos;
				this->speed = speed;
				this->acceleration = acc;

				vertex[0] = sf::Vertex(sf::Vector2f(pos.x + someOffset, pos.y + someOffset), sf::Vector2f(1, 1));
				vertex[1] = sf::Vertex(sf::Vector2f(pos.x + someOffset, pos.y - someOffset), sf::Vector2f(1, 0));
				vertex[2] = sf::Vertex(sf::Vector2f(pos.x - someOffset, pos.y - someOffset), sf::Vector2f(0, 0));
				vertex[3] = sf::Vertex(sf::Vector2f(pos.x - someOffset, pos.y + someOffset), sf::Vector2f(0, 1));

				lifeSpan += (float)(rand() % 2000 + (-1000));
#ifdef DEBUG
				std::cout << "\n\nPos: X: " << pos.x << " Y: " << pos.y << "\n";
				std::cout << "Spd: X: " << speed.x << " Y: " << speed.y << "\n";
				std::cout << "Acc: X: " << acceleration.x << " Y: " << acceleration.y << "\n\n\n";
#endif // DEBUG
			}
			~Particle() {

			}

			void ApplyForce(sf::Vector2f acc)
			{
				acceleration += acc;
			}
			void tick()
			{
				speed += acceleration * TICK_RATE_SECONDS * TIME_STAMP_M;
				pos += speed * TICK_RATE_SECONDS * TIME_STAMP_M;
				acceleration = sf::Vector2f(0,0);
				//shape.setPosition(pos);
				//vertex[0].position = pos;
				updateVerticies();
				Timer();
#ifdef DEBUG
				std::cout << "\n\nPos: X: " << pos.x << " Y: " << pos.y << "\n";
				std::cout << "Spd: X: " << speed.x << " Y: " << speed.y << "\n";
				std::cout << "Acc: X: " << acceleration.x << " Y: " << acceleration.y << "\n\n\n";
#endif // DEBUG
			}
			void updateVerticies() {
				vertex[0].position = sf::Vector2f(pos.x + someOffset, pos.y + someOffset);
				vertex[1].position = sf::Vector2f(pos.x + someOffset, pos.y - someOffset);
				vertex[2].position = sf::Vector2f(pos.x - someOffset, pos.y - someOffset);
				vertex[3].position = sf::Vector2f(pos.x - someOffset, pos.y + someOffset);
			}
		};
	}
}