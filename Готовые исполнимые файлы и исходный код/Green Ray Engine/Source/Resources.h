#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "Texture.h"

namespace gr
{

class Resources
{
private:
	std::map <Texture, sf::Texture*> textures;  // Array of all textures
public:
	/* You shouldn't create new Resources object
	Use a predefined object instead */
	Resources();  // Constructor

	~Resources();  // Destructor

	/* Should be used by game objects in order to retrieve their textures */
	sf::Texture *getTexture(Texture texture_id);  // Returns reference to the specified texture
};

extern Resources resources;  // Predefined object

}