#include "Resources.h"

gr::Resources gr::resources;

gr::Resources::Resources()
{
	// Load all game resources
	// Load textures

	sf::Texture tmp;

	tmp.loadFromFile("Stone.png");
	textures.emplace(Texture::STONE_TEXTURE, new sf::Texture(tmp));

	tmp.loadFromFile("Ladder.png");
	textures.emplace(Texture::LADDER_TEXTURE, new sf::Texture(tmp));
}

gr::Resources::~Resources()
{
	for (std::map <Texture, sf::Texture*>::iterator it = textures.begin();
		 it != textures.end(); it++)
	{
		delete it->second;
	}
}

sf::Texture *gr::Resources::getTexture(Texture texture_id)
{
	return textures.at(texture_id);
}