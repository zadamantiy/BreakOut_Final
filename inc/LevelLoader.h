#ifndef _LEVELLOADER_H_
#define _LEVELLOADER_H_

#include <vector>

class LevelLoader
{
//data
private:
	int h;                   // field height 
	int w;                   // field width
	std::vector<int> level;  // array of blocks in one level
	std::string levelName;   // name of the level

//methods
public:   
	void Load_level(std::string lvlName);

	int getType(int n) const;
	std::string getName() const;
	
	int getHeight() const { return h; }
	int getWidth() const { return w; }
};

#endif  /*_LEVELLOADER_H_*/