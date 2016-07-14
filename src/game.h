/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
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
 * game.h is the header of game.cpp.
 */

#pragma once

#include "gui.h"
#include <string>

/**
 * Load the game textures.
 */
void load_game_textures (SDL_Renderer* main_renderer,
			 SDL_Texture* textures[][10]);

/**
 * Run a game.
 *
 * @param main_renderer is the renderer of the window.
 * @param save_path is the path to the save’s directory we want to load.
 * @param network_read_pipe is the read end pipe to the network fork. (Need to delete)
 * @param network_write_pipe is the write end pipe to the network fork. (Need to delete)
 * @param screen_dimensions is the dimensions of the screen.
 * @param players is a vector containing Player object. (Need to delete)
 * @return 0 if we need to quit or 1 if we need to display the main menu again.
 */
int run_game (SDL_Renderer* main_renderer, const char* save_path,
	      int network_read_pipe, int network_write_pipe,
	      struct size screen_dimensions,
	      std::vector<Player>& players);

/**
 * Destroy the game’s textures.
 */
void destroy_game_textures (SDL_Texture* player_texture, SDL_Texture* textures[][10]);

/**
 * Move screen_origin and hero_coords coordinates if a key is pressed.
 */
int move_coordinates_on_keydown (struct coordinates* screen_origin,
				 bool* keys_state,
				 int screen_height,
				 int screen_width,
				 struct size* hero_coords);

/**
 * Blit the players in “players” on the renderer “renderer”
 *
 * @param players is the vector containing every players.
 * @param screen_origin is the offset in pixels from the origin of the map.
 * @param renderer is the renderer where we blit the players.
 * @param player_texture is the texture to blit.
 * @param screen_dimensons is the screen’s dimensions.
 */
void display_players (std::vector<Player>& players,
		      struct coordinates screen_origin,
		      SDL_Renderer* renderer,
		      SDL_Texture* player_texture,
		      struct size screen_dimensions);
