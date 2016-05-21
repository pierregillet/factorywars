/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <corentin@nybble.fr>
 *
 * Copyright (C) 2016 Loup Fourment
 *
 * Copyright (C) 2016 Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 *
 * factorywars is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * factorywars is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * display_map.cpp contains the functions to display the map.
 */

#include "save.h"
#include "structures.h"
#include "display_map.h"
#include <stdio.h>
#include "config.h"


void load_biomes (SDL_Renderer** Renderer, SDL_Texture** table)
{
  table[0] = loadTexture (Renderer, "media/textures/biome1.png");
  table[1] = loadTexture (Renderer, "media/textures/biome1.png");    
  table[2] = loadTexture (Renderer, "media/textures/biome2.png");    
  table[3] = loadTexture (Renderer, "media/textures/biome1.png");    
  table[4] = loadTexture (Renderer, "media/textures/biome1.png");                  
  if (table[0] == NULL)
    printf("erreur %s\n", SDL_GetError()); 
}


void
display_background (SDL_Renderer** Renderer, std::string path, SDL_Texture** table, int x, int y)
{
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  const int chunk_width = NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;
  const int screen_height = atoi (get_config_value ("height"));
  const int screen_width = atoi (get_config_value ("width"));

  x = (x < 0)? 0 : x;
  y = (y < 0)? 0 : y;

  for(int i(0) ; i < screen_height + y % chunk_width; i += chunk_width)
    {
      for(int j(0); j < screen_width + x % chunk_width; j += chunk_width)
	{
	  struct coordinates hero_coords = {.x = x + j, .y = y + i};
	  struct coordinates coords = get_chunk_coordinates_from_player_movement (hero_coords);
  
	  int id = get_biome_id (coords, path.c_str ());
	  if (id == -1)
	    {
	      id = 2;
	    }

	  SDL_Texture* display_id = table[id];

	  blit (Renderer, j - x % chunk_width, i - y % chunk_width, chunk_width, chunk_width,  display_id);
	}
    }
}  
