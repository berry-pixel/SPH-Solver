#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>


namespace Constants {

	inline constexpr float spacing       = 4.1f;
	inline constexpr float radius        = spacing/2;
	inline constexpr float kernelSupport = spacing * 2;

	inline constexpr float dt            = 0.0255f;
	inline constexpr float restDensity   = 1.1f;
	inline constexpr float mass          = restDensity * spacing * spacing;

	inline constexpr float viscosity     = 50.0f;
	inline constexpr float stiffness     = 15000.0f;

	inline constexpr sf::Vector2f gravity = { 0.f, 9.8f };

}


namespace CustomColors {

    inline constexpr sf::Color Background{22, 24, 32};
    inline constexpr sf::Color Fluid{90, 140, 190};
    inline constexpr sf::Color Boundary{140, 140, 150};


}

#endif
