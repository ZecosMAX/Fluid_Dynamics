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
		struct colourSystem {
			const char *name;                     /* Colour system name */
			float xRed, yRed,              /* Red x, y */
				xGreen, yGreen,          /* Green x, y */
				xBlue, yBlue,            /* Blue x, y */
				xWhite, yWhite,          /* White point x, y */
				gamma;                   /* Gamma correction for system */
		};
		class Particle
		{
		private:
			long long lastTimeMeasure = 0;
			const struct colourSystem
				/*				Name					xRed    yRed    xGreen  yGreen  xBlue  yBlue    White point        Gamma   */
				NTSCsystem =	{ "NTSC",               0.67,   0.33,   0.21,   0.71,   0.14,   0.08,   IlluminantC,    GAMMA_REC709 },
				EBUsystem =		{ "EBU (PAL/SECAM)",    0.64,   0.33,   0.29,   0.60,   0.15,   0.06,   IlluminantD65,  GAMMA_REC709 },
				SMPTEsystem =	{ "SMPTE",              0.630,  0.340,  0.310,  0.595,  0.155,  0.070,  IlluminantD65,  GAMMA_REC709 },
				HDTVsystem =	{ "HDTV",               0.670,  0.330,  0.210,  0.710,  0.150,  0.060,  IlluminantD65,  GAMMA_REC709 },
				CIEsystem =		{ "CIE",                0.7355, 0.2645, 0.2658, 0.7243, 0.1669, 0.0085, IlluminantE,    GAMMA_REC709 },
				Rec709system =	{ "CIE REC 709",        0.64,   0.33,   0.30,   0.60,   0.15,   0.06,   IlluminantD65,  GAMMA_REC709 };
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
			float bbTemp = 5000;
			float bb_spectrum(float wavelength)
			{
				float wlm = wavelength * 1e-9;   /* Wavelength in meters */

				return (3.74183e-16 * pow(wlm, -5.0)) /
					(exp(1.4388e-2 / (wlm * bbTemp)) - 1.0);
			}
			void spectrum_to_xyz(float(*spec_intens)(float wavelength), float *x, float *y, float *z)
			{
				int i;
				float lambda, X = 0, Y = 0, Z = 0, XYZ;

				/* CIE colour matching functions xBar, yBar, and zBar for
				   wavelengths from 380 through 780 nanometers, every 5
				   nanometers.  For a wavelength lambda in this range:

						cie_colour_match[(lambda - 380) / 5][0] = xBar
						cie_colour_match[(lambda - 380) / 5][1] = yBar
						cie_colour_match[(lambda - 380) / 5][2] = zBar

					To save memory, this table can be declared as floats
					rather than floats; (IEEE) float has enough
					significant bits to represent the values. It's declared
					as a float here to avoid warnings about "conversion
					between floating-point types" from certain persnickety
					compilers. */

				static float cie_colour_match[81][3] = {
					{0.0014,0.0000,0.0065}, {0.0022,0.0001,0.0105}, {0.0042,0.0001,0.0201},
					{0.0076,0.0002,0.0362}, {0.0143,0.0004,0.0679}, {0.0232,0.0006,0.1102},
					{0.0435,0.0012,0.2074}, {0.0776,0.0022,0.3713}, {0.1344,0.0040,0.6456},
					{0.2148,0.0073,1.0391}, {0.2839,0.0116,1.3856}, {0.3285,0.0168,1.6230},
					{0.3483,0.0230,1.7471}, {0.3481,0.0298,1.7826}, {0.3362,0.0380,1.7721},
					{0.3187,0.0480,1.7441}, {0.2908,0.0600,1.6692}, {0.2511,0.0739,1.5281},
					{0.1954,0.0910,1.2876}, {0.1421,0.1126,1.0419}, {0.0956,0.1390,0.8130},
					{0.0580,0.1693,0.6162}, {0.0320,0.2080,0.4652}, {0.0147,0.2586,0.3533},
					{0.0049,0.3230,0.2720}, {0.0024,0.4073,0.2123}, {0.0093,0.5030,0.1582},
					{0.0291,0.6082,0.1117}, {0.0633,0.7100,0.0782}, {0.1096,0.7932,0.0573},
					{0.1655,0.8620,0.0422}, {0.2257,0.9149,0.0298}, {0.2904,0.9540,0.0203},
					{0.3597,0.9803,0.0134}, {0.4334,0.9950,0.0087}, {0.5121,1.0000,0.0057},
					{0.5945,0.9950,0.0039}, {0.6784,0.9786,0.0027}, {0.7621,0.9520,0.0021},
					{0.8425,0.9154,0.0018}, {0.9163,0.8700,0.0017}, {0.9786,0.8163,0.0014},
					{1.0263,0.7570,0.0011}, {1.0567,0.6949,0.0010}, {1.0622,0.6310,0.0008},
					{1.0456,0.5668,0.0006}, {1.0026,0.5030,0.0003}, {0.9384,0.4412,0.0002},
					{0.8544,0.3810,0.0002}, {0.7514,0.3210,0.0001}, {0.6424,0.2650,0.0000},
					{0.5419,0.2170,0.0000}, {0.4479,0.1750,0.0000}, {0.3608,0.1382,0.0000},
					{0.2835,0.1070,0.0000}, {0.2187,0.0816,0.0000}, {0.1649,0.0610,0.0000},
					{0.1212,0.0446,0.0000}, {0.0874,0.0320,0.0000}, {0.0636,0.0232,0.0000},
					{0.0468,0.0170,0.0000}, {0.0329,0.0119,0.0000}, {0.0227,0.0082,0.0000},
					{0.0158,0.0057,0.0000}, {0.0114,0.0041,0.0000}, {0.0081,0.0029,0.0000},
					{0.0058,0.0021,0.0000}, {0.0041,0.0015,0.0000}, {0.0029,0.0010,0.0000},
					{0.0020,0.0007,0.0000}, {0.0014,0.0005,0.0000}, {0.0010,0.0004,0.0000},
					{0.0007,0.0002,0.0000}, {0.0005,0.0002,0.0000}, {0.0003,0.0001,0.0000},
					{0.0002,0.0001,0.0000}, {0.0002,0.0001,0.0000}, {0.0001,0.0000,0.0000},
					{0.0001,0.0000,0.0000}, {0.0001,0.0000,0.0000}, {0.0000,0.0000,0.0000}
				};

				for (i = 0, lambda = 380; lambda < 780.1; i++, lambda += 5) {
					float Me;

					Me = (*spec_intens)(lambda);
					X += Me * cie_colour_match[i][0];
					Y += Me * cie_colour_match[i][1];
					Z += Me * cie_colour_match[i][2];
				}
				XYZ = (X + Y + Z);
				*x = X / XYZ;
				*y = Y / XYZ;
				*z = Z / XYZ;
			}
			void xyz_to_rgb(struct colourSystem *cs,
				float xc, float yc, float zc,
				float *r, float *g, float *b)
			{
				float xr, yr, zr, xg, yg, zg, xb, yb, zb;
				float xw, yw, zw;
				float rx, ry, rz, gx, gy, gz, bx, by, bz;
				float rw, gw, bw;

				xr = cs->xRed;    yr = cs->yRed;    zr = 1 - (xr + yr);
				xg = cs->xGreen;  yg = cs->yGreen;  zg = 1 - (xg + yg);
				xb = cs->xBlue;   yb = cs->yBlue;   zb = 1 - (xb + yb);

				xw = cs->xWhite;  yw = cs->yWhite;  zw = 1 - (xw + yw);

				/* xyz -> rgb matrix, before scaling to white. */

				rx = (yg * zb) - (yb * zg);  ry = (xb * zg) - (xg * zb);  rz = (xg * yb) - (xb * yg);
				gx = (yb * zr) - (yr * zb);  gy = (xr * zb) - (xb * zr);  gz = (xb * yr) - (xr * yb);
				bx = (yr * zg) - (yg * zr);  by = (xg * zr) - (xr * zg);  bz = (xr * yg) - (xg * yr);

				/* White scaling factors.
				   Dividing by yw scales the white luminance to unity, as conventional. */

				rw = ((rx * xw) + (ry * yw) + (rz * zw)) / yw;
				gw = ((gx * xw) + (gy * yw) + (gz * zw)) / yw;
				bw = ((bx * xw) + (by * yw) + (bz * zw)) / yw;

				/* xyz -> rgb matrix, correctly scaled to white. */

				rx = rx / rw;  ry = ry / rw;  rz = rz / rw;
				gx = gx / gw;  gy = gy / gw;  gz = gz / gw;
				bx = bx / bw;  by = by / bw;  bz = bz / bw;

				/* rgb of the desired point */

				*r = (rx * xc) + (ry * yc) + (rz * zc);
				*g = (gx * xc) + (gy * yc) + (gz * zc);
				*b = (bx * xc) + (by * yc) + (bz * zc);
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
			sf::Vertex vertex[1];
			sf::Color color = sf::Color(0, 0, 20);

			Particle() {
			}
			Particle(sf::Vector2f pos, bool random) {

			}
			Particle(sf::Vector2f pos, sf::Vector2f speed, sf::Vector2f acc) {
				
				shape = *new sf::CircleShape(20.0f);
				this->pos = pos;
				this->speed = speed;
				this->acceleration = acc;

				vertex[0] = sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Vector2f(1, 1));

				//vertex[0] = sf::Vertex(sf::Vector2f(pos.x + someOffset, pos.y + someOffset), sf::Vector2f(1, 1));
				//vertex[1] = sf::Vertex(sf::Vector2f(pos.x + someOffset, pos.y - someOffset), sf::Vector2f(1, 0));
				//vertex[2] = sf::Vertex(sf::Vector2f(pos.x - someOffset, pos.y - someOffset), sf::Vector2f(0, 0));
				//vertex[3] = sf::Vertex(sf::Vector2f(pos.x - someOffset, pos.y + someOffset), sf::Vector2f(0, 1));

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
				float t, x, y, z, r, g, b;

				speed += acceleration * TICK_RATE_SECONDS * TIME_STAMP_M;
				pos += speed * TICK_RATE_SECONDS * TIME_STAMP_M;
				bbTemp = (speed.x * speed.x + speed.y * speed.y);
				spectrum_to_xyz(bb_spectrum, &x, &y, &z);
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
				vertex[0].position = sf::Vector2f(pos.x, pos.y);
			}
		};
	}
}