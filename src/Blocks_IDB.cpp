#include "Blocks.h"

//Indesctructable
int block_IDB::GetType()
{
	return 2;
}

bool block_IDB::Collision()
{
	return false;
};
