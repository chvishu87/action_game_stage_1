#include <cmath>
#include "manager.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  SDL_FreeSurface(orbSurface);
  delete orbFrame;
  SDL_FreeSurface(bgSurface);
  delete bgFrame;
  delete Gamedata::getInstance();
  
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  gdata( Gamedata::getInstance() ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),			// Is deleted automatically. Need not use SDL_SurfaceFree
  orbSurface( io.loadAndSet(gdata->getXmlStr("redorbFile"), true) ),
  bgSurface(io.loadAndSet(gdata->getXmlStr("backgroundFile"), true) ),
  orbFrame(new Frame(orbSurface,
                gdata->getXmlInt("redorbWidth"), 
                gdata->getXmlInt("redorbHeight"), 0, 0)),

  bgFrame(new Frame(bgSurface,
		gdata->getXmlInt("backgroundWidth"),
		gdata->getXmlInt("backgroundHeight"), 0, 0)),
  bg(Vector2f(gdata->getXmlInt("backgroundSrcX"),
	      gdata->getXmlInt("backgroundSrcY")),
     Vector2f(gdata->getXmlInt("backgroundXspeed"),
	      gdata->getXmlInt("backgroundYspeed")),
     "back",bgFrame),
  /*bg1(Vector2f(gdata->getXmlInt("backgroundSrcX"),
	      gdata->getXmlInt("backgroundSrcY")833,0),
     Vector2f(gdata->getXmlInt("backgroundXspeed"),
	      gdata->getXmlInt("backgroundYspeed")),
     "back",bgFrame),*/
  orb()

{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  orb.reserve(gdata->getXmlInt("orbCount"));			// Reserving memory in the vector for the sprites that are created.
  for(int i=0; i<gdata->getXmlInt("orbCount"); i++) {
    orb.push_back(Sprite(Vector2f(rand()%gdata->getXmlInt("worldWidth"),	// Inserting multiple sprite objects into vector with initial starting position at random place in the world.
               rand()%gdata->getXmlInt("worldHeight")), 
      Vector2f(gdata->getXmlInt("redorbXspeed"),
               gdata->getXmlInt("redorbYspeed")),
      "redorb", orbFrame));
  }
  atexit(SDL_Quit);
}

void Manager::drawBackground() const {			// Not being used.
  SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, gdata->getXmlInt("backgroundRed"), 
  							 gdata->getXmlInt("backgroundGreen"), 
							 gdata->getXmlInt("backgroundBlue")) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void Manager::play() {
  SDL_Event event;
  std::vector<Sprite>::iterator itr;		// Iterator for parsing the sprites in the vector to call the draw() and update() functions of the sprite.
  bool done = false;
  bool keyCatch = false;
  bool backgrnd = gdata->getXmlBool("backgroundOn");
  while ( not done ) {
    if(backgrnd)	// If the condition is true background will display the color metioned in the game.xml else it will display the default background image.
    	{ drawBackground(); }
    else
    	{// bg1.draw(); 
		  bg.draw();
		}
    Uint32 ticks = clock.getElapsedTicks();
    for(itr=orb.begin(); itr < orb.end(); itr++) {
      itr->draw(); 
      itr->update(ticks);
    }
    
    SDL_Flip(screen);

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYUP) { keyCatch = false; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE : done = true; break;
        case SDLK_q      : done = true; break;
        case SDLK_p      : {
          if (!keyCatch) {
            keyCatch = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
          }
          break;
        }
        default          : break;
      }
    }
  }
}
