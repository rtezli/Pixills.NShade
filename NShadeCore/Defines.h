#pragma once

#define WIN32_LEAN_AND_MEAN

#define DIRECTINPUT_VERSION 0x0800

#ifndef FPS
#define FPS(fps) {std::chrono::milliseconds(1000 / fps)}
#endif