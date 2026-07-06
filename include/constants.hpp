#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <SFML/System/Vector2.hpp>


namespace Constants {

	inline constexpr float spacing       = 4.1f;
	inline constexpr float radius        = spacing/2;
	inline constexpr float kernelSupport = spacing * 2;

	inline constexpr float dt            = 0.0155f;
	inline constexpr float restDensity   = 1.1f;
	inline constexpr float mass          = restDensity * spacing * spacing;

	inline constexpr float viscosity     = 50.0f;
	inline constexpr float stiffness     = 20000.0f;

	inline constexpr sf::Vector2f gravity = { 0.f, 9.8f };

}

#endif
