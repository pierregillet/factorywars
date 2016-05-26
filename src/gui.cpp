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
 * gui.cpp contains the gui
 */

#include "gui.h"

SDL_Texture*
loadTexture(SDL_Renderer** Renderer, std::string path)
{
  SDL_Texture* NewTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str ());

  NewTexture = SDL_CreateTextureFromSurface (*Renderer, loadedSurface);
  SDL_FreeSurface (loadedSurface);

  return NewTexture;
}

bool 
loadMedia (SDL_Renderer** Renderer,
	   SDL_Texture** KeyPressTexture,
	   SDL_Texture** toolbar)
{
  bool success = true;
  
  // every box of the table is associated to an image
  KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] = loadTexture (Renderer, "media/textures/LEFT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_UP] = loadTexture (Renderer, "media/textures/LEFT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_UP ] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] = loadTexture (Renderer, "media/textures/RIGHT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] = loadTexture (Renderer, "media/textures/LEFT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] == NULL)
    success = false;  
    
  KeyPressTexture[KEY_PRESS_SURFACE_RIGHT ] = loadTexture (Renderer, "media/textures/RIGHT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    success = false;

  *toolbar = loadTexture (Renderer, "media/hud/toolbar.png");
  if (*toolbar == NULL)
    success = false;
  
  return success;
}

bool 
init (SDL_Window** Window,
      SDL_Renderer** Renderer,
      SDL_Texture** KeyPressTexture,
      SDL_Texture** biomes,
      SDL_Texture** items,
      SDL_Texture** toolbar,
      int* screen_height,
      int* screen_width)
{
  bool success = true;
	
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Error: %s\n", SDL_GetError ());
      success = false;
      return success;
    }
	
  // if the SDL launched correctly
  *Window = SDL_CreateWindow ("Factorywars",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      *screen_width,
			      *screen_height,
			      SDL_WINDOW_SHOWN
			      /*| SDL_WINDOW_RESIZABLE*/);
	  
  if (*Window == NULL) 
    {
      printf ("Couldn’t create window: %s\n", SDL_GetError());
      SDL_Quit();
      success = false;
      return success;
    }
	  
  // if window has been created without errors
  *Renderer = SDL_CreateRenderer (*Window,
				  -1,
				  SDL_RENDERER_ACCELERATED
				  | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawColor (*Renderer, 0xFF,0xFF,0xFF,0xFF);

  if (!loadMedia (Renderer, KeyPressTexture, toolbar))
    success = false;
  
  load_biomes (Renderer, biomes);
  load_items (Renderer, items);
  return success;
}

int
handle_keydown (SDL_Keycode event_keycode,
		bool* keys_state,
		SDL_Texture** CurrentTexture,
		SDL_Texture** key_press_texture)
{
  bool keydown = 1;
  switch (event_keycode)
    {
    case SDLK_UP:
      keys_state[0] = keydown;
      break;
    case SDLK_DOWN:
      keys_state[1] = keydown;
      break;
    case SDLK_LEFT:
      keys_state[2] = keydown;
      *CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_LEFT];
      break;
    case SDLK_RIGHT:
      keys_state[3] = keydown;
      *CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_RIGHT];
      break;
    default:
      break;
    }
  return 1;
}

int
handle_keyup (SDL_Keycode event_keycode,
	      bool* keys_state,
	      SDL_Texture** CurrentTexture,
	      SDL_Texture** key_press_texture)
{
  bool keyup = 0;
  switch (event_keycode)
    {
    case SDLK_UP:
      keys_state[0] = keyup;
      break;
    case SDLK_DOWN:
      keys_state[1] = keyup;
      break;
    case SDLK_LEFT:
      keys_state[2] = keyup;
      break;
    case SDLK_RIGHT:
      keys_state[3] = keyup;
      break;
    default:
      break;
    }
  return 1;
}

int
handle_clickdown (int button,
		  coordinates click_coords,
		  bool* clicks_state,
		  int* screen_height,
		  int* screen_width,
		  struct coordinates* screen_origin,
		  struct map_coordinates* click_map_coords)
{
  bool clickdown = 1;
  *click_map_coords = get_map_coords (click_coords,
				      screen_height,
				      screen_width,
				      *screen_origin);
  switch (button)
    {
    case SDL_BUTTON_LEFT:
	clicks_state[0] = clickdown;
	break;
    case SDL_BUTTON_MIDDLE:
	clicks_state[1] = clickdown;
	break;
    case SDL_BUTTON_RIGHT:
      clicks_state[2] = clickdown;
      break;
    case SDL_BUTTON_X1:
      clicks_state[3] = clickdown;
      break;
    case SDL_BUTTON_X2:
      clicks_state[4] = clickdown;
      break;
    default:
      break;
    }
  return 1;
}

int
handle_clickup (int button,
		coordinates click_coords,
		bool* clicks_state,
		int* screen_height,
		int* screen_width,
		struct coordinates* screen_origin)
{
  // Not necessary right now because the clicks_state array elements are put to 0 once events are handles
  /*
  bool clickup = 0;

  switch (button)
    {
    case SDL_BUTTON_LEFT:
      clicks_state[0] = clickup;
      break;
    case SDL_BUTTON_MIDDLE:
      clicks_state[1] = clickup;
      break;
    case SDL_BUTTON_RIGHT:
      clicks_state[2] = clickup;
      break;
    case SDL_BUTTON_X1:
      clicks_state[3] = clickup;
      break;
    case SDL_BUTTON_X2:
      clicks_state[4] = clickup;
      break;
    default:
      break;
    }
  */
  return 1;
}

int
handle_mousewheel (int wheel_x,
		   int* screen_height,
		   int* screen_width,
		   struct coordinates* screen_origin,
		   std::vector<Player>& players)
{
  players[0].changeSelectedTool (wheel_x);
  return 1;
}

int
handle_events (SDL_Texture** CurrentTexture,
	       SDL_Texture** biomes,
	       bool* keys_state,
	       bool* clicks_state,
	       SDL_Texture** key_press_texture,
	       int* screen_height,
	       int* screen_width,
	       struct coordinates screen_origin,
	       struct map_coordinates* click_map_coords,
	       std::vector<Player>& players)
{
  SDL_Event event;
  coordinates click_coords;
  
  while (SDL_PollEvent (&event) != 0)
    {
      if (event.key.repeat != 0)
	continue;
      switch (event.type)
	{
	case SDL_QUIT:
	  return 0;
	  break;
	      
	case SDL_KEYDOWN:
	  handle_keydown (event.key.keysym.sym,
			  keys_state,
			  CurrentTexture,
			  key_press_texture);
	  break;

	case SDL_KEYUP:
	  handle_keyup (event.key.keysym.sym,
			keys_state,
			CurrentTexture,
			key_press_texture);
	  break;

	case SDL_MOUSEBUTTONDOWN:
	  click_coords.x = event.button.x;
	  click_coords.y = event.button.y;
	  handle_clickdown (event.button.button,
			    click_coords,
			    clicks_state,
			    screen_height,
			    screen_width,
			    &screen_origin,
			    click_map_coords);
	  break;

	case SDL_MOUSEBUTTONUP:
	  click_coords.x = event.button.x;
	  click_coords.y = event.button.y;
	  handle_clickup (event.button.button,
			  click_coords,
			  clicks_state,
			  screen_height,
			  screen_width,
			  &screen_origin);
	  break;

	case SDL_MOUSEWHEEL:
	  handle_mousewheel (event.wheel.y,
			     screen_height,
			     screen_width,
			     &screen_origin,
			     players);
	  break;
	  
	default:
	  break;
	}
    }
  
  return 1;
}

int
move_coordinates_on_keydown (struct coordinates* screen_origin,
			     bool* keys_state,
			     struct coordinates* hero_coords,
			     struct coordinates screen_center)
{
  if (hero_coords -> x >= 680 && hero_coords -> y >= 400  )
  {
        screen_origin->y += (keys_state[0])? (-5) : 0;
        screen_origin->y += (keys_state[1])? 5 : 0;
        screen_origin->x += (keys_state[2])? (-5) : 0;
        screen_origin->x += (keys_state[3])? 5 : 0;
  }  
  if (screen_origin -> y <= 0 || screen_origin->x <= 0)
  {
        hero_coords->y += (keys_state[0])? (-5) : 0;
        hero_coords->y += (keys_state[1])? 5 : 0;
        hero_coords->x += (keys_state[2])? (-5) : 0;
        hero_coords->x += (keys_state[3])? 5 : 0;        
  }
  
  hero_coords->x = (hero_coords->x < 0 )? 0 : hero_coords->x;
  hero_coords->y = (hero_coords->y < 0 )? 0 : hero_coords->y;
 
  hero_coords -> x = (hero_coords -> x > 680)? 680 : hero_coords -> x;
  hero_coords -> y = (hero_coords -> y > 400)? 400 : hero_coords -> y;
 
  return 1;
}

void 
refresh_renderer (SDL_Renderer** Renderer)
{
  SDL_RenderClear (*Renderer);
}

int
blit (SDL_Renderer** Renderer,
      struct coordinates blit_origin,
      int width,
      int height,
      SDL_Texture* texture)
{
  SDL_Rect Rect = {.x = (int) blit_origin.x,
		   .y = (int) blit_origin.y,
		   .w = width, .h = height};
  // SDL_QueryTexture (texture, NULL, NULL, &Rect.w, &Rect.h);
  
  SDL_RenderSetViewport(*Renderer, &Rect);
  SDL_RenderCopy (*Renderer, texture, NULL,NULL);

  return 1;
}

void
display_blits(SDL_Renderer** Renderer)
{
  SDL_RenderPresent (*Renderer);
}

void
quit_sdl (SDL_Window** Window,
	  SDL_Renderer** Renderer,
	  SDL_Texture** CurrentTexture,
	  SDL_Texture** biomes,
	  SDL_Texture** items)
{
  for (int i = 0; i < 5; i++)
    {
      SDL_DestroyTexture (biomes[i]);
      SDL_DestroyTexture (items[i]);
    }
  
  SDL_DestroyTexture (*CurrentTexture);
  SDL_DestroyRenderer (*Renderer);
  SDL_DestroyWindow (*Window);

  SDL_Quit();
}

int 
run_gui (int read_pipe,
	 int write_pipe,
	 std::vector<Player>& players)
{
  int screen_height = atoi (get_config_value ("height"));
  int screen_width = atoi (get_config_value ("width"));
  SDL_Window *Window = NULL;
  SDL_Renderer* Renderer = NULL;

  SDL_Texture *biomes[5];
  SDL_Texture *items[5];
  SDL_Texture *key_press_texture [KEY_PRESS_SURFACE_TOTAL];
  SDL_Texture* toolbar = NULL;

  if (!init (&Window,
	     &Renderer,
	     key_press_texture,
	     biomes,
	     items,
	     &toolbar,
	     &screen_height,
	     &screen_width))
    return 1;

  struct coordinates screen_center; 
  screen_center.x = screen_width / 2;
  screen_center.y = screen_height / 2;
 
  struct coordinates screen_origin = {.x = 0,
				      .y = 0};

  struct coordinates hero_coords = {.x = screen_center.x,
				    .y = screen_center.y};
  
  SDL_Texture *CurrentTexture = NULL;
  CurrentTexture = key_press_texture [KEY_PRESS_SURFACE_DEFAULT];
  
  /* 
   * keys_state only 4 elements
   * because we only use 4 keys as of now
   * keys_state[0] -> SDLK_UP
   * keys_state[1] -> SDLK_DOWN
   * keys_state[2] -> SDLK_LEFT
   * keys_state[3] -> SDLK_RIGHT
   */
  bool keys_state[4] = {0};

  /* 
   * clicks_state[0] -> SDL_BUTTON_LEFT
   * clicks_state[1] -> SDL_BUTTON_MIDDLE
   * clicks_state[2] -> SDL_BUTTON_RIGHT
   * clicks_state[3] -> SDL_BUTTON_X1
   * clicks_state[4] -> SDL_BUTTON_X2
   */
  bool clicks_state[5] = {0};

  struct map_coordinates click_map_coords;

  // We need to display the map at the beginning
  display_background (&Renderer, "save", biomes, items, screen_origin);

  // Display character
  blit (&Renderer, screen_center, 25, 41, CurrentTexture);
  
  // Display HUD
  struct coordinates toolbar_origin = {.x = screen_width / 4 ,
				       .y = (int) (screen_height - (screen_width / 2 * 0.11))}; 
  struct coordinates toolbar_size = {.x = screen_width / 2,
				     .y = (int) (screen_width / 2 * 0.11)};
  blit (&Renderer,
	toolbar_origin,
	toolbar_size.x,
	toolbar_size.y,
	toolbar);

  display_blits(&Renderer);

  while (handle_events (&CurrentTexture,
			biomes,
			keys_state,
			clicks_state,
			key_press_texture,
			&screen_height,
			&screen_width,
			screen_origin,
			&click_map_coords,
			players) != 0)
    {
      // Keyboard handling
      for (int i = 0; i < 4; i++)
	{
	  if (keys_state[i])
	    {
              move_coordinates_on_keydown (&screen_origin, keys_state, &hero_coords, screen_center);

	      send_move_command (write_pipe, screen_origin, screen_height, screen_width);

	      refresh_renderer (&Renderer);
	      display_background (&Renderer, "save", biomes, items, screen_origin);
	      blit (&Renderer, hero_coords, 25, 41, CurrentTexture);
	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    toolbar);
	      display_blits(&Renderer);
	      break;
	    }
	}
      // End of keyboard handling

      // Left click handling
      if (clicks_state[0])
	{
	  if (get_surface_item (click_map_coords.chunk,
				click_map_coords.square,
				"save") == -1)
	    {
	      set_surface_item(click_map_coords.chunk,
			       click_map_coords.square,
			       1,
			       "save");
	      refresh_renderer (&Renderer);
	      display_background (&Renderer,
				  "save",
				  biomes,
				  items,
				  screen_origin);
	      
	      blit (&Renderer,
		    hero_coords,
		    25,
		    41,
		    CurrentTexture);
	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    toolbar);
	      display_blits(&Renderer);
	    }
	    
	  clicks_state[0] = 0;
	}
      // End of left click handling
      
      // Right click handling
      if (clicks_state[2])
	{
	  if (get_surface_item (click_map_coords.chunk,
				click_map_coords.square,
				"save") != -1)
	    {
	      set_surface_item(click_map_coords.chunk,
			       click_map_coords.square,
			       -1,
			       "save");
	      refresh_renderer (&Renderer);
	      display_background (&Renderer,
				  "save",
				  biomes,
				  items,
				  screen_origin);
	      blit (&Renderer,
		    hero_coords,
		    25,
		    41,
		    CurrentTexture);
	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    toolbar);
	      display_blits(&Renderer);
	    }
	  clicks_state[2] = 0;
	}
      // End of right click handling
      
      SDL_Delay (1/200);
    }
  quit_sdl (&Window, &Renderer, &CurrentTexture, biomes, items);
  
  return 0;
}  

struct map_coordinates
get_map_coords (struct coordinates click_coords,
		int* screen_height,
		int* screen_width,
		struct coordinates screen_origin)
{
  struct map_coordinates click_map_coords;

  float x_float = screen_origin.x;
  float y_float = screen_origin.y;

  click_map_coords.chunk.x = (int) (x_float + (float) click_coords.x) / 24.0 / 16.0;
  click_map_coords.chunk.y = (int) (y_float + (float) click_coords.y) / 24.0 / 16.0;
  click_map_coords.square.x = (int) ((x_float + (float) click_coords.x) / 24.0) - ((float) click_map_coords.chunk.x * 16.0);
  click_map_coords.square.y = (int) ((y_float + (float) click_coords.y) / 24.0) - ((float) click_map_coords.chunk.y * 16.0);

  // printf("\n chunk.x : %ld \n",click_map_coords.chunk.x);
  // printf("\n chunk.y : %ld \n",click_map_coords.chunk.y);

  // printf("\n square.x : %ld \n",click_map_coords.square.x);
  // printf("\n square.y : %ld \n",click_map_coords.square.y);

  return click_map_coords;
}

