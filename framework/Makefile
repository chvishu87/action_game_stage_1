
ifeq ("$(shell whoami)", "malloy")
  CXX = clang++
else
  CXX = g++
endif
CXX_LINK = g++

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lexpat

OBJS = \
	main.o \
	manager.o \
	gamedata.o \
	parseXML.o \
	clock.o \
	vector2f.o \
	sprite.o \
	frame.o \
	ioManager.o

EXEC = run

# $< refers to the first dependency
%.o: %.cpp
	$(CXX_LINK) $(CXXFLAGS) -c $< -o $@

# $@ refers to the target
$(EXEC): $(OBJS)
	g++ $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.cpp
manager.o: manager.cpp manager.h gamedata.h ioManager.h sprite.h clock.h
ioManager.o: ioManager.cpp ioManager.h
parseXML.o: parseXML.cpp parseXML.h
gamedata.o: gamedata.cpp gamedata.h parseXML.h
spriteFactory.o: spriteFactory.cpp spriteFactory.h factory.h frame.h
clock.o: clock.cpp clock.h
vector2f.o: vector2f.cpp vector2f.h
frame.o: frame.cpp frame.h
sprite.o: sprite.cpp sprite.h drawable.h frame.h
frame.o: frame.cpp frame.h ioManager.h

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
