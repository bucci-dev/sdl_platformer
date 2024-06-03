#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <cstring>
#include <string>
#include <vector>
#include "import_classes.h"


map graveyard_map;
tileset tilesets[4];
layer layers[4];


int parser() {
	std::ifstream input_file("./gid_test.tmx");
	std::vector<std::string> input;
	char buffer[256];
	int count = 0;

	while (!input_file.fail()) {
		input_file.getline(buffer, sizeof(buffer));
		input.push_back(buffer);

		count++;
		std::cout << ".";
	}

	std::cout << std::endl << "Read " << count << " Lines" << std::endl;


	for (int x=0; x < input.size(); x++) {
		std::cout << input[x] << std::endl;
	
	}


	const char *pos_mapWidth = strstr(input[1].c_str(), "width=\"");
	const char *pos_mapHeight = strstr(input[1].c_str(), "height=\"");
	const char *pos_mapTileWidth = strstr(input[1].c_str(), "tilewidth=\"");
	const char *pos_mapTileHeight = strstr(input[1].c_str(), "tileheight=\"");


// STRING STREAMS FOR STRING TO INTEGER CONVERSION
	std::stringstream str_stream[16];


	int converted_res;
	char res[256];
	int x;
	int y;

	x = 7;
	y = 0;
	while (isdigit(pos_mapWidth[x])) {
		res[y] = pos_mapWidth[x];
		y++;
		x++;
	}
	res[y] = '\0';

	str_stream[0] << res;
	str_stream[0] >> converted_res;

	graveyard_map.width = converted_res;


	x = 8;
	y = 0;
	while (isdigit(pos_mapHeight[x])) {
		res[y] = pos_mapHeight[x];
		y++;
		x++;
	}
	res[y] = '\0';

	str_stream[1] << res;
	str_stream[1] >> converted_res;

	graveyard_map.height = converted_res;


	x = 11;
	y = 0;
	while (isdigit(pos_mapTileWidth[x])) {
		res[y] = pos_mapTileWidth[x];
		y++;
		x++;
	}
	res[y] = '\0';

	str_stream[2] << res;
	str_stream[2] >> converted_res;

	graveyard_map.tilewidth = converted_res;


	x = 12;
	y = 0;
	while (isdigit(pos_mapTileHeight[x])) {
		res[y] = pos_mapTileHeight[x];
		y++;
		x++;
	}
	res[y] = '\0';

	str_stream[3] << res;
	str_stream[3] >> converted_res;

	graveyard_map.tileheight = converted_res;



	std::cout << "result: " << graveyard_map.width << std::endl;
	std::cout << "result: " << graveyard_map.height << std::endl;
	std::cout << "result: " << graveyard_map.tilewidth << std::endl;
	std::cout << "result: " << graveyard_map.tileheight << std::endl;


	std::regex tileset("<tileset");
	std::regex layer("<layer");
	std::regex data_start("<data");
	std::regex data_end("</data");

	// USED IN TILESET IMPORT
	std::regex tile_start("<tile id=");
	std::regex tile_end("</tile");
	// USED IN TILESET IMPORT

	std::vector<int> linenumber_tileset;
	std::vector<int> linenumber_layer;
	std::vector<int> linenumber_data_start;
	std::vector<int> linenumber_data_end;

	// USED IN TILESET IMPORT
	std::vector<int> linenumber_tile_start;
	std::vector<int> linenumber_tile_end;
	// USED IN TILESET IMPORT

	for (int x=0; x < input.size(); x++) {
		if (std::regex_search(input[x], tileset)) {
			linenumber_tileset.push_back(x);
		}
	
		if (std::regex_search(input[x], layer)) {
			linenumber_layer.push_back(x);
		}

		if (std::regex_search(input[x], data_start)) {
			linenumber_data_start.push_back(x);
		}

		if (std::regex_search(input[x], data_end)) {
			linenumber_data_end.push_back(x);
		}

	}

	// EXTRACT TILESET FIRSTGID FROM .TMX MAP FILE

		std::basic_string<char> temp[16];
		std::basic_string<char> temp_output;

	for (int x=0; x < linenumber_tileset.size(); x++) {
		std::string num_test;
		int firstgid_start = 10;

		num_test = strstr(input[linenumber_tileset[x]].c_str(), "firstgid=\"")[firstgid_start];


		while (num_test != "\"") {
			temp[firstgid_start-10] = num_test;
			firstgid_start++;
			num_test = strstr(input[linenumber_tileset[x]].c_str(), "firstgid=\"")[firstgid_start];
		}

		int firstgid_end = firstgid_start - 10;

		for (int x=0; x < firstgid_end; x++) {
			temp_output.append(temp[x]);
			temp[x].clear();
		}


		tilesets[x].firstgid = atoi(temp_output.c_str());
		temp_output.clear();


		
	}

	// EXTRACT TILESET SOURCE FROM .TMX MAP FILE

	for (int x=0;x < linenumber_tileset.size(); x++ ) {
		std::string source_test;
		int source_start = 8;

		source_test = strstr(input[linenumber_tileset[x]].c_str(), "source=\"")[source_start];

		while (source_test != "\"") {
			temp_output.append(source_test);
			source_start++;
			source_test = strstr(input[linenumber_tileset[x]].c_str(), "source=\"")[source_start];
		}


		tilesets[x].source = temp_output.c_str();
		temp_output.clear();

}

	// EXTRACT LAYER ID, WIDTH, HEIGHT, DATA

	for (int x=0; x < linenumber_layer.size(); x++) {
		std::string num_test;
		int id_start = 4;
		int width_start = 7;
		int height_start = 8;


	// EXTRACT LAYER ID

		num_test = strstr(input[linenumber_layer[x]].c_str(), "id=\"")[id_start];


		while (num_test != "\"") {
			temp[id_start-4] = num_test;
			id_start++;
			num_test = strstr(input[linenumber_layer[x]].c_str(), "id=\"")[id_start];
		}

		int id_end = id_start - 4;

		for (int x=0; x < id_end; x++) {
			temp_output.append(temp[x]);
			temp[x].clear();
		}


		layers[x].id = atoi(temp_output.c_str());
		temp_output.clear();


	// EXTRACT LAYER WIDTH

		num_test = strstr(input[linenumber_layer[x]].c_str(), "width=\"")[width_start];

		while (num_test != "\"") {
			temp[width_start-7] = num_test;
			width_start++;
			num_test = strstr(input[linenumber_layer[x]].c_str(), "width=\"")[width_start];
		}

		int width_end = width_start - 7;

		for (int x=0; x < width_end; x++) {
			temp_output.append(temp[x]);
			temp[x].clear();
		}


		layers[x].width = atoi(temp_output.c_str());
		temp_output.clear();


	// EXTRACT LAYER HEIGHT

		num_test = strstr(input[linenumber_layer[x]].c_str(), "height=\"")[height_start];

		while (num_test != "\"") {
			temp[height_start-8] = num_test;
			height_start++;
			num_test = strstr(input[linenumber_layer[x]].c_str(), "height=\"")[height_start];
		}

		int height_end = height_start - 8;

		for (int x=0; x < height_end; x++) {
			temp_output.append(temp[x]);
			temp[x].clear();
		}


		layers[x].height = atoi(temp_output.c_str());
		temp_output.clear();


	// EXTRACT LAYER NAME

	for (int x=0;x < linenumber_layer.size(); x++ ) {
		std::string name_test;
		int name_start = 6;

		name_test = strstr(input[linenumber_layer[x]].c_str(), "name=\"")[name_start];

		while (name_test != "\"") {
			temp_output.append(name_test);
			name_start++;
			name_test = strstr(input[linenumber_layer[x]].c_str(), "name=\"")[name_start];
		}


		layers[x].name = temp_output.c_str();
		temp_output.clear();

}

	// EXTRACT LAYER DATA
temp_output.clear();

for (int csv_y=0; csv_y < layers[x].height; csv_y++ ) {
	int offset = 0;
	for (int csv_x=0; csv_x < layers[x].width; csv_x++ ) {

		num_test = input[linenumber_data_start[x] + csv_y].c_str()[csv_x + offset];

		while (num_test != ",") {
			temp_output.append(num_test);
			offset++;
			num_test = input[linenumber_data_start[x] + csv_y].c_str()[csv_x + offset];

}

		layers[x].csv.push_back(atoi(temp_output.c_str()));
		temp_output.clear();


}
}




}



std::cout << tilesets[0].firstgid << std::endl;
std::cout << tilesets[0].source << std::endl;

std::cout << tilesets[1].firstgid << std::endl;
std::cout << tilesets[1].source << std::endl;

std::cout << layers[0].id << std::endl;
std::cout << layers[0].width << std::endl;
std::cout << layers[0].height << std::endl;
std::cout << layers[0].name << std::endl;

//std::cout << layers[0].csv[0] << ',' << layers[0].csv[1] << std::endl;

for (int z=0; z < layers[0].csv.size(); z++) {

		std::cout << layers[0].csv[z];

		if (z % 30 == 29) {
			std::cout << std::endl;
		}
}

	// IMPORT TILESETS
input_file.close();
std::ifstream input_file2("./" + tilesets[0].source);
std::vector<std::string> input_tileset;
count = 0;


while (!input_file2.fail()) {
		input_file2.getline(buffer, sizeof(buffer));
		input_tileset.push_back(buffer);

		count++;
		std::cout << ".";
}
std::cout << std::endl << "Read " << count << " Lines" << std::endl;


const char *pos_tsetName = strstr(input_tileset[1].c_str(), "name=\"");
const char *pos_tsetColumn = strstr(input_tileset[1].c_str(), "columns=\"");
const char *pos_tsetTileCount = strstr(input_tileset[1].c_str(), "tilecount=\"");
const char *pos_tsetTileWidth = strstr(input_tileset[1].c_str(), "tilewidth=\"");
const char *pos_tsetTileHeight = strstr(input_tileset[1].c_str(), "tileheight=\"");


x = 9;
y = 0;
while (isdigit(pos_tsetColumn[x])) {
	res[y] = pos_tsetColumn[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[4] << res;
str_stream[4] >> converted_res;

tilesets[0].columns = converted_res;


x = 11;
y = 0;
while (isdigit(pos_tsetTileCount[x])) {
	res[y] = pos_tsetTileCount[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[5] << res;
str_stream[5] >> converted_res;

tilesets[0].tilecount = converted_res;


x = 11;
y = 0;
while (isdigit(pos_tsetTileWidth[x])) {
	res[y] = pos_tsetTileWidth[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[6] << res;
str_stream[6] >> converted_res;

tilesets[0].tilewidth = converted_res;


x = 12;
y = 0;
while (isdigit(pos_tsetTileHeight[x])) {
	res[y] = pos_tsetTileHeight[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[7] << res;
str_stream[7] >> converted_res;

tilesets[0].tileheight = converted_res;


	// EXTRACT TILESET NAME

for (int x=0;x < input_tileset[1].size(); x++ ) {
		std::string name_test;
		int name_start = 6;

		name_test = strstr(input_tileset[1].c_str(), "name=\"")[name_start];

		while (name_test != "\"") {
			temp_output.append(name_test);
			name_start++;
			name_test = strstr(input_tileset[1].c_str(), "name=\"")[name_start];
		}


		tilesets[0].name = temp_output.c_str();
		temp_output.clear();

}

	// EXTRACT TILESET IMAGE SOURCE PATH, WIDTH, HEIGHT

	//IMAGE SOURCE PATH
for (int x=0;x < input_tileset[2].size(); x++ ) {
		std::string source_test;
		int source_start = 8;

		source_test = strstr(input_tileset[2].c_str(), "source=\"")[source_start];

		while (source_test != "\"") {
			temp_output.append(source_test);
			source_start++;
			source_test = strstr(input_tileset[2].c_str(), "source=\"")[source_start];
		}


		tilesets[0].source = temp_output.c_str();
		temp_output.clear();

}


	//SOURCE WIDTH
const char *pos_tsetSourceWidth = strstr(input_tileset[2].c_str(), "width=\"");

x = 7;
y = 0;
while (isdigit(pos_tsetSourceWidth[x])) {
	res[y] = pos_tsetSourceWidth[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[8] << res;
str_stream[8] >> converted_res;

tilesets[0].sourcewidth = converted_res;


	//SOURCE HEIGHT
const char *pos_tsetSourceHeight = strstr(input_tileset[2].c_str(), "height=\"");

x = 8;
y = 0;
while (isdigit(pos_tsetSourceHeight[x])) {
	res[y] = pos_tsetSourceHeight[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[9] << res;
str_stream[9] >> converted_res;

tilesets[0].sourceheight = converted_res;

// EXTRACT TILESET HITBOXES, START WITH TILE ID... THEN HITBOX OBJECT
// TILE ID
//std::string tileid_test;
tile tiles_from_tileset[8];

//int pos_tsetTileID = ;



for (int x=0; x < input_tileset.size(); x++) {
	if (std::regex_search(input_tileset[x], tile_start)) {
		linenumber_tile_start.push_back(x);
	}

	if (std::regex_search(input_tileset[x], tile_end)) {
		linenumber_tile_end.push_back(x);
	}
}

//if (linenumber_tile_start.size() > 0) {

//}

for (int z=0; z < linenumber_tile_start.size(); z++) {
	//pos_tsetTileID[z] = strstr(input_tileset[linenumber_tile_start[z]].c_str(), "<tile id=\"");

	x = 11; // "<tile id=\"" + 1 from beginning of line
	y = 0;
	while (isdigit(input_tileset[linenumber_tile_start[z]][x])) {
		res[y] = input_tileset[linenumber_tile_start[z]][x];
		y++;
		x++;
	}
	res[y] = '\0';

	str_stream[10 + z] << res;
	str_stream[10 + z] >> converted_res;

	tiles_from_tileset[z].id = converted_res;

}


x = 8;
y = 0;
while (isdigit(pos_tsetSourceHeight[x])) {
	res[y] = pos_tsetSourceHeight[x];
	y++;
	x++;
}
res[y] = '\0';

str_stream[9] << res;
str_stream[9] >> converted_res;

tilesets[0].sourceheight = converted_res;


for (int x=0; x < input_tileset.size(); x++) {
		std::cout << input_tileset[x] << std::endl;

}

std::cout << tilesets[0].name << std::endl;
std::cout << tilesets[0].tilewidth << std::endl;
std::cout << tilesets[0].tileheight << std::endl;
std::cout << tilesets[0].tilecount << std::endl;
std::cout << tilesets[0].columns << std::endl;

std::cout << tilesets[0].source << std::endl;
std::cout << tilesets[0].sourcewidth << std::endl;
std::cout << tilesets[0].sourceheight << std::endl;

std::cout << "Tileset tileid: " << tiles_from_tileset[0].id << std::endl;
std::cout << "Tileset tileid: " << tiles_from_tileset[1].id << std::endl;
std::cout << "Tileset tileid: " << tiles_from_tileset[2].id << std::endl;

graveyard_map.layers.push_back(layers[0]);
graveyard_map.tilesets.push_back(tilesets[0]);

return 0;
}
