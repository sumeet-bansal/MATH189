#ifndef RANDOM_SCATTER_HPP
#define RANDOM_SCATTER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <tgmath.h>

using namespace std;

namespace RandomScatter {

  const int SEED = 13355037;
  const int MOD = 4;

  /*
   * Find the mean of the vector
   */
  double mean( vector<int> vec )
  {
    double total = 0; 
    for (int i = 0; i < vec.size(); i++)
    {
      total += vec[i]; 
    }
    return total / (double) vec.size();
  }

  /*
   * Find the standard deviation of a vector
   */
  double stdev( vector<int> vec, double mean )
  {
    double var = 0;
    double size = (double) vec.size();

    for (int i = 0; i < size; i++)
    {
      var += (vec[i] - mean) * (vec[i] - mean) / size;
    }

    return sqrt( var );
  }

   
  /*
   * Generate a random DNA strand of a certain length, and find palindromes of
   * a range of lengths
   *
   * seqLength: length of the DNA sequence to be generated
   * palMin: minimum length of palindromes to be searched for (inclusive)
   * palMax: maximum length of palindromes to be searched for (inclusive)
   * trials: number of times the experiment should be repeated
   * print: true if stats for every trial should be printed, false if only overall
   *        numbers should be printed
   */
  void randomSequence( int seqLength, int palMin, int palMax, int trials, bool print )
  {
    cout << endl << "Testing random DNA sequences for palindrome frequencies: " << endl;

    srand( SEED );

    //Make sure max and min are even
    int min = palMin;
    int max = palMax;

    if (palMin % 2 == 1)
    {
      min--;
    }
    if (palMax % 2 == 1)
    {
      max++;
    }

    //Set up array for overarching stats
    vector<double> stats(max - min + 2);
    stats[0] = 1;
    stats[1] = 1;
    stats[2] = 1;
    stats[3] = 1;
    stats[4] = 1;
    stats[5] = 1;


    //Repeat experiment a number of times
    for (int i = 0; i < (int) trials; i++)
    {

      //Generate pseudorandom DNA sequence
      int sequence[seqLength];

      int random;
      for (int j = 0; j < seqLength; j++)
      {
        sequence[j] = rand() % MOD;
      }


      //Find palindromes of different lengths
      double total = 0;
      int index = 0;

      for (int palLength = min; palLength <= max; palLength += 2)
      {
        int count = 0;

        //Create sliding frame for finding palindromes
        for (int k = 0; k < seqLength - palLength; k++)
        {
          //Find palindromes within this frame
          int lo = k;
          int hi = lo + palLength - 1;

          bool palindrome = true;
          while (hi > lo && palindrome)
          {
            if ( sequence[lo] == sequence[hi] || (sequence[lo] + sequence[hi]) % 2 == 1 )
            {
              palindrome = false;
            }
            
            lo++;
            hi--;
          }

          if (palindrome)
          {
            count++;
          }
        }

        if (print)
        {
          cout << "Number of length " << palLength << " palindromes: " << count << endl;
        }

        total += count;
        stats[index] += count;
        index++;
      }
      
      if (print)
      {
        cout << "Total number of palindromes: " << total << endl << endl;
      }

      stats[index] += total;
    }

    //Print overall stats
    int statsIndex = 0;
    for (int palLength = min; palLength <= max; palLength += 2)
    {
      cout << "Average length " << palLength << " palindromes: " 
           << stats[statsIndex] / trials << endl;
      statsIndex++;
    }

    cout << "Average number of palindromes: " << stats[statsIndex] / trials << endl;
  }

  /*
   * Generate a DNA strand with a number of palindromes scattered in
   * 
   * seqLength: length of DNA sequence
   * numPal: number of palindromes to scatter in the sequence
   * lengthPal: length of palindromes to scatter in the sequence
   * numFrames: number of frames in which to count palindromes
   * trials: number of times to repeat the experiment
   * print: prints results for each trial if true, prints overall results if false
   */
  void randomScatter( int seqLength, int numPal, int lengthPal, int numFrames, 
                      int trials, bool print)
  {
    cout << endl << "Testing random scattering of palindromes for distance: " << endl;
    srand( SEED );

    int failures = 0;
    int random;

    vector<int> allDistVec;
    vector<int> allNumVec;

    //Repeat the experiment a number of times
    for (int i = 0; i < trials; i++)
    {
      //Make vector of appropriate length
      vector<int> sequence( seqLength, 0 );

      //Scatter appropriate number of palindromes into sequence
      for (int j = 0; j < numPal; j++)
      {
        sequence[ rand() % seqLength ] = 1;
      }

      //Check whether palindromes overlap. If they do, throw out this experiment
      bool overlap = false;
      for (int k = 0; k < seqLength; k++)
      {
        if (overlap)
        {
          break;
        }

        if (sequence[k] == 1)
        {
          int l = 0;
          for (l = k + 1; l < lengthPal + k; l++)
          {
            if (l >= seqLength || sequence[l]++ == 1)
            {
              //Palindromes overlap or run off the end of the sequence
              failures++;
              i--;
              overlap = true;
              break;
            }
          }
          k = l;
        }
      }

      if (overlap)
      {
        continue;
      }

      //Continue if sequence is valid
/*
      //Find distances between palindromes
      vector<int> distVect;
      int distCount = 0;
      bool endGap = false;
      for (int m = 0; m < seqLength; m++)
      {
        while (sequence[m] == 0)
        {
          distCount++;
          m++;
          endGap = true;
        }

        if (endGap)
        {
          distVect.push_back( distCount );
          distCount = 0;
          endGap = false;
        }
      }

      //Print stats
      if (print)
      {
        cout << endl << "Distances: ";
        //Don't print first and last entry, since those aren't distances b/w pals
        for (int n = 1; n < distVect.size() - 1; n++)
        {
          cout << distVect[n] << ",";
        }
        cout << endl << endl;
      }

      //Feed data into allDistVec
      for (int q = 1; q < distVect.size() - 1; q++)
      {
        allDistVec.push_back( distVect[q] );
      }

      //Find number of palindromes per frame
      int frameLength = seqLength / numFrames;
      vector<int> palVect;

      //Check every frame except the last one
      for (int o = 0; o < numFrames - 1; o++)
      {
        int palCount = 0;
        for (int p = o * frameLength; p < (o + 1) * frameLength - lengthPal + 1; p++)
        {
          if (sequence[p] == 1)
          {
            palCount++;
            while (sequence[p] == 1)
            {
              p++;
            }
          }
        }
        palVect.push_back( palCount );
      }

      //Feed data into allNumVec
      for (int r = 0; r < palVect.size(); r++)
      {
        allNumVec.push_back( palVect[r] );
      }

      //Print stats, if appropriate
      if (print)
      {
        cout << "Palindromes per frame of size " << frameLength << ": ";
        for (int s = 0; s < palVect.size(); s++)
        {
          cout << palVect[s] << ",";
        }
        cout << endl << endl;
      }
    }*/


    //Find number of palindromes in each rolling frame
    int FR = 200;
    for (int frameSize = FR; frameSize <= FR; frameSize+=1000)
    {
      cout << "Frame size: " << frameSize << endl;
      for (int i = 0; i < seqLength - frameSize; i+= 100)
      {
        int palCount = 0;
        for (int j = i; j < i + frameSize; j++)
        {
          if (sequence[j] == 1)
          {
            palCount++;
            
            while (sequence[j] == 1)
            {
              j++;
            }
          }
        }
        cout << palCount << ",";
      }
    }
cout << endl << endl;
    for (int frameSize = FR; frameSize <= FR; frameSize+=1000)
    {
      for (int i = 0; i < seqLength - frameSize; i+= 100)
      {
        cout << i << ",";
      }
    }
}
    //Print aggregate stats
    double distAve = mean( allDistVec );
    double distDev = stdev( allDistVec, distAve );
    double numAve = mean( allNumVec );
    double numDev = stdev( allNumVec, numAve );
    cout << endl;
    cout << "Average distance between palindromes: " << distAve << endl;
    cout << "St.dev. of distance between palindromes: " << distDev << endl;
    cout << "Average palindromes per frame of size " << seqLength / numFrames
         << ": " << numAve << endl;
    cout << "St.dev. of palindromes per frame: " << numDev << endl;
    cout << endl;
  }

}

#endif //RANDOM_SCATTER_HPP
