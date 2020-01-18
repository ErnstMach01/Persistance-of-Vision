/*
*
* Name: Ernst Mach
* Project: Persistance of vision application
* Date: February 5, 2018
* Description: When program starts plays a song for the user and displays ascii art to the console. Once
* the song is completed a light will begin flashing at 1 hertz. The user will have access to 3 buttons,
* one button will increase the speed of the blinking increasing by 1 hertz per push, a another button will
* decrease the speed of the blinking by 1 hertz per push, and a final button will
* display the current hertz to the console.
*
*/

#include <unistd.h>
#include <stdio.h>	
#include </usr/include/sys/io.h> //IO library
#define BASEPORT 0x378 /* parallel or lp1 port address */

float HertzIncrement = 1;	//How much to increase the hertz by each time
float Hertz = 1;		//Starting hertz value
float DisplaySpeed = 0;	//Variable for the speed at which the lights flash on and off
int ImportValue =0;	//Variable to store what input values are coming from the import bases
int lock= 0;	//Variable used as a lock method to control that when the button is pressed it will only increase by 1

//Function used to turn hertz into microseconds delay
float DisplayCalc(float Hertz) 
{
	float Display;
	Display = ((1000000/Hertz)/2);
	return Display;
}

//Function to play a note, takes 3 parameters, the hertz of the note, how long the note is to be played, and the delay before the next note is played
int PlayNote(int NoteFrequency, float duration, int delay) 
{
	int HertzInterval = (1000000/NoteFrequency)/2;	//used to calculate how long to wait for each on and off time in microseconds
		for(float i=0; i<(NoteFrequency*duration); i++)
		{
//For loop that takes the note frequency and multiplies it by the duration. Each time the loop completes it increments
			outb(0x2,BASEPORT);	//Sends an on signal to the second port
			usleep(HertzInterval);	//Wait a given amount of time
			outb(0x0,BASEPORT);	//Sends an off signal to the second port
			usleep(HertzInterval);	//Wait a given amount of time
		}

	usleep(delay);	//delay before next note is played
}

//Function to play a song along with accompanying ascii art
void StartTheme() 
{
	//Ascii art of a sheep
	printf("           __  _\n");
	printf("       .-.'  `; `-._  __  _\n");
	printf("      (_,         .-:'  `; `-._\n");
	printf("    ,'o(        (_,           )\n");
	printf("   (__,-'      ,'o(            )>\n");
	printf("      (       (__,-'            )\n");
	printf("       `-'._.--._(             )\n");
	printf("          |||  |||`-'._.--._.-'\n");
	printf("                     |||  |||\n");
	
	//Plays the song Marry had a Little Lamb
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(262, 0.50, 200000);//plays the note C
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.80, 200000);//plays the note E
	PlayNote(294, 0.50, 200000);//Plays the note D
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(294, 0.80, 200000);//plays the note D
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.80, 200000);//plays the note E
	PlayNote(330, 0.50, 200000);//plays the note E 
	PlayNote(294, 0.50, 200000);//plays the note D 
	PlayNote(262, 0.50, 200000);//plays the note C
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(330, 0.50, 200000);//plays the note E
	PlayNote(294, 0.50, 200000);//plays the note D
	PlayNote(262, 1.00, 200000);//plays the note C
}

int main()
{
	ioperm(BASEPORT, 3, 1); // Get permission to access the ports
	StartTheme();	//Play the StartTheme function at the start

	while(1)
	{
		ImportValue = inb(BASEPORT+1);	//Constantly check for input from the baseport

		//Opens up the lock when the button is not pressed
		if(ImportValue == 0x78) 
		{
			lock = 0;
		}
 
		// Used to speed up display, makes sure that the hertz cannot be lowered past 1 hertz
		if( ImportValue == 0x68 && Hertz > 1 && lock == 0) 	//Checks if a button is pressed
		{
			Hertz = Hertz - HertzIncrement;	//Decreases the hertz
			lock = 1;	//Locks the button so that it will not continue
		}

		// Used to slowdown display
		if(ImportValue == 0x58 && lock == 0) 	//Checks if a button is pressed
		{	
			Hertz = Hertz + HertzIncrement;	//Increases the hertz
			lock = 1;	//Locks the button so that it will not continue
		}

		// Used to display Hz to terminal
		if( ImportValue == 0x70 && lock ==0) 	//Checks if a button is pressed
		{	
			printf("The LED is flashing at %f Hertz\n", Hertz);//Prints the hertz to the screen
			lock = 1; 	//Locks the button so that it will not continue
		}

		DisplaySpeed = DisplayCalc(Hertz);//Calls function and puts the value as DisplaySpeed.
		outb(0x1, BASEPORT); // Sends a single to turn on the first light
		usleep(DisplaySpeed); // Wait a given amount of time
		outb(0x0, BASEPORT); // Sends a single to turn off the light
		usleep(DisplaySpeed); // Wait a given amount of time
	}

	ioperm(BASEPORT, 3, 0); // We don't need the ports anymore
	// release ports for other programs however as loop is infinite we will never get here

	return 0;
}
