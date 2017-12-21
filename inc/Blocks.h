#ifndef _BLOCKS_H_
#define _BLOCKS_H_

#include <SFML/Graphics/Sprite.hpp>

//Parent for the blocks
class block_pattern
{
//data
protected:
	int hp;             // health points of the block 
	sf::Sprite sprite;  // block's sprite

//methods
protected:
	explicit block_pattern(const sf::Texture& texture, const int health) 
		: hp(health), sprite(texture)
	{
	};

public:
	block_pattern() = delete;
	virtual ~block_pattern() = default;

	int health() const { return hp; }
	sf::Sprite& getSprite() { return sprite; }

	virtual bool Collision() = 0; // process collision for the block
	virtual int GetType() = 0;    // get type of the block
};

//Desctructable block
class block_DB : public block_pattern
{
public:
	explicit block_DB(const sf::Texture& texture, const int health = 1) : block_pattern(texture, health) {};

	bool Collision() override;
	int GetType() override;
	int GetHP() const;
};


//Indesctructable block
class block_IDB : public block_pattern
{
public:
	explicit block_IDB(const sf::Texture& texture) : block_pattern(texture, 0) {};
	int GetType() override;
	bool Collision() override;
};

#endif /*_BLOCKS_H_*/