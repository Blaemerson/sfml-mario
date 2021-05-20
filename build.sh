#!/bin/bash

g++ -c src/*.cpp

mv *.o build/

g++ build/*.o -lsfml-system -lsfml-window -lsfml-graphics -o ./bin/game.app

./bin/game.app
