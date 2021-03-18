# INSTALL

1. Install SFML: simply follow the official SFML tutorials. They are very detailed.

   1. Visual Studio:

      * https://www.sfml-dev.org/tutorials/2.5/start-vc.php

      * Note 1: If you run VS 2019, downloading the latest available version of the SFML SDK will do the trick (currently from 2017). It is recommended to install the 32-bit version of SFML.
      * Note 2: The "Tron: Lightcycles" project was completely made in VS 2019 on Windows 10 and I followed the second tutorial.

   2. Linux:

      * https://www.sfml-dev.org/tutorials/2.5/start-linux.php
      * Note: Some of the commands from this tutorial may differ according to the Linux version you are currently using.

2. Put the files from this repository to your project (to the location of the main.cpp from the previous step/ or to different project).

   * This repository contains the most important SFML files which are needed to compile and run the game.

3. Build and run the project

   * This is done in the same way as described in the last part of the SFML tutorials from step 1

   * Note 1: The first loading of all of the SFML objects and libraries may take very long time (1 to 5 minutes)
   * Note 2: If you are using VS on Windows, then you can find the .exe game file in the Debug directory of the Tron VS project. You can just copy/move the .exe file to the folder one level above (where the main.cpp is located). Then the game can be run directly from that .exe file.

