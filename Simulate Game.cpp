#include "sprites.cpp"
#include "events.cpp"
#include "PokeCenter.cpp"
#include <cstdio>
#define isDown(b) input->buttons[b].isDown
#define isPressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)
internal void
simulateGame(Input* input)
