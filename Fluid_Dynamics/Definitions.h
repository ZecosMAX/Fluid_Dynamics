#pragma once

//#define Phys_Debug
//#define DEBUG
//#define DEBUGv2

#define Gravity 10.0f

#define WIDTH 1280
#define HEIGHT 720
#define PI std::_Pi

#define PARTICLE_CUTTER (1)

#define TIME_STAMP_M 3.0f
#define UPS 100
#define TICK_RATE (1000.0f / UPS)
#define TICK_RATE_SECONDS (1.0f / UPS)

#define IlluminantC     0.3101, 0.3162          /* For NTSC television */
#define IlluminantD65   0.3127, 0.3291          /* For EBU and SMPTE */
#define IlluminantE     0.33333333, 0.33333333  /* CIE equal-energy illuminant */
#define GAMMA_REC709    0               /* Rec. 709 */