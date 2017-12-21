#include "Blocks.h"
#include <SFML/Graphics/Sprite.hpp>

//Desctructable
bool block_DB::Collision()
{
	hp--;
	if (hp == 0)
	{
		sprite.setPosition(-100, 0);
	}
	return true;
}

int block_DB::GetType()
{
	return 1;
}

int block_DB::GetHP() const
{
	return hp;
}
