#include "ncurses_display.h"
#include "system.h"

int main() {
  System system; //initial point of entry for the program
  NCursesDisplay::Display(system);
}