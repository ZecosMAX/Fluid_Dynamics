#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <ctime>

#include <SFML/System/Vector2.hpp>
#include "Particle.cpp"

namespace ZecosMAX
{
	namespace FluidDynamics
	{
		class Emitter
		{
		private:
			sf::Vector2f pos;
			
		public:
			Emitter()
			{
			}
			Emitter(sf::Vector2f pos)
			{
				this->pos = pos;
			}
			~Emitter() {
			}
#ifdef DEBUG
			std::vector<Particle> MakeParticles(int n = 1) {
#endif // DEBUG
#ifndef DEBUG
			std::vector<Particle> MakeParticles(int n = 64) {
#endif
				std::vector<Particle> p;
				for (short i = 0; i < n; i++)
				{
					auto rvec = sf::Vector2f(
						(float)(rand() % 2000 + (-1000)) / 20,
						(float)(rand() % 1000 + (-5000)) / 100
					);

					Particle particle(
						pos,
						rvec,
						sf::Vector2f(.0f, .0f));

#ifdef DEBUG
					std::cout << "\n\nParticle (emitter_var_b_a): \n";
					std::cout << "\tPos: X: " << particle.pos.x << " Y: " << particle.pos.y << "\n";
					std::cout << "\tSpd: X: " << particle.speed.x << " Y: " << particle.speed.y << "\n";
					std::cout << "\tAcc: X: " << particle.acceleration.x << " Y: " << particle.acceleration.y << "\n\n";
#endif // DEBUG
					p.push_back(particle);
#ifdef DEBUG
					std::cout << "\n\nParticle (emitter_var_a_a): \n";
					std::cout << "\tPos: X: " << particle.pos.x << " Y: " << particle.pos.y << "\n";
					std::cout << "\tSpd: X: " << particle.speed.x << " Y: " << particle.speed.y << "\n";
					std::cout << "\tAcc: X: " << particle.acceleration.x << " Y: " << particle.acceleration.y << "\n\n";
#endif // DEBUG
				}
#ifdef DEBUG
				std::cout << "\n\nParticle (emitter_array): \n";
				std::cout << "\tPos: X: " << p[0].pos.x << " Y: " << p[0].pos.y << "\n";
				std::cout << "\tSpd: X: " << p[0].speed.x << " Y: " << p[0].speed.y << "\n";
				std::cout << "\tAcc: X: " << p[0].acceleration.x << " Y: " << p[0].acceleration.y << "\n\n";

				std::cout << "Test of rand: "<< (float)(rand() % 200 + (-100)) / 1 <<"\n";
				std::cout << "Added n of particles\n";
				std::cout << "n is (at emitter): " << p.size() << "         \n";
#endif // DEBUG
				return p;
			}
		};
	}
}