simpleEventBuilder
******************

* A simple eventbuilder ROOT macro set for data analysis using CAEN's digitizers.
* Three files:
	- progressbar.h : a standalone analysis progress indicator
	- simpleEventBuilder.C : the main ROOT macro
	- makePlots.C : auxiliary, bare-bones ROOT plotter macro 
	
* To test-run, try on a *nix terminal with ROOT (v6.22 or higher) installed:
	- root -l
	- .x simpleEventBuilder.C(1)  //Generates run_1_builtFile.root.  Using .x simpleEventBuilder.C++(1) will compile the macro, as required
	- .x makePlots(1) //Will print a coincident data plot

Author:
Sudarsan B

E-mail:
sbalak2@lsu.edu
bsudarsan92@gmail.com

