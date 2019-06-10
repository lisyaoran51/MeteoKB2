#include "Base/Sheetmusic/io/file_reader.h"
#include "Base/Sheetmusic/format/sm_decoder.h"

using namespace Base::Sheetmusic::io;
using namespace Base::Sheetmusic::format;


int main() {

	Game* game = new MeteorGame();
	game->Async();

	
	game->Run();
	

	return 0;
}



int main_deprecated(int argc, char *argv[])
{
	file_reader_t* reader;
	
	// read sheet music
	sm_decoder_t decoder(reader);

	Sm<fall_effect_group>* sm;

	// decode sheet music to sm type
	decoder.decode(path, sm);

	// build events from sm
	vector<Event*>* events;
	sm_event_builder_t event_builder(sm);
	events = event_builder.build();

	event_timer_t timer;
	timer.add_range(events);

	event_runner_t event_runner;
	event_to_map_converter_t event2map_cvtr;
	neokb_renderer_t renderer;

	while (running)
	{
		event_runner.update();
		running_event_t tmp_events = event_runner.get_events();
		map = event2map_cvtr(tmp_events);
		renderer.renderer(map);
		
	}



}