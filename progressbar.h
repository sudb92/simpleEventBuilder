/*
progressbar.h

Takes in the current iteration index and the full size of a loop, and prints a progressbar to the console.

B.Sudarsan, E.Good, K.Macon
sbalak2@lsu.edu
Fall2019-Spring2020
*/
#pragma once
#include <math.h>
#include <iostream>
static int temp = -1;
void progressbar(double currentiter, double fullsize)
{ 
 double seekpercent = ((double)currentiter/(double)fullsize)*100+10;
if(((int)floor(seekpercent))%10==0 && (int)floor(seekpercent)!= temp)
  {
	//std::cout << "\nProcess " << (int)seekpercent << "% complete..";
    std::cout<<"\r[";
	for(int i=0; i<100; i=i+10)
	{
		if(i<(int)(seekpercent)) std::cout << "=";
		else std::cout << " ";
	}
    std::cout << "]";
    std::cout << " Status: " << (int)seekpercent << " % complete.." << std::flush;

	temp = (int)floor(seekpercent);
  }

}
