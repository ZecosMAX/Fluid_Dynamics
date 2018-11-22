#pragma warning( disable : 4996 )  

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <sstream> 
#include <random> /* srand, rand */
#include <chrono>
#include <thread>

#include "Particle.cpp"
#include "Emitter.cpp"
#include "Physics.cpp"

#include "Definitions.h"

using namespace std::chrono;





namespace ZecosMAX
{
	namespace FluidDynamics
	{
		class SystemHandler
		{
		private:
			sf::ContextSettings settings;
			sf::RenderWindow *window;
			sf::View *view;
			sf::Shader *shader;
			sf::Shader *shaderF;
			sf::Texture* CircleTexture;

			Physics physics;
			Emitter emitter;
			std::vector<Particle> particles;

			unsigned int particles_count = 0;

			bool close = false;
			bool adding = false;
		public:	
			Particle p[1024*64];

			SystemHandler() {
				emitter = *new Emitter(sf::Vector2f(-1.0f, 0));
				settings.antialiasingLevel = 8;
				window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Graphs", sf::Style::Default, settings);
				view = new sf::View(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT));
				shader = new sf::Shader();
				shaderF = new sf::Shader();
				CircleTexture = new sf::Texture();

				if (!CircleTexture->loadFromFile("Circle.png"))
				{
					std::cout << "There is some error at TEXTURE, idk what exactly\nSo fuck off\n";
				}

				if (!sf::Shader::isAvailable())
				{
					std::cout << "Shader are not even available\n";
					// shaders are not available...
				}
				std::ifstream ifsv("vertex_shader.vert");
				std::string vertex((std::istreambuf_iterator<char>(ifsv)),
					(std::istreambuf_iterator<char>()));

				std::ifstream ifsf("fragment_shader.frag");
				std::string fragment((std::istreambuf_iterator<char>(ifsf)),
					(std::istreambuf_iterator<char>()));

				std::ifstream ifsfr("Fractal_shader.frag");
				std::string fractal((std::istreambuf_iterator<char>(ifsfr)),
					(std::istreambuf_iterator<char>()));

				// load both shaders
				if (!shader->loadFromMemory(vertex, sf::Shader::Vertex))
				{
					std::cout << "There is some error, idk what exactly\nSo fuck off\n";// << sf::err().rdbuf();
					std::cout << vertex << "\n";
					//return;
					// error...
				}
				if (!shader->loadFromMemory(fragment, sf::Shader::Fragment))
				{
					std::cout << "There is some error, idk what exactly\nSo fuck off\n";// << sf::err().rdbuf();
					std::cout << fragment << "\n";
					//return;
					// error...
				}
				if (!shaderF->loadFromMemory(vertex, sf::Shader::Vertex))
				{
					std::cout << "There is some error, idk what exactly\nSo fuck off\n";// << sf::err().rdbuf();
					std::cout << vertex << "\n";
					//return;
					// error...
				}
				if (!shaderF->loadFromMemory(fractal, sf::Shader::Fragment))
				{
					std::cout << "There is some error, idk what exactly\nSo fuck off\n";// << sf::err().rdbuf();
					std::cout << fractal << "\n";
					//return;
					// error...
				}
				//StartRenderLoop();
			}
			~SystemHandler() {
			}

			void StartRenderLoop()
			{
#ifdef DEBUG
				std::cout << "...\tStarted a renderloop\n";
#endif // DEBUG
				view->setCenter(0, 0);


				/*std::vector<sf::Vertex> verticiesStatic;
				verticiesStatic.push_back(sf::Vertex(sf::Vector2f( WIDTH / 2,  HEIGHT / 2), sf::Vector2f(.5, .5)));
				verticiesStatic.push_back(sf::Vertex(sf::Vector2f( WIDTH / 2, -HEIGHT / 2),	sf::Vector2f(.5, -.5)));
				verticiesStatic.push_back(sf::Vertex(sf::Vector2f(-WIDTH / 2, -HEIGHT / 2), sf::Vector2f(-.5, -.5)));
				verticiesStatic.push_back(sf::Vertex(sf::Vector2f(-WIDTH / 2,  HEIGHT / 2),	sf::Vector2f(-.5, .5)));*/


				shader->setParameter("color", sf::Vector3f(0.5f, 0.5f ,0.5f ));


				int IterLimit = 0;
				float x = 0.25, y = 0;
				float scale = 1.0;
				while (window->isOpen())
				{
					shaderF->setUniform("Limit", IterLimit);
					shaderF->setUniform("offset", sf::Vector2f(x, y));
					shaderF->setUniform("position", sf::Vector2f(0, 0));
					shaderF->setUniform("scale", scale);

					sf::String title;
					title += "Iterations ";
					title += std::to_string(IterLimit);
					window->setTitle(title);

					sf::Event event;
					while (window->pollEvent(event))
					{
						//окно будет закрываться по нажатию на Esc
						if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
						{
							window->close();
							close = true;
							return;
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Pause))
						{
							//physics.addParticles(emitter.MakeParticles(1000));
							//particles_count += (1);
							adding = !adding;
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0))
						{
							IterLimit++;
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num9))
						{
							IterLimit--;
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)) {
							y-= (0.01 / scale);
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)) {
							y+= (0.01 / scale);
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)) {
							x += (0.01 / scale);
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)) {
							x -= (0.01 / scale);
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::PageUp)) {
							scale *= 2;
						}
						if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::PageDown)) {
							scale /= 2;
						}
					}

					// Очистка
					//window->clear(sf::Color(247, 239, 232));
					window->clear(sf::Color(247, 239, 232));
					window->setView(*view);

#ifdef DEBUG
					//std::cout << "\rParticles count: " << particles.size();
#endif // DEBUG

					//window->draw(&verticiesStatic[0], verticiesStatic.size(), sf::Quads, shaderF);

					try
					{
						for (size_t i = 0; i < particles.size(); i++)
						{
							//sf::CircleShape particle(2.0f);
							//particle.setPosition(p[i].pos);
							//particle.setFillColor(p[i].color);
							//sf::Shader::bind(shader);
							
							//window->draw(p[i].shape, shader);
							shader->setUniform("pos", sf::Vector2f(0,0));
							window->draw(p[i].vertex, 1, sf::Points, shader);
							//sf::Shader::bind(NULL);
						}
					}
					catch (const std::exception&)
					{

					}
					window->display();
				}
			}
			bool WindowShouldClose()
			{
				return close;
			}
			int c = 0;
			void update()
			{
#ifdef DEBUG
				std::cout << "\rParticles count: " << particles.size() << "        ";
#endif // DEBUG
				std::cout << "\rParticles count: " << particles.size() << "        ";
				// Обрабатываем события в цикле
				
				if(adding)
				{
					physics.addParticles(emitter.MakeParticles(10));
				}
				particles = physics.tick();
				if (particles.size())
				{
					std::copy(particles.begin(), particles.end(), p);
				}
			}
		};
	}
}


int main() {
	srand(time(NULL));
	ZecosMAX::FluidDynamics::SystemHandler* handler = new ZecosMAX::FluidDynamics::SystemHandler();
	std::thread updateThread([&handler]() -> void {
#ifdef DEBUG
		std::cout << "...\tStarted a updater thread\n";
#endif // DEBUG
		long long lastTimeMeasure = 0;
		while (!handler->WindowShouldClose())
		{
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
				);
			if (ms.count() - lastTimeMeasure >= TICK_RATE)
			{
				handler->update();
				lastTimeMeasure = ms.count();
			}
			std::this_thread::sleep_for(TICK_RATE * 0.8ms);
		}
	});
	handler->StartRenderLoop();
	//system("pause");
	updateThread.join();
	return 0;
}