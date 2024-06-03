#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "texture.h"
#include "import_classes.h"

extern map graveyard_map;
extern tileset tilesets[4];
extern layer layers[4];

// GRAVITY <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
float gravity_accel = 1.08;
float gravity_precast = 1;
int gravity_result = 1;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

extern int parser();
extern std::vector<unsigned int> load_map(map);

//Screen dimension constants
const int SCREEN_WIDTH = 992;
const int SCREEN_HEIGHT = 660;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

const int MAX_ANIMATION_FRAMES = 20;
SDL_Rect gSpriteClips[ MAX_ANIMATION_FRAMES ];

LTexture wiz_idle;
LTexture wiz_run;
LTexture wiz_meleeattack;
LTexture wiz_rangedattack;
LTexture wiz_rangedattackfast;
LTexture wiz_death;
LTexture wiz_hit;
LTexture wiz_jumpdown;
LTexture wiz_jumpmid;
LTexture wiz_jumpup;
LTexture wiz_stunned;

LTexture projectile_magic;
LTexture projectile_magic_impact;

LTexture background;


LTexture* spritesheet = &wiz_idle;




bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !wiz_run.loadFromFile( "./assets/wizard/spr_wizard_run.png" ) )
	{
		printf( "Failed to load wizard run animation texture!\n" );
		success = false;
	}

	if( !wiz_idle.loadFromFile( "./assets/wizard/spr_wizard_idle.png" ) )
	{
		printf( "Failed to load wizard idle animation texture!\n" );
		success = false;
	}

	if( !wiz_meleeattack.loadFromFile( "./assets/wizard/spr_wizard_meleeattack.png" ) )
	{
		printf( "Failed to load wizard meleeattack animation texture!\n" );
		success = false;
	}

	if( !wiz_death.loadFromFile( "./assets/wizard/spr_wizard_death.png" ) )
	{
		printf( "Failed to load wizard death animation texture!\n" );
		success = false;
	}

	if( !wiz_hit.loadFromFile( "./assets/wizard/spr_wizard_hit.png" ) )
	{
		printf( "Failed to load wizard hit animation texture!\n" );
		success = false;
	}

	if( !wiz_jumpdown.loadFromFile( "./assets/wizard/spr_wizard_jumpdown.png" ) )
	{
		printf( "Failed to load wizard jumpdown animation texture!\n" );
		success = false;
	}

	if( !wiz_jumpmid.loadFromFile( "./assets/wizard/spr_wizard_jumpmid.png" ) )
	{
		printf( "Failed to load wizard jumpmid animation texture!\n" );
		success = false;
	}

	if( !wiz_jumpup.loadFromFile( "./assets/wizard/spr_wizard_jumpup.png" ) )
	{
		printf( "Failed to load wizard jumpup animation texture!\n" );
		success = false;
	}

	if( !wiz_rangedattack.loadFromFile( "./assets/wizard/spr_wizard_rangedattack.png" ) )
	{
		printf( "Failed to load wizard rangedattack animation texture!\n" );
		success = false;
	}

	if( !wiz_rangedattackfast.loadFromFile( "./assets/wizard/spr_wizard_rangedattackfast.png" ) )
	{
		printf( "Failed to load wizard rangedattackfast animation texture!\n" );
		success = false;
	}

	if( !wiz_stunned.loadFromFile( "./assets/wizard/spr_wizard_stunned.png" ) )
	{
		printf( "Failed to load wizard stunned animation texture!\n" );
		success = false;
	}

	if( !projectile_magic.loadFromFile( "./assets/wizard/spr_magic_projectile.png" ) )
	{
		printf( "Failed to load projectile magic animation texture!\n" );
		success = false;
	}

	if( !projectile_magic_impact.loadFromFile( "./assets/wizard/spr_magic_projectileimpact.png" ) )
	{
		printf( "Failed to load projectile magic impact animation texture!\n" );
		success = false;
	}

	if( !background.loadFromFile( graveyard_map.tilesets[0].source ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	else
	{
		//Set sprite clips
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w = 128;
		gSpriteClips[ 0 ].h = 128;

		gSpriteClips[ 1 ].x = 128;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w = 128;
		gSpriteClips[ 1 ].h = 128;
		
		gSpriteClips[ 2 ].x = 256;
		gSpriteClips[ 2 ].y =   0;
		gSpriteClips[ 2 ].w = 128;
		gSpriteClips[ 2 ].h = 128;

		gSpriteClips[ 3 ].x = 384;
		gSpriteClips[ 3 ].y =   0;
		gSpriteClips[ 3 ].w = 128;
		gSpriteClips[ 3 ].h = 128;

		gSpriteClips[ 4 ].x = 512;
		gSpriteClips[ 4 ].y =   0;
		gSpriteClips[ 4 ].w = 128;
		gSpriteClips[ 4 ].h = 128;

		gSpriteClips[ 5 ].x = 640;
		gSpriteClips[ 5 ].y =   0;
		gSpriteClips[ 5 ].w = 128;
		gSpriteClips[ 5 ].h = 128;

		gSpriteClips[ 6 ].x = 768;
		gSpriteClips[ 6 ].y =   0;
		gSpriteClips[ 6 ].w = 128;
		gSpriteClips[ 6 ].h = 128;

		gSpriteClips[ 7 ].x = 896;
		gSpriteClips[ 7 ].y =   0;
		gSpriteClips[ 7 ].w = 128;
		gSpriteClips[ 7 ].h = 128;

		gSpriteClips[ 8 ].x = 1024;
		gSpriteClips[ 8 ].y =   0;
		gSpriteClips[ 8 ].w = 128;
		gSpriteClips[ 8 ].h = 128;

		gSpriteClips[ 9 ].x = 1152;
		gSpriteClips[ 9 ].y =   0;
		gSpriteClips[ 9 ].w = 128;
		gSpriteClips[ 9 ].h = 128;

		gSpriteClips[ 10 ].x = 1280;
		gSpriteClips[ 10 ].y =   0;
		gSpriteClips[ 10 ].w = 128;
		gSpriteClips[ 10 ].h = 128;

		gSpriteClips[ 11 ].x = 1408;
		gSpriteClips[ 11 ].y =   0;
		gSpriteClips[ 11 ].w = 128;
		gSpriteClips[ 11 ].h = 128;

		gSpriteClips[ 12 ].x = 1536;
		gSpriteClips[ 12 ].y =   0;
		gSpriteClips[ 12 ].w = 128;
		gSpriteClips[ 12 ].h = 128;

		gSpriteClips[ 13 ].x = 1664;
		gSpriteClips[ 13 ].y =   0;
		gSpriteClips[ 13 ].w = 128;
		gSpriteClips[ 13 ].h = 128;

		gSpriteClips[ 14 ].x = 1792;
		gSpriteClips[ 14 ].y =   0;
		gSpriteClips[ 14 ].w = 128;
		gSpriteClips[ 14 ].h = 128;

		gSpriteClips[ 15 ].x = 1920;
		gSpriteClips[ 15 ].y =   0;
		gSpriteClips[ 15 ].w = 128;
		gSpriteClips[ 15 ].h = 128;

		gSpriteClips[ 16 ].x = 2048;
		gSpriteClips[ 16 ].y =   0;
		gSpriteClips[ 16 ].w = 128;
		gSpriteClips[ 16 ].h = 128;

		gSpriteClips[ 17 ].x = 2176;
		gSpriteClips[ 17 ].y =   0;
		gSpriteClips[ 17 ].w = 128;
		gSpriteClips[ 17 ].h = 128;

		gSpriteClips[ 18 ].x = 2304;
		gSpriteClips[ 18 ].y =   0;
		gSpriteClips[ 18 ].w = 128;
		gSpriteClips[ 18 ].h = 128;

		gSpriteClips[ 19 ].x = 2432;
		gSpriteClips[ 19 ].y =   0;
		gSpriteClips[ 19 ].w = 128;
		gSpriteClips[ 19 ].h = 128;



	}
	
	return success;
}

void close()
{
	//Free loaded images
	wiz_idle.free();
	wiz_run.free();
	wiz_meleeattack.free();
	wiz_rangedattack.free();
	wiz_rangedattackfast.free();
	wiz_death.free();
	wiz_hit.free();
	wiz_jumpdown.free();
	wiz_jumpmid.free();
	wiz_jumpup.free();
	wiz_stunned.free();

	projectile_magic.free();
	projectile_magic_impact.free();

	background.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	// PIPE THIS TO PARSER
	parser();
	std::vector<unsigned int> loaded_index = load_map(graveyard_map);
	int map_total_tiles = graveyard_map.width * graveyard_map.height;
	SDL_Rect loaded_map_clips[map_total_tiles];

	for (int x=0; x < map_total_tiles; x++) {
		loaded_map_clips[x].x = (loaded_index[x] * 32) % 256;
		loaded_map_clips[x].y = (((loaded_index[x] * 32) - loaded_map_clips[x].x) / 256) * 32;
		loaded_map_clips[x].w = 32;
		loaded_map_clips[x].h = 32;


}

    std::cout << "-----------------------" << std::endl;
    std::cout << graveyard_map.tileheight << std::endl;
    std::cout << graveyard_map.tilewidth << std::endl;
    std::cout << graveyard_map.height << std::endl;
    std::cout << graveyard_map.width << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << graveyard_map.layers[0].name << std::endl;
    std::cout << graveyard_map.tilesets[0].name << std::endl;
	// PIPE THIS TO PARSER
    std::cout << "-----------------------" << std::endl;
	for (int x=0; x < loaded_index.size(); x++) {
		std::cout << loaded_index[x] << std::endl;
	}
	std::cout << "-----------------------" << std::endl;
	std::cout << "Loaded_index size: " << loaded_index.size() << std::endl;




	wiz_idle.setFrameCount(8);
	wiz_idle.setFrameWidth(128);
	wiz_idle.setFrameHeight(128);
	wiz_run.setFrameCount(8);
	wiz_run.setFrameWidth(128);
	wiz_run.setFrameHeight(128);
	wiz_meleeattack.setFrameCount(6);
	wiz_meleeattack.setFrameWidth(128);
	wiz_meleeattack.setFrameHeight(128);
	wiz_rangedattack.setFrameCount(20);
	wiz_rangedattack.setFrameWidth(128);
	wiz_rangedattack.setFrameHeight(128);
	wiz_rangedattackfast.setFrameCount(10);
	wiz_rangedattackfast.setFrameWidth(128);
	wiz_rangedattackfast.setFrameHeight(128);
	wiz_death.setFrameCount(19);
	wiz_death.setFrameWidth(128);
	wiz_death.setFrameHeight(128);
	wiz_hit.setFrameCount(4);
	wiz_hit.setFrameWidth(128);
	wiz_hit.setFrameHeight(128);
	wiz_jumpdown.setFrameCount(4);
	wiz_jumpdown.setFrameWidth(128);
	wiz_jumpdown.setFrameHeight(128);
	wiz_jumpmid.setFrameCount(3);
	wiz_jumpmid.setFrameWidth(128);
	wiz_jumpmid.setFrameHeight(128);
	wiz_jumpup.setFrameCount(4);
	wiz_jumpup.setFrameWidth(128);
	wiz_jumpup.setFrameHeight(128);
	wiz_stunned.setFrameCount(4);
	wiz_stunned.setFrameWidth(128);
	wiz_stunned.setFrameHeight(128);

	projectile_magic.setFrameCount(8);
	projectile_magic.setFrameWidth(30);
	projectile_magic.setFrameHeight(23);
	projectile_magic_impact.setFrameCount(6);
	projectile_magic_impact.setFrameWidth(28);
	projectile_magic_impact.setFrameHeight(32);

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Current animation frame
			int frame = 0;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//User presses a key
					else if( e.type == SDL_KEYDOWN )
					{
						//Select surfaces based on key press
						switch( e.key.keysym.sym )
						{
							case SDLK_RIGHT:
							spritesheet = &wiz_run;
							break;


							case SDLK_SPACE:
							spritesheet = &wiz_meleeattack;
							break;

							case SDLK_1:
							spritesheet = &wiz_death;
							break;

							case SDLK_2:
							spritesheet = &wiz_hit;
							break;

							case SDLK_3:
							spritesheet = &wiz_jumpdown;
							break;

							case SDLK_4:
							spritesheet = &wiz_jumpmid;
							break;

							case SDLK_5:
							spritesheet = &wiz_jumpup;
							break;

							case SDLK_6:
							spritesheet = &wiz_rangedattack;
							break;

							case SDLK_7:
							spritesheet = &wiz_rangedattackfast;
							break;

							case SDLK_8:
							spritesheet = &wiz_stunned;
							break;

							case SDLK_9:
							spritesheet = &projectile_magic;
							break;

							case SDLK_0:
							spritesheet = &projectile_magic_impact;
							break;

							case SDLK_g:
							gravity_precast = 1;
							gravity_accel = 1.08;
							break;

							default:
							spritesheet = &wiz_idle;
							break;
						}
					}

					else if (e.type == SDL_KEYUP) {

						spritesheet = &wiz_idle;

					}


				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current frame
				SDL_Rect* currentClip = &gSpriteClips[ frame / 5 ];

// PIPE BACKGROUND.RENDER TO LOAD_MAP FUNCTION AND USE VECTOR<INDEX_TILESET> AS CLIPS   <<<<<<<--------------------
				int clip_index = 0;



				for (int y=0; y < graveyard_map.height; y++ ) {

					for (int x=0; x < graveyard_map.width; x++) {
							//SDL_Rect clip;
							//clip.x = (loaded_index[clip_index] % graveyard_map.tilesets[0].sourcewidth) * clip.w;
							//clip.y = ((loaded_index[clip_index] - (loaded_index[clip_index] % graveyard_map.tilesets[0].sourcewidth)) / graveyard_map.tilesets[0].sourcewidth) * clip.h;

							//clip.h = 32;
							//clip.w = 32;
						//clip_index = (y*graveyard_map.width) + x;


						background.render(x * graveyard_map.tilewidth, y * graveyard_map.tileheight, &loaded_map_clips[clip_index]);
						clip_index++;
					}
				}

				// HITBOX TEST
				hitbox test_box;
				int x_render = ( SCREEN_WIDTH - currentClip->w ) / 2;
				int y_render = (( SCREEN_HEIGHT - 350 - currentClip->h ) / 2) + gravity_result;
				test_box.height = 128;
				test_box.width = 128;
				test_box.x = x_render;
				test_box.y = y_render - gravity_result + 300;

				spritesheet->render( x_render, y_render, currentClip );

				gravity_precast *= gravity_accel;
				float gravity_precast_next = gravity_precast * gravity_accel;
				gravity_result = (int)gravity_precast;
				int gravity_result_next = (int)gravity_precast_next;

				if (y_render - gravity_result + gravity_result_next > test_box.y) {
					gravity_accel = 1;
				}


				//Update screen
				SDL_RenderPresent( gRenderer );

				//Go to next frame
				++frame;

				//Cycle animation
				if ( frame / 5 >= spritesheet->getFrameCount() )
				{
					frame = 0;

				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
