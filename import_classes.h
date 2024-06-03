#pragma once
#include <string>
#include <vector>

class hitbox {
public:
	int id;
	int width;
	int height;
	int x;
	int y;
};

class tile {
public:
	int id;
	int width;
	int height;
};

class frame {
public:
	int tileid;
	int duration;
};

class tileset {
public:
	int firstgid;

	std::string source;
	int sourcewidth;
	int sourceheight;

	std::string name;
	int tilecount;
	int columns;
	int tilewidth;
	int tileheight;

	frame *animation = NULL;
};

class layer {
public:
	int id;
	std::string name;
	int width;
	int height;

	std::vector<unsigned int> csv;
};

class map {
public:
	int width;
	int height;
	int tilewidth;
	int tileheight;

	std::vector<layer> layers;
	std::vector<tileset> tilesets;
};


