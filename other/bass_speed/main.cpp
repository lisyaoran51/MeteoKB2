// BASS demo test
// A very basic OpenGL/GLUT application
// Create a window, load some sounds and play them.
// A test program for BASS - available from http://www.un4seen.com/
// This program uses two sound effects - available from http://www.freesound.org/
// To test this program you will need two wav sound effect files!
#include "bass.h"
#include "bass_fx.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
//#include <GL/glut.h>

// g++ main.cpp -o test2 -lbass -lbass_fx

using std::cout;
using std::endl;





int main(int argc, char **argv)
{
  if (!BASS_Init(-1,44100,0,0,NULL))
		cout << "Can't initialize device";

	HSTREAM stream;
	
	char * fileName = "Take_a_bow.mp3";
	
	stream = BASS_StreamCreateFile(false, fileName, 0, 0, BASS_STREAM_DECODE);//BASS_STREAM_DECODE

	HSTREAM stream2 = BASS_FX_TempoCreate(stream, BASS_FX_FREESOURCE);
	
	float freq = 44100;
	BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream2), BASS_ATTRIB_FREQ, &freq);
	freq *= 0.5f;
	cout << freq << endl;
	BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_FREQ, freq);

	float f=0;
	BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream2), BASS_ATTRIB_TEMPO_PITCH, &f);
	cout << f << endl;
	BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_PITCH, -log(0.5f)/log(1.0594630943592953098f));
	
	cout << BASS_ChannelGetLength(stream2, BASS_POS_BYTE) << endl;
	
	cout << BASS_ChannelBytes2Seconds(stream2, BASS_ChannelGetLength(stream2, BASS_POS_BYTE)) << endl;
	
	if (!BASS_ChannelPlay(stream2, true)) {
		cout << "Can't play sample" << endl;
	}
	
	
	int count = 0;
	 while(1){
		 double currentTimeLocal = BASS_ChannelBytes2Seconds(stream2, BASS_ChannelGetPosition(stream2, BASS_POS_BYTE));
		 cout << currentTimeLocal << "," << currentTimeLocal/0.5f << ", " << count << endl;
	  //usleep(1000000);
	  sleep(1);
	  count++;
	  
	  if(count % 11 == 10){
		  float freq = 44100;
		BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream2), BASS_ATTRIB_FREQ, &freq);
		freq *= 1.2f;
		cout << freq << endl;
		BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_FREQ, freq);
		float f=0;
		BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream2), BASS_ATTRIB_TEMPO_PITCH, &f);
		cout << f << endl;
		BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_PITCH, -log(1.2f)/log(1.0594630943592953098f));
	  }
	  if(count % 11 == -1){
		QWORD pos = BASS_ChannelSeconds2Bytes(stream2, 60.f);
		if(!BASS_ChannelSetPosition(stream2, pos, BASS_POS_BYTE)){
			cout << "Can't set pos" << endl;
		}
		
		}
	 }
  
  return 0;	// return 0 indicates program exited OK
}
