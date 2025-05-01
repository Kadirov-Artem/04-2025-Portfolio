#include "Entity.h"

int gr::Entity::total = 0;
std::map <int, gr::Entity*> *gr::Entity::entity_array;
std::map <gr::IntCrd, gr::Tile*> *gr::Entity::tile_array;

gr::Entity::Entity(float ax, float ay, float aw, float ah):
	x(ax), y(ay), w(aw), h(ah), id(total++), animation_cycle(0), animation_id(0),
	vx(0), vy(0), is_moving_hor(false), is_moving_ver(false)
{
	// Setup hitbox
	hitbox.setOutlineColor(sf::Color(0, 0, 255));
	hitbox.setFillColor(sf::Color(0, 0, 0, 0));
	hitbox.setOutlineThickness(1);
	hitbox.setSize(sf::Vector2f(w * TILE_SIZE, h * TILE_SIZE));

	// Setup debug text
	/*debug_text.setFont(gr::debug_font);
	debug_text.setCharacterSize(gr::DEBUG_FONT_SIZE);
	debug_text.setFillColor(sf::Color::White);
	debug_text.setOutlineColor(sf::Color::Black);
	debug_text.setOutlineThickness(1);*/
}

gr::Entity::~Entity()
{
	--total;
}

void gr::Entity::render(sf::RenderWindow &window)
{
	// Calculate coordinates of the left top corner of the sprite on screen and render it
	// if it is captured by the camera
	int sx, sy;
	if ( cam.crdToPix(x, y, sx, sy, w, h) )
	{
		hitbox.setPosition(sx, sy);
		window.draw(hitbox);
	}
}

void gr::Entity::update_all()
{
	for (std::map <int, Entity*>::iterator it = entity_array->begin();
		 it != entity_array->end(); it++)
	{
		it->second->update();
	}
}

void gr::Entity::render_all(sf::RenderWindow &window)
{
	for (std::map <int, Entity*>::reverse_iterator it = entity_array->rbegin();
		 it != entity_array->rend(); it++)
	{
		it->second->render(window);
	}
}

void gr::Entity::debug()
{
	debug_stream.str("");  // Reset debug stream
	
	// Append debug information
	debug_stream << std::boolalpha << std::setprecision(3) << std::fixed
		<< "x; y = " << x << "; " << y << std::endl << "vx; vy = " << vx << "; " << vy << std::endl
		<< "is_moving_hor = " << is_moving_hor << std::endl
		<< "is_moving_ver = " << is_moving_ver << std::endl;
}

void gr::Entity::bind_entity_array(std::map <int, gr::Entity*> *acont)
{
	entity_array = acont;
}

void gr::Entity::bind_tile_array(std::map <IntCrd, Tile*> *acont)
{
	tile_array = acont;
}

gr::Tile *gr::Entity::getTile(int ax, int ay)
{
	if ( tile_array->count(IntCrd(ax, ay)) )
	{
		return tile_array->at(IntCrd(ax, ay));
	}
	else
	{
		return &empty_tile;
	}
}

std::string gr::Entity::get_debug_string() const
{
	return debug_stream.str();
}