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
	
	stream = BASS_StreamCreateFile(false, fileName, 0, 0, BASS_STREAM_DECODE);

	HSTREAM stream2 = BASS_FX_TempoCreate(stream, BASS_FX_FREESOURCE);
	
	float freq = 44100;
	BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream2), BASS_ATTRIB_FREQ, &freq);
	freq /= 1.0594630943592953098f;
	freq /= 1.0594630943592953098f;
	freq /= 1.0594630943592953098f;
	freq /= 1.0594630943592953098f;
	freq /= 1.0594630943592953098f;
	freq /= 1.0594630943592953098f;
	freq /= 1.0594630943592953098f;
	cout << freq << endl;
	BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_FREQ, freq);

	float f=0;
	BASS_ChannelGetAttribute(BASS_FX_TempoGetSource(stream2), BASS_ATTRIB_TEMPO_PITCH, &f);
	cout << f << endl;
	BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_PITCH, 7.f);
	
	if (!BASS_ChannelPlay(stream2, true)) {
		cout << "Can't play sample" << endl;
	}
	 while(1){
	  usleep(1);
	 }
  
  return 0;	// return 0 indicates program exited OK
}
