#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "sprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const Gamedata* gdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface *screen;
  SDL_Surface * const orbSurface;
  SDL_Surface * const bgSurface;
  Frame * const orbFrame;
  Frame * const bgFrame;
  Sprite bg;
  //Sprite bg1;
  std::vector<Sprite> orb;
  

  void drawBackground() const;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
