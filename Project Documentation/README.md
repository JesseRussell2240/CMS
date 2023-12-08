# CMS
CMS coded message system for eng projects 3 project 2



________________________________________________________________________________________________________________
New Features/Change log/version control


CMS V0.0.1
The purpose of this code is to be able to record audio from a user 
and then give them an option to replay and or save the audio.
Further down the line the program will have a fully opperational
GUI giving users an option to chose the length, bit rate, destination
and encryption of the message. 

CMS V0.0.1
The basic use of the program so far consists of launching up and
recording 10s of audio, after recording the audio it asks the user if
they want to save it and if so where. Then gives them an option to replay
the saved audio


________________________________________________________________________________________________________________

Week 1
Jesse
Created project outline/schedule 
set up git
weekly stats reports
Trouble shoot for various IDE related issues

Hayden
Created test case document
comment provided main and reserched paramaters for various given functions

Zach
Commented provided files
researched windows audio paramaters etc
began legwork to have custom GUI setup

Goals for week 2:

Implement RS232 comports, prepare GUI for helper functions to be moved over into QT. Establish git hub to ensure we
can start editing the same repository. 

________________________________________________________________________________________________________________

Week 2
Jesse
modified audio recording into helper functions and text based GUI intorder to better ingrate with the GUI

Hayden
Modified status report template and filled out this weeks status report
Wrote code to use a buffer inorder to have a longer recive window that checks for garbage

Zach
rs232 set up comport virtual code calls

Goals for week 3

Begin implementation for XOR cypher for text. Move more helper functions into the GUI. Create Menu system to easily navigate through
the user options. 

________________________________________________________________________________________________________________
Week 3
Jesse
Implemented play and record into GUI
Merged RS232 code with audio recording, wrote a basic merged copy of menu systems, removed mains and called helper functions.

Hayden
Status report, and work breakdown strucutre. Improved menu system

Zach
Started creating helper functions for XOR encrytion to be implemented into transmission and reciver functions

Goals For week 4
________________________________________________________________________________________________________________
Week 4
Jesse
Added load and save file functionality 
Added menu options to change, save and load settings
Refractured transmission

Hayden
Added the capability to transmit custom messages as well as random messages from the FortuneCookie.txt file

Zach
Status report and added encryption files
________________________________________________________________________________________________________________
Week 5
Jesse
Added headers into file. Ensures header were senf=ding with payload

Hayden
Starting implementing hashing//phonebook

Zach
Added queues, and began editing voteon to be implemented for error correction of header.

Goals for week 6/7

Finished

________________________________________________________________________________________________________________
Week 6/7
Jesse 
Implimented encryption for text
implimented compression and decompresion for text
implimented compression for audio
Spent time testing and debugging
Begain working on presentation


Hayden
Implimented queues on recieve station
Implimented PhoneBook with queues
Finalized interface for dignostic functions
Begain working on presentation
commented underexplained code


Zach
Implimented error correction for header
implimented error detection for payload
refactored code
Begain working on presentation
begain recoding demo video for presentation

----------------------------------------------------------------------------------------------------------------


TO_DO
-intgrate rs232 communications into GUI
-transmit virtual audio
- transmit physical audio 
- finish non gui implementation of code. 
- figureout how to update the comm port pased on used provided input. (a switch case is a nasty solution to this