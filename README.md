# COMP3004 Project - Team 46
### Group Members:

- Danielle Nkem 101218592
- Charles Yan 101193325
- Kaitlyn Warhurst 101217643
- Emtiaz Alam 101114889
- Jearina Tseung 101199241

TA: Vishal

Demo Video: https://youtu.be/5iT_4gfAPT0

Files:
- DBManager.cpp
- DBManager.h
- battery.h
- battery.cpp
- chrono.cpp
- chrono.h
- eegsite.h
- eegsite.cpp
- main.cpp
- mainwindow.cpp
- mainwindow.h
- menu.cpp
- menu.h
- qcustomplot.h
- qcustomplot.cpp
- session.cpp
- session.h

Diagrams & Documentations:
- StateDiagram.pdf
- UMLDiagram.pdf
- UseCases.pdf
- SequenceDiagram.pdf (contains diagrams for scenarios covering normal and safety operation)
- UMLClassDiagram.pdf

Assumptions/Notes:
- 7 eeg sites
- 4 rounds of treatment ((flat rate of the dominant frequency + 5) * the round)
- Shortened timings for testings:
    - When a new session creation is paused for 10 seconds the session is deleted and the user has to reset
    - When the session is being created, the battery drain rate is 1.0. (session is active)
    - Outside of the new session, the normal battery drain rate is 0.01. (idle)
 - The ui explanation is in the demo video
 
Assigned Tasks:
- Use Cases: Danielle, Charles, Kaitlyn, Emtiaz, Jearina
- UML Class Diagram: Danielle, Charles, Kaitlyn, Emtiaz, Jearina
- UML Sequence diagrams:
    - normal operation of treatment with Neureset device: Danielle
    - therapy history viewing with PC: Charles
    - battery low response of the device: Jearina
    - connection loss between electrodes and the device: Emtiaz
- UML State machine diagram: Kaitlyn
- Textual explanation: Danielle, Charles, Kaitlyn, Emtiaz, Jearina
- Traceablility Matrix: Danielle, Charles, Kaitlyn, Emtiaz, Jearina
- Code: Danielle, Charles, Kaitlyn, Emtiaz, Jearina

References:
- Team3-dena_prototype
- Heartwave

GitHub Link:
- https://github.com/EmtiazAlam/team-collaboration.git 

Instructions for Compiling

Needed external libraries and how to install/compile them:
fftw3: Download from: https://www.fftw.org/download.html 
full installation instructions: https://www.fftw.org/fftw3_doc/Installation-on-Unix.html

- Summary of how to install: 
    1. Download the .tar.gz
    2. Extract files using "tar -xvf"
    3. Run the following commands within the folder (the make and make install may need to be run with sudo - 'sudo make'):
        ./configure
        make
        make install
