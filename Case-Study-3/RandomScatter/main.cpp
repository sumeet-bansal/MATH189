#include <iostream>
#include "RandomScatter.hpp"

int main()
{
  //Random sequence variables
  int SEQLENGTH = 229354;
  int PALMIN = 10;
  int PALMAX = 20;
  int TRIALS = 100;
  bool PRINT = false;

  RandomScatter::randomSequence( SEQLENGTH, PALMIN, PALMAX, TRIALS, PRINT );

  //Random scatter variables
  SEQLENGTH = 229354;
  int NUMPAL = 296;
  int LENGTHPAL = 10;
  int NUMFRAMES = 50;
  TRIALS = 100;
  PRINT = false;

  RandomScatter::randomScatter( SEQLENGTH, NUMPAL, LENGTHPAL, NUMFRAMES, TRIALS, PRINT);
  
  return 0;
}
