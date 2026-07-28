#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>


namespace Constants {

	inline constexpr float spacing       = 4.1f;
	inline constexpr float radius        = spacing/2;
	inline constexpr float kernelSupport = spacing * 2;

	inline constexpr float dt            = 0.0035f;
	inline constexpr float restDensity   = 1.1f;
	inline constexpr float mass          = restDensity * spacing * spacing;

	inline constexpr float viscosity     = 80.0f;
	inline constexpr float stiffness     = 16000.0f;

	inline constexpr sf::Vector2f gravity = { 0.f, 9.8f };

	inline constexpr float windowHeight = 1200;
	inline constexpr float windowWidth = 800;


	constexpr float kernelAlpha =
        5.0f / (14.0f * M_PI * spacing * spacing);

}


namespace CustomColors {

    inline constexpr sf::Color Background{22, 24, 32};
    inline constexpr sf::Color Fluid{90, 140, 190};
    inline constexpr sf::Color Boundary{140, 140, 150};


}

#endif
