// BASS demo test
// A very basic OpenGL/GLUT application
// Create a window, load some sounds and play them.
// A test program for BASS - available from http://www.un4seen.com/
// This program uses two sound effects - available from http://www.freesound.org/
// To test this program you will need two wav sound effect files!
#include "bass.h"
#include <iostream>
#include <unistd.h>
//#include <GL/glut.h>

// g++ main4.cpp -o test5 -lbass

using std::cout;
using std::endl;

HSAMPLE *samples=NULL;

/*
void printString(void *font, char* str)
{
	while (*str != NULL)
		glutBitmapCharacter(font, *str++);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT); // Just clear the screen
  glColor3f (1.0, 1.0, 1.0); // set colour ( R, G, B )
  glRasterPos2f(-0.95,0.8);
  printString(GLUT_BITMAP_HELVETICA_18, "Press '1' and/or '2' to play sounds");
  glRasterPos2f(-0.95,0.5);
  printString(GLUT_BITMAP_HELVETICA_18, "'p' to pause and 'r' to resume");

  glFlush();  // Redraw (single buffered)
}
*/
HSAMPLE loadSample(char * filename)
{
	HSAMPLE sam;
	if (sam=BASS_SampleLoad(FALSE,filename,0,0,3,BASS_SAMPLE_OVER_POS))
		cout << "sample " << filename << " loaded!" << endl;
	else
	{
		cout << "Can't load sample1";
		exit (0);
	}
	return sam;
}

HSAMPLE loadSampleRepeat(char * filename)
{
	HSAMPLE sam;
	if (sam=BASS_SampleLoad(FALSE,filename,0,0,3,BASS_SAMPLE_OVER_POS|BASS_SAMPLE_LOOP))
		cout << "sample " << filename << " loaded!" << endl;
	else
	{
		cout << "Can't load sample2";
		exit (0);
	}
	return sam;
}

void init(void) 
{
   //glClearColor(0.0, 0.0, 0.0, 0.0); // set background colour

	/* Initialize default output device */
	if (!BASS_Init(-1,44100,0,0,NULL))
		cout << "Can't initialize device";

	samples = new HSAMPLE[12];
	// Following comment is from source basstest file!
	/* Load a sample from "file" and give it a max of 3 simultaneous
	playings using playback position as override decider */
	//samples[0] = loadSample( "German_Concert_D_051_083.wav");
	//samples[1] = loadSample( "German_Concert_D_052_083.wav");
	//samples[2] = loadSample( "German_Concert_D_053_083.wav");
	//samples[3] = loadSample( "German_Concert_D_054_083.wav");
	//samples[4] = loadSample( "German_Concert_D_055_083.wav");
	//samples[5] = loadSample( "German_Concert_D_056_083.wav");
	//samples[6] = loadSample( "German_Concert_D_057_083.wav");
	//samples[7] = loadSample( "German_Concert_D_058_083.wav");
	//samples[8] = loadSample( "German_Concert_D_059_083.wav");
	//samples[9] = loadSample( "German_Concert_D_060_083.wav");
	//samples[10] = loadSample( "German_Concert_D_060_083_Start.wav");
	samples[10] = loadSample( "German_Concert_D_060_083_Start7.wav");
	
	samples[11] = loadSampleRepeat( "German_Concert_D_060_083_Repeat3.wav");
	
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		// Free up the BASS resources
		BASS_Free();
		exit(0);
	case '1':
		{	// These curly braces are strictly not required but
			// Visual Studio .NET 2003 complains if they are not present
			HCHANNEL ch=BASS_SampleGetChannel(samples[0],FALSE);
			BASS_ChannelSetAttribute(ch,BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch,BASS_ATTRIB_VOL,1);
			if (!BASS_ChannelPlay(ch,FALSE))
				cout << "Can't play sample" << endl;
		}
		break;
	case '2':
		{	// See comment above!
			HCHANNEL ch=BASS_SampleGetChannel(samples[1],FALSE);
			BASS_ChannelSetAttribute(ch,BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch,BASS_ATTRIB_VOL,1);
			if (!BASS_ChannelPlay(ch,FALSE))
				cout << "Can't play sample" << endl;
		}
		break;
	case 'p':
		// Pause output
		BASS_Pause();
		break;
	case 'r':
		// Resume playing
		BASS_Start();
		break;
	}
}


int main(int argc, char **argv)
{
  //glutInit(&argc, argv);
  //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  //glutInitWindowSize(320, 160);
  //glutInitWindowPosition(150, 150);
  //glutCreateWindow("Basic GL/GLUT App");
  init();

  /*
  if (BASS_GetVersion()!=MAKELONG(2,2)) {
	MessageBox(0,"BASS version 2.2 was not loaded","Incorrect BASS.DLL",0);
	return 0;
  }
	*/
	HCHANNEL ch[12];
  //glutKeyboardFunc(keyboard);
  //glutDisplayFunc(display);
  //glutMainLoop();	// loop while waiting for messages
  ch[0]=BASS_SampleGetChannel(samples[0],FALSE);
			BASS_ChannelSetAttribute(ch[0],BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch[0],BASS_ATTRIB_VOL,1);
			
	
  ch[1]=BASS_SampleGetChannel(samples[2],FALSE);
			BASS_ChannelSetAttribute(ch[1],BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch[1],BASS_ATTRIB_VOL,1);
			
ch[2]=BASS_SampleGetChannel(samples[1],FALSE);
			BASS_ChannelSetAttribute(ch[2],BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch[2],BASS_ATTRIB_VOL,1);
			//if (!BASS_ChannelPlay(ch[2],FALSE))
			//	cout << "Can't play sample" << endl;
			
			
ch[10]=BASS_SampleGetChannel(samples[10],FALSE);
			BASS_ChannelSetAttribute(ch[10],BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch[10],BASS_ATTRIB_VOL,1);
			
ch[11]=BASS_SampleGetChannel(samples[11],FALSE);
			BASS_ChannelSetAttribute(ch[11],BASS_ATTRIB_PAN,0);
			BASS_ChannelSetAttribute(ch[11],BASS_ATTRIB_VOL,0);
			
			HSTREAM streamHandle = BASS_StreamCreateFile(FALSE, "Take_a_bow.mp3", 0, 0, 0);
			//BASS_ChannelPlay(streamHandle, FALSE);

	double length = BASS_ChannelBytes2Seconds(ch[10], BASS_ChannelGetLength(ch[10], BASS_POS_BYTE));
	bool tail = false;
	
	float myVolume = 0;
	float startVolume = 1;
	
int count = 0;
  while(1){
	  //usleep(1);
	  
	  if( count == 100000){
		  if (!BASS_ChannelPlay(ch[10],FALSE))
				cout << "Can't play sample" << endl;
			if (!BASS_ChannelPlay(ch[11],FALSE))
				cout << "Can't play sample" << endl;
	  }
	  
	  if(count > 100000){
		  
		//bool isRunning = BASS_ChannelIsActive(ch[10]) == BASS_ACTIVE_PLAYING;
		//
		//if(!isRunning){
		//	if (!BASS_ChannelPlay(ch[11],FALSE))
		//		cout << "Can't play sample" << endl;
		//}
		
		QWORD bytePos = BASS_ChannelGetPosition(ch[10], BASS_POS_BYTE);
		double secPos = BASS_ChannelBytes2Seconds(ch[10], bytePos);
		
		//cout << length - secPos << endl;
		float vol = -1;
		BASS_ChannelGetAttribute(ch[11],BASS_ATTRIB_VOL, &vol);
		
		if(count > 200000){
			
			BASS_ChannelStop(ch[10]);
			BASS_SampleFree(samples[10]);
			
			cout << "Stop" << endl;
		}
		
		if(secPos > 5 && !tail){
			
			BASS_ChannelSetAttribute(ch[10],BASS_ATTRIB_VOL, 0);
			BASS_ChannelSetAttribute(ch[11],BASS_ATTRIB_VOL, 1);
			
			BASS_ChannelSlideAttribute(ch[11], BASS_ATTRIB_VOL, 0, (DWORD)10000);
			tail = true;
		}
		else if(secPos > 5){
			if(BASS_ChannelIsActive(ch[10]) == BASS_ACTIVE_PLAYING)
				BASS_ChannelPause(ch[10]);
			
			cout << secPos << ":" << vol << endl;
			if(vol == 0){
				BASS_ChannelPause(ch[11]);
				cout << "over" << endl;
			}
		}
		
		/*
		
		//if(length - secPos < 0.2 && !tail){
		if(secPos > 5 && !tail){
			BASS_ChannelSetAttribute(ch[11],BASS_ATTRIB_VOL, myVolume);
			//BASS_ChannelPause(ch[10]);
			if (!BASS_ChannelPlay(ch[11],FALSE))
				cout << "Can't play sample" << endl;
			
			BASS_ChannelSlideAttribute(ch[10], BASS_ATTRIB_VOL, 0, (DWORD)300);
			
			BASS_ChannelSlideAttribute(ch[11], BASS_ATTRIB_VOL, 1, (DWORD)200);
			tail = true;
			cout << vol << endl;
		}
		
		if(tail && vol < 1){
			//BASS_ChannelSetAttribute(ch[10], BASS_ATTRIB_VOL, startVolume *= 0.999);
			//BASS_ChannelSetAttribute(ch[11], BASS_ATTRIB_VOL, myVolume += 0.0001);
			//cout << vol << endl;
		}
		else if(tail && !BASS_ChannelIsSliding(ch[11], BASS_ATTRIB_VOL)){
			BASS_ChannelSlideAttribute(ch[11], BASS_ATTRIB_VOL, 0, (DWORD)8000);
		}
		*/
	  }
	  
	  //if( count == 1000000){
		//  if (!BASS_ChannelPlay(ch[1],FALSE))
		//		cout << "Can't play sample" << endl;
	  //}
	count++;
  };
  
  return 0;	// return 0 indicates program exited OK
}
