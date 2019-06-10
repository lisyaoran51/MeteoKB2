VPATH=Base:Base/Config:Base/Graphic:Base/Graphic/Map:Base/Graphic/Renderer:Base/Play:Base/Ruleset:\
Base/Scheduler:Base/Scheduler/Event:Base/Scheduler/Event/ControlPoints:Base/Scheduler/Event/GameEvents:Base/Scheduler/Event/Effect:\
Base/Scheduler:Base/Scheduler/Event/GameEvents:Base/Scheduler:Base/Scheduler/Event/SystemEvents:\
Base/Scheduler/Event/Effect/Algorithm:\
Base/Sheetmusic:Base/Sheetmusic/Format:Base/Sheetmusic/IO:Base/Sheetmusic/Pattern:\
Devices:Devices/Graphic/Renderers:Devices/LedDriver:\
RulesetMeteor:RulesetMeteor/Config:RulesetMeteor/Graphic:RulesetMeteor/Graphic/Renderer:\
RulesetMeteor/Play:RulesetMeteor/Ruleset:RulesetMeteor/Scheduler/Event/Effect:RulesetMeteor/Scheduler/Event/Effect/Algorithm:\
RulesetMeteor/Sheetmusic:RulesetMeteor/Sheetmusic/Pattern:\
Util:Util/Hierachal:Util/Update

#INCLUDE =  

INCLUDE = -IBase -IBase/Config -IBase/Graphic -IBase/Graphic/Map -IBase/Graphic/Renderer -IBase/Play -IBase/Ruleset \
-IBase/Scheduler -IBase/Scheduler/Event -IBase/Scheduler/Event/ControlPoints -IBase/Scheduler/Event/GameEvents -IBase/Scheduler/Event/Effect \
-IBase/Scheduler/Event/GameEvents -IBase/Scheduler/Event/SystemEvents \
-IBase/Sheetmusic -IBase/Sheetmusic/Pattern -IBase/Sheetmusic/IO -IBase/Sheetmusic/Format \
-IRulesetMeteor -IRulesetMeteor/Config -IRulesetMeteor/Graphic -IRulesetMeteor/Graphic/Renderer \
-IRulesetMeteor/Play -IRulesetMeteor/Scheduler/Event/Effect -IRulesetMeteor/Scheduler/Event/Effect/Algorithm \
-IRulesetMeteor/Sheetmusic -IRulesetMeteor/Sheetmusic/Pattern \
-IUtil -IUtil/Hierachal -IUtil/Update

#INCLUDE = -IBase -IBase/Config -IBase/Graphic -I./Base/Graphic/Map -I./Base/Graphic/Renderer -I./Base/Play:Base/Ruleset \
#-I./Base/Scheduler -I./Base/Scheduler/Event -I./Base/Scheduler/Event/ControlPoints -I./Base/Scheduler/Event/Effect \
#-I./RulesetMeteor -I./RulesetMeteor/Config -I./RulesetMeteor/Graphic -I./RulesetMeteor/Graphic/Renderer \
#-I./RulesetMeteor/Play -I./RulesetMeteor/Scheduler/Event/Effect -I./RulesetMeteor/Scheduler/Event/Effect/Algorithm \
#-I./RulesetMeteor/Sheetmusic -I./RulesetMeteor/Sheetmusic/Pattern \
#-I./Util -I./Util/Hierachal -I./Util/Update


TARGET  = MeteoKB
#compiler 
CC = g++
#cflags 
CFLAGS = -Wall -ansi -s -std=c++14 -pthread
#object 
OBJS = Program.o Game.o \
FrameworkConfigManager.o \
Map.o renderer.o \
Player.o Playfield.o Session.o Ruleset.o RulesetInfo.o \
Scheduler.o Event.o EventProcessorMaster.o GameEvent.o StartGameEvent.o ControlPoint.o NoteControlPoint.o Effect.o \
SystemEvent.o StopSystemEvent.o SystemEventHandler.o \
SheetmusicConverter.o SheetmusicManager.o SheetmusicMetadata.o SheetmusicPostprocessor.o WorkingSheetmusic.o \
SmDecoder.o SimpleSmDecoder.o FileReader.o Pattern.o PatternGenerator.o \
WS2812v10Renderer.o dma.o pwm.o ws2811.o mailbox.o pcm.o rpihw.o \
MeteorGame.o MeteorConfigManager.o MeteorPlayfield.o MeteorRuleset.o MeteorRulesetExecutor.o \
ExplodeEffect.o ExplodeEffectMapper.o FallEffect.o FallEffectMapper.o GlowLineEffect.o GlowLineEffectMapper.o TargetLineEffect.o TargetLineEffectMapper.o \
ExplodeMapAlgorithm.o FallMapAlgorithm.o GlowLineMapAlgorithm.o TargetLineMapAlgorithm.o \
MeteorSheetmusicConverter.o MeteorPatternGenerator.o \
FileSystem.o MtoObject.o ProgramInitializer.o \
Cachable.o ChildAddable.o HasParent.o Loadable.o LoadStateHandler.o \
Updater.o Updatable.o



all:$(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $@ -pthread
	
.c.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $< 	
	
.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $< 

clean:
	rm -f *.o $(TARGET)