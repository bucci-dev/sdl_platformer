#include <iostream>
#include <vector>
#include "import_classes.h"
#include "texture.h"

extern map graveyard_map;
extern tileset tilesets[4];
extern layer layers[4];

extern int parser();

std::vector<unsigned int> load_map(map z_map) {
    int layer_csv;
    std::vector<unsigned int> index_tileset;


    for (int y=0; y < z_map.height; y++) {
        for (int x=0; x < z_map.width; x++ ) {

            layer_csv = z_map.layers[0].csv[(y*z_map.width)+x];


            if (layer_csv >=  z_map.tilesets[0].firstgid && layer_csv < (z_map.tilesets[0].firstgid + z_map.tilesets[0].tilecount)) {

                index_tileset.push_back(layer_csv - z_map.tilesets[0].firstgid);
            }
        }

    }

return index_tileset;
}


