#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>

#define screen_w 800
#define screen_h 600

#define color_step 15
#define box_step 10

struct obj{
  char x;
  char y;
  SDL_Rect dst;
  
  struct obj *next;
};

void new_lab(struct obj *lab, SDL_Surface *lab_img);
void rm_lab(struct obj *lab);
void rm_all_lab(struct obj *lab);

void mv_labs(struct obj *lab, SDL_Surface *lab_img);
void draw_labs(struct obj *lab, SDL_Surface *lab_img, SDL_Surface *screen);
void clear_labs(struct obj *lab, Uint32 color, SDL_Surface *screen);

unsigned short count_labs(struct obj *lab);
void produce_labs(struct obj *lab, SDL_Surface *lab_img, unsigned short units);
void reduce_labs(struct obj *lab, unsigned short units);

int main()
{
  //variables
  SDL_Surface *screen;
  SDL_Surface *image;
  SDL_Surface *original_img;
  SDL_Surface *lab_img;
  
  struct obj lab;
  lab.x = box_step;
  lab.y = box_step;
  lab.dst.x = 0;
  lab.dst.y = 0;
  lab.next = NULL;
  
  SDL_Event user_input;
  
  unsigned char red = 255;
  unsigned char green = 0;
  unsigned char blue = 0;
  
  char info = -1;
  char bckg = 1;
  
  double zoom = 0.5;
  
  //frame_rate variables
  Uint32 frame_timestop = SDL_GetTicks();
  
  //init SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(-1);
  if((screen = SDL_SetVideoMode(screen_w, screen_h, 24, SDL_HWSURFACE)) == NULL)
    exit(-1);
  if((image = IMG_Load("lava.png")) == NULL)
    exit(-1);
  if((original_img = IMG_Load("lab.png")) == NULL)
    exit(-1);
  lab_img = zoomSurface(original_img, zoom, zoom, 0);
  SDL_EnableKeyRepeat(200, 50);
  SDL_ShowCursor(0);
  atexit(SDL_Quit);
  
  //init ttf stuff
  TTF_Init();
  SDL_Color color = {200, 200, 200};
  TTF_Font *font = TTF_OpenFont("mono.ttf", 30);
  SDL_Surface *text = NULL;
  char cache_string[128];
  
  SDL_Rect text_dest;
  text_dest.x = 0;
  text_dest.y = screen_h - 30;
  text_dest.w = screen_w/2;
  text_dest.h = 30;
  
  //drawing loop
  while(1)
  {
    //handle user input
    while(SDL_PollEvent(&user_input))
    {
      if(user_input.type == SDL_KEYDOWN)
      {
        if(user_input.key.keysym.sym == SDLK_PLUS)
          new_lab(&lab, lab_img);
        else if(user_input.key.keysym.sym == SDLK_MINUS)
          rm_lab(&lab);
        
        if(user_input.key.keysym.sym == SDLK_f)
          produce_labs(&lab, lab_img, 50);
        else if(user_input.key.keysym.sym == SDLK_d)
          reduce_labs(&lab, 50);
        else if(user_input.key.keysym.sym == SDLK_r)
        {
          SDL_FillRect(screen, NULL, 0);
          rm_all_lab(&lab);
        }
        
        if(user_input.key.keysym.sym == SDLK_z && zoom < 1)
        {
          zoom += 0.1;
          lab_img = zoomSurface(original_img, zoom, zoom, 0);
        }
        else if(user_input.key.keysym.sym == SDLK_u && zoom > 0.2)
        {
          zoom -= 0.1;
          lab_img = zoomSurface(original_img, zoom, zoom, 0);
        }
        else if(user_input.key.keysym.sym == SDLK_t)
        {
          zoom = 0.5;
          lab_img = zoomSurface(original_img, zoom, zoom, 0);
        }
        
        if(user_input.key.keysym.sym == SDLK_i)
        {
          SDL_FillRect(screen, &text_dest, 0);
          info *= -1;
        }
        
        if(user_input.key.keysym.sym == SDLK_b)
        {
          SDL_FillRect(screen, NULL, 0);
          bckg *= -1;
        }
      }
    }
    if(user_input.key.keysym.sym == SDLK_ESCAPE)
      break;
    
    //draw background
    if(bckg == 1)
    {
      if(red < 255 && green == 0)
      {
        red += color_step;
      }
      else if(blue > 0 && red == 255)
      {
        blue -=  color_step;
      }
      
      if(green < 255 && blue == 0)
        green += color_step;
      else if(red > 0 && green == 255)
        red -= color_step;
      
      if(blue < 255 && red == 0)
        blue += color_step;
      else if(green > 0 && blue == 255)
        green -= color_step;
      
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, red, green, blue));
      SDL_BlitSurface(image, NULL, screen, NULL);
    }
    else
    {
      clear_labs(&lab, 0, screen);
      if(info == 1)
      {
        SDL_FillRect(screen, &text_dest, 0);
      }
    }
    
    mv_labs(&lab, lab_img);
    draw_labs(&lab, lab_img, screen);
    
    if(info == 1)
    {
      sprintf(cache_string, "mspf: %03i, objects: %i", SDL_GetTicks() - frame_timestop, count_labs(&lab));
      text = TTF_RenderText_Solid(font, cache_string, color);
      SDL_BlitSurface(text, NULL, screen, &text_dest);
    }
    
    //flip screen
    SDL_Flip(screen);
    frame_timestop = SDL_GetTicks();
  }
  
  rm_all_lab(&lab);
  SDL_FreeSurface(image);
  SDL_FreeSurface(lab_img);
  SDL_FreeSurface(original_img);
  SDL_FreeSurface(screen);
  return 0;
}

//additional functions
void new_lab(struct obj *lab, SDL_Surface *lab_img)
{
  while(lab->next != NULL)
  {
    lab = lab->next;
  }
  if((lab->next = malloc(sizeof(struct obj))) != NULL)
  {
    if(rand() % 2)
      lab->next->x = box_step;
    else
      lab->next->x = -1 * box_step;
    
    if(rand() % 2)
      lab->next->y = box_step;
    else
      lab->next->y = -1 * box_step;
    
    lab->next->dst.x = rand() % (screen_w - lab_img->w);
    lab->next->dst.y = rand() % (screen_h - lab_img->h);
    lab->next->dst.w = lab_img->w;
    lab->next->dst.h = lab_img->h;
    
    lab->next->next = NULL;
  }
}
void rm_lab(struct obj *lab)
{
  if(lab->next != NULL)
  {
    while(lab->next->next != NULL)
    {
      lab = lab->next;
    }
    free(lab->next);
    lab->next = NULL;
  }
}
void rm_all_lab(struct obj *lab)
{
  while(lab->next != NULL)
  {
    rm_lab(lab);
  }
}

void mv_labs(struct obj *lab, SDL_Surface *lab_img)
{
  while(lab != NULL)
  {
    if(lab->dst.x <= 0)
      lab->x = box_step;
    else if(lab->dst.x + lab_img->w >= screen_w)
      lab->x = -box_step;
    
    if(lab->dst.y <= 0)
      lab->y = box_step;
    else if(lab->dst.y + lab_img->h >= screen_h)
      lab->y = -box_step;
    
    lab->dst.x += lab->x;
    lab->dst.y += lab->y;
    lab = lab->next;
  }
}
void draw_labs(struct obj *lab, SDL_Surface *lab_img, SDL_Surface *screen)
{
  while(lab != NULL)
  {
    SDL_BlitSurface(lab_img, NULL, screen, &lab->dst);
    lab = lab->next;
  }
}
void clear_labs(struct obj *lab, Uint32 color, SDL_Surface *screen)
{
  while(lab != NULL)
  {
    SDL_FillRect(screen, &lab->dst, 0);
    lab = lab->next;
  }
}

unsigned short count_labs(struct obj *lab)
{
  unsigned short counter = 0;
  while(lab != NULL)
  {
    counter++;
    lab = lab->next;
  }
  return counter;
}
void produce_labs(struct obj *lab, SDL_Surface *lab_img, unsigned short units)
{
  unsigned short counter = 0;
  while(counter < units)
  {
    new_lab(lab, lab_img);
    counter++;
  }
}
void reduce_labs(struct obj *lab, unsigned short units)
{
  unsigned short counter = 0;
  while(counter < units)
  {
    rm_lab(lab);
    counter++;
  }
}