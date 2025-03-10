/*
  u8x8_d_sdl_128x64.c
*/

#include "u8x8.h"
#include "SDL.h"
#include "SDL_video.h"
#include <assert.h>

#define HEIGHT (64)
#define WIDTH 128

SDL_Surface *u8g_sdl_screen;
int u8g_sdl_multiple = 2;
Uint32 u8g_sdl_color[256];

void u8g_sdl_set_pixel(int x, int y, int idx)
{
  Uint32  *ptr;
  Uint32 offset;
  int i, j;
  
  if ( y >= HEIGHT )
    return;
  if ( y < 0 )
    return;
  if ( x >= WIDTH )
    return;
  if ( x < 0 )
    return;
  
  for( i = 0; i < u8g_sdl_multiple; i++ )
    for( j = 0; j < u8g_sdl_multiple; j++ )
    {
      offset = 
	(y * u8g_sdl_multiple + j) * u8g_sdl_screen->w * u8g_sdl_screen->format->BytesPerPixel + 
	(x * u8g_sdl_multiple + i) * u8g_sdl_screen->format->BytesPerPixel;
      assert( offset < u8g_sdl_screen->w * u8g_sdl_screen->h * u8g_sdl_screen->format->BytesPerPixel );
      ptr = u8g_sdl_screen->pixels + offset;
      *ptr = u8g_sdl_color[idx];
    }
}

void u8g_sdl_set_8pixel(int x, int y, uint8_t pixel)
{
  int cnt = 8;
  while( cnt > 0 )
  {
    if ( (pixel & 1) == 0 )
    {
      u8g_sdl_set_pixel(x,y,0);
    }
    else
    {
      u8g_sdl_set_pixel(x,y,3);
    }
    pixel >>= 1;
    y++;
    cnt--;
  }
}

void u8g_sdl_set_multiple_8pixel(int x, int y, int cnt, uint8_t *pixel)
{
  uint8_t b;
  while( cnt > 0 )
  {
    b = *pixel;
    u8g_sdl_set_8pixel(x, y, b);
    x++;
    pixel++;
    cnt--;
  }
}


#define W(x,w) (((x)*(w))/100)

void u8g_sdl_init(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) 
  {
    printf("Unable to initialize SDL:  %s\n", SDL_GetError());
    exit(1);
  }
  
  /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_SetVideoMode */
  u8g_sdl_screen = SDL_SetVideoMode(WIDTH*u8g_sdl_multiple,HEIGHT*u8g_sdl_multiple,32,SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( u8g_sdl_screen == NULL ) 
  {
    printf("Couldn't set video mode: %s\n", SDL_GetError());
    exit(1);
  }
  //printf("%d bits-per-pixel mode\n", u8g_sdl_screen->format->BitsPerPixel);
  //printf("%d bytes-per-pixel mode\n", u8g_sdl_screen->format->BytesPerPixel);
  
  u8g_sdl_color[0] = SDL_MapRGB( u8g_sdl_screen->format, 0, 0, 0 );
  u8g_sdl_color[1] = SDL_MapRGB( u8g_sdl_screen->format, W(100, 50), W(255,50), 0 );
  u8g_sdl_color[2] = SDL_MapRGB( u8g_sdl_screen->format, W(100, 80), W(255,80), 0 );
  u8g_sdl_color[3] = SDL_MapRGB( u8g_sdl_screen->format, 100, 255, 0 );

  /*
  u8g_sdl_set_pixel(0,0);
  u8g_sdl_set_pixel(1,1);
  u8g_sdl_set_pixel(2,2);
  */

  /* update all */
  /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_UpdateRect */
  SDL_UpdateRect(u8g_sdl_screen, 0,0,0,0);

  atexit(SDL_Quit);
  return;
}

/* return ascii key value or -1 */
int u8g_sdl_get_key(void)
{
  SDL_Event event;
	/* http://www.libsdl.org/cgi/docwiki.cgi/SDL_PollEvent */
  if ( SDL_PollEvent(&event) != 0 )
  {
    switch (event.type) 
    {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_KEYDOWN:
          switch( event.key.keysym.sym )
          {
            /*     /usr/include/SDL/SDL_keysym.h */
            case SDLK_a: return 'a';
            case SDLK_b: return 'b';
            case SDLK_c: return 'c';
            case SDLK_d: return 'd';
            case SDLK_e: return 'e';
            case SDLK_f: return 'f';
            case SDLK_g: return 'g';
            case SDLK_h: return 'h';
            case SDLK_i: return 'i';
            case SDLK_j: return 'j';
            case SDLK_k: return 'k';
            case SDLK_l: return 'l';
            case SDLK_m: return 'm';
            case SDLK_n: return 'n';
            case SDLK_o: return 'o';
            case SDLK_p: return 'p';
            case SDLK_q: return 'q';
            case SDLK_r: return 'r';
            case SDLK_s: return 's';
            case SDLK_t: return 't';
            case SDLK_u: return 'u';
            case SDLK_v: return 'v';
            case SDLK_w: return 'w';
            case SDLK_x: return 'x';
            case SDLK_y: return 'y';
            case SDLK_z: return 'z';
            case SDLK_SPACE: return ' ';
            case SDLK_UP: return 273;
            case SDLK_DOWN: return 274;
            case SDLK_RIGHT: return 275;
            case SDLK_LEFT: return 276;
            
            default: return 0;
          }
    }
  }
  return -1;
}


/*
void main(void)
{
  u8g_sdl_init();
  u8g_sdl_set_pixel(0,0,3);
  u8g_sdl_set_pixel(0,1,3);
  u8g_sdl_set_pixel(0,2,3);
  u8g_sdl_set_pixel(1,1,3);
  u8g_sdl_set_pixel(2,2,3);
  while( u8g_sdl_get_key() < 0 )
    ;
}
*/

static const u8x8_display_info_t u8x8_sdl_128x64_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_takeover_edge = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 16,
  /* tile_hight = */ 8,
#if U8X8_DEFAULT_FLIP_MODE == 0 
  /* default_x_offset = */ 0,
#else
  /* default_x_offset = */ 0,
#endif
};


uint8_t u8x8_d_sdl(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2, &u8x8_sdl_128x64_info);
      u8g_sdl_init();
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      x *= 8;
      x += u8g2->x_offset;
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      y *= 8;
    
      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	u8g_sdl_set_multiple_8pixel(x, y, c*8, ptr);
	arg_int--;
      } while( arg_int > 0 );

      /* update all */
      /* http://www.libsdl.org/cgi/docwiki.cgi/SDL_UpdateRect */
      SDL_UpdateRect(u8g_sdl_screen, 0,0,0,0);
      
      break;
    default:
      return 0;
  }
  return 1;
}


void u8x8_Setup_SDL_128x64(u8x8_t *u8g2)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8g2);
  
  /* setup specific callbacks */
  u8g2->display_cb = u8x8_d_sdl;
  
}

void u8g2_Setup_SDL_128x64(u8g2_t *u8g2)
{
  
  static uint8_t buf[128*8];
  /*
  int i;  
  for( i = 0; i < 128*8; i++ )
    buf[i] = (i>>2);
  */
  
  u8x8_Setup_SDL_128x64(u8g2_GetU8x8(u8g2));
  u8g2->tile_buf_ptr = buf;
  u8g2->tile_buf_height = 8;
  u8g2->tile_curr_row = 0;
  u8g2->draw_color = 1;
}

void u8g2_draw_pixel(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y)
{
  uint8_t *ptr;
  uint8_t bit_pos, mask;
  uint16_t offset;
  ptr = u8g2->tile_buf_ptr;
  /* bytes are vertical, lsb on top (y=0), msb at bottom (y=7) */
  bit_pos = y;		/* overflow truncate is ok here... */
  bit_pos &= 7; 	/* ... because only the lowest 3 bits are needed */
  y &= ~7;		/* zero the lowest 3 bits, y is tile-row * 8 from  now on */
  offset = y;		/* y might be 8 or 16 bit, but we need 16 bit, so use a 16 bit variable */
  offset *= u8g2_GetU8x8(u8g2)->display_info->tile_width;
  ptr += offset;
  ptr += x;
  mask = 1;
  mask <<= bit_pos;
  if ( u8g2->draw_color != 0 )
  {
    *ptr |= mask;
  }
  else
  {
    mask ^= 255;
    *ptr &= mask;
  }  
}

/*
  x,y		Upper left position of the line
  len		length of the line in pixel, len must not be 0
  dir		0: horizontal line (left to right)
		1: vertical line (top to bottom)
*/
void u8g2_DrawHVLine(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir)
{
  if ( dir == 0 )
  {
    do
    {
      u8g2_draw_pixel(u8g2, x, y);
      x++;
      len--;
    } while( len != 0 );
  }
  else
  {
    do
    {
      u8g2_draw_pixel(u8g2, x, y);
      y++;
      len--;
    } while( len != 0 );
  }
}

static void u8g2_send_tile_row(u8g2_t *u8g2, uint8_t tile_row)
{
  uint8_t *ptr;
  uint16_t offset;
  uint8_t w;
  
  w = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  offset = tile_row;
  ptr = u8g2->tile_buf_ptr;
  offset *= w;
  offset *= 8;
  ptr += offset;
    
  u8x8_display_DrawTile(u8g2_GetU8x8(u8g2), 0, tile_row, w, ptr);
}

void u8g2_SendBuffer(u8g2_t *u8g2)
{
  uint8_t cnt = u8g2->tile_buf_height;
  uint8_t row = u8g2->tile_curr_row;
  do
  {
    u8g2_send_tile_row(u8g2, row);
    cnt--;
    row++;
  } while( cnt > 0 );
}




