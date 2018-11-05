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
				//sf::Vector2f A(10, 10);
				//std::cout << "A len: " << sqrt(A.x * A.x + A.y * A.y) << "\n";
				//auto B = VectorField::shortenLength(A, 14.1421f*2);
				//std::cout << "B len: " << sqrt(B.x * B.x + B.y * B.y) << "\n";
				//B = VectorField::shortenLength(B, 14.1421f);
				//std::cout << "B len 2: " << sqrt(B.x * B.x + B.y * B.y) << "\n";
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
				//���� �� ���� �������� �������, �� � ���������� ���� �� ��� ����� ��������� � ������
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
						//TODO: �������������� ������ � ���������� ������
						cpv[i].acceleration += VectorField::GetAccelerationVector(cpv[i].pos.x, cpv[i].pos.y);
						cpv[i].acceleration += sf::Vector2f(0.0f, Gravity);

						cpv[i].speed = VectorField::GetSpeedVector(cpv[i].pos.x, cpv[i].pos.y, cpv[i].speed);

#ifdef Phys_Debug
						std::cout << "\n\nParticle (a_a): \n";
						std::cout << "\tPos: X: " << cpv[0].pos.x << " Y: " << cpv[0].pos.y << "\n";
						std::cout << "\tSpd: X: " << cpv[0].speed.x << " Y: " << cpv[0].speed.y << "\n";
						std::cout << "\tAcc: X: " << cpv[0].acceleration.x << " Y: " << cpv[0].acceleration.y << "\n\n";
#endif // Phys_Debug

						cpv[i].tick();	//��������� �������������� �� �������, �.�. �������� � �������� � �������
						res.push_back(cpv[i]);	//��������� ������� � ������, ����� ���� ��� �� �� ����������� ��� ���������
					}
				}
				cpv = res;
				//������� ������ � ���, ��� �������������� �� ������� �� ������� ����� ������, � �� ������ ������ ���������
				//��� ��� �������� ������ �� ������ �� ���������

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