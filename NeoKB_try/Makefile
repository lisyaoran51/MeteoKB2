VPATH=Framework:Framework/Allocation:Framework/Allocation/Hierachal:Framework/Allocation/Update:\
Framework/Audio:Framework/Audio/Sample:Framework/Audio/Track:\
Framework/Configurations:\
Framework/Devices:\
Framework/Exception:\
Framework/Graphic:Framework/Graphic/Map:\
Framework/Host:\
Framework/Input:Framework/Input/Handler:Framework/Input/KeyBindings:\
Framework/IO:Framework/IO/Stores:\
Framework/Database:\
Framework/Scenes:\
Framework/Threading:\
Framework/Timing:Framework/Timing/SpeedAdjusters:\
\
Games:Games/Input:\
Games/Ruleset:Games/Ruleset/Modifiers:Games/Ruleset/Judgements:\
Games/Scene:Games/Scene/Interlude:Games/Scene/Play:Games/Scene/Select:\
Games/Scheduler/Event:Games/Scheduler/Event/ControlPoints:Games/Scheduler/Event/Effect:\
Games/Scheduler/Event/Effect/Algorithm:Games/Scheduler/Event/GameEvents:Games/Scheduler/Event/SystemEvents:\
Games/Sheetmusic:Games/Sheetmusic/Format:Games/Sheetmusic/Format/Parser:Games/Sheetmusic/IO:Games/Sheetmusic/Pattern:\
Games/UI:\
\
Instruments:Instruments/Audio:Instruments/Input:\
\
Util:Util/DataStructure:\
\
ThirdParty/Bass:\
\
Desktop:Desktop/Devices:\
\
RulesetMeteor:RulesetMeteor/Config:RulesetMeteor/Input:RulesetMeteor/Play:RulesetMeteor/Ruleset:RulesetMeteor/Ruleset/Judgements:\
RulesetMeteor/Scheduler/Event:RulesetMeteor/Scheduler/Event/ControlPoints:RulesetMeteor/Scheduler/Event/Effect:RulesetMeteor/Scheduler/Event/Effect/Algorithm:\
RulesetMeteor/Sheetmusic:RulesetMeteor/Sheetmusic/Pattern:\
RulesetMeteor/Timing:\




INCLUDE = -IFramwork -IFramework/Allocation -IFramework/Allocation/Hierachal -IFramework/Allocation/Update \
-IFramework/Audio -IFramework/Audio/Sample -IFramework/Audio/Track \
-IFramework/Configurations \
-IFramework/Devices \
-IFramework/Exception \
-IFramework/Graphic -IFramework/Graphic/Map \
-IFramework/Host \
-IFramework/Input -IFramework/Input/Handler -IFramework/Input/KeyBindings \
-IFramework/IO -IFramework/IO/Stores \
-IFramework/Database \
-IFramework/Scenes \
-IFramework/Threading \
-IFramework/Timing -IFramework/Timing/SpeedAdjusters \
\
-IGames -IGames/Input \
-IGames/Ruleset -IGames/Ruleset/Modifiers -IGames/Ruleset/Judgements \
-IGames/Scene -IGames/Scene/Interlude -IGames/Scene/Play -IGames/Scene/Select \
-IGames/Scheduler/Event -IGames/Scheduler/Event/ControlPoints -IGames/Scheduler/Event/Effect \
-IGames/Scheduler/Event/Effect/Algorithm -IGames/Scheduler/Event/GameEvents -IGames/Scheduler/Event/SystemEvents \
-IGames/Sheetmusic -IGames/Sheetmusic/Format -IGames/Sheetmusic/Format/Parser -IGames/Sheetmusic/IO -IGames/Sheetmusic/Pattern \
-IGames/UI \
\
-IInstruments -IInstruments/Audio -IInstruments/Input \
\
-IUtil -IUtil/DataStructure \
\
-IThirdParty/Bass \
\
-IDesktop -IDesktop/Devices \
\
-IRulesetMeteor -IRulesetMeteor/Config -IRulesetMeteor/Input -IRulesetMeteor/Play -IRulesetMeteor/Ruleset -IRulesetMeteor/Ruleset/Judgements \
-IRulesetMeteor/Scheduler/Event -IRulesetMeteor/Scheduler/Event/ControlPoints -IRulesetMeteor/Scheduler/Event/Effect -IRulesetMeteor/Scheduler/Event/Effect/Algorithm \
-IRulesetMeteor/Sheetmusic -IRulesetMeteor/Sheetmusic/Pattern \
-IRulesetMeteor/Timing 


#INCLUDE = -IGames -IGames/Config -IGames/Graphic -IGames/Graphic/Map -IGames/Graphic/Renderer -IGames/Play -IGames/Ruleset \
#-IGames/Scheduler -IGames/Scheduler/Event -IGames/Scheduler/Event/ControlPoints -IGames/Scheduler/Event/GameEvents -IGames/Scheduler/Event/Effect \
#-IGames/Scheduler/Event/GameEvents -IGames/Scheduler/Event/SystemEvents \
#-IGames/Sheetmusic -IGames/Sheetmusic/Pattern -IGames/Sheetmusic/IO -IGames/Sheetmusic/Format \
#-IRulesetMeteor -IRulesetMeteor/Config -IRulesetMeteor/Graphic -IRulesetMeteor/Graphic/Renderer \
#-IRulesetMeteor/Play -IRulesetMeteor/Scheduler/Event/Effect -IRulesetMeteor/Scheduler/Event/Effect/Algorithm \
#-IRulesetMeteor/Sheetmusic -IRulesetMeteor/Sheetmusic/Pattern \
#-IUtil -IUtil/Hierachal -IUtil/Update

#INCLUDE = -IGames -IGames/Config -IGames/Graphic -I./Games/Graphic/Map -I./Games/Graphic/Renderer -I./Games/Play:Games/Ruleset \
#-I./Games/Scheduler -I./Games/Scheduler/Event -I./Games/Scheduler/Event/ControlPoints -I./Games/Scheduler/Event/Effect \
#-I./RulesetMeteor -I./RulesetMeteor/Config -I./RulesetMeteor/Graphic -I./RulesetMeteor/Graphic/Renderer \
#-I./RulesetMeteor/Play -I./RulesetMeteor/Scheduler/Event/Effect -I./RulesetMeteor/Scheduler/Event/Effect/Algorithm \
#-I./RulesetMeteor/Sheetmusic -I./RulesetMeteor/Sheetmusic/Pattern \
#-I./Util -I./Util/Hierachal -I./Util/Update




TARGET  = MeteoKB
#compiler 
CC = g++
#cflags 
CFLAGS = -Wall -ansi -s -std=c++14 -pthread -lbass -lstdc++fs
#object 
OBJS = Main.o \
\
Base.o Game.o \
Cachable.o ChildAddable.o Container.o HasParent.o Loadable.o LoadStateHandler.o MtoObject.o Schedulable.o Scheduler.o Triggerable.o Updatable.o \
AdjustableAudioComponent.o AudioComponent.o AudioManager.o \
BassSample.o BassSampleChannel.o Sample.o SampleChannel.o SampleManager.o \
BassTrack.o Track.o TrackManager.o \
ConfigManager.o FrameworkConfigManager.o \
DatabaseContext.o DatabaseContextFactory.o DatabaseStore.o FakeDatabaseContextFactory.o FakeMeteoDatabaseContext.o MeteoDatabaseContext.o \
BluetoothDevice.o Device.o DisplayDevice.o InputDevice.o KeyboardDevice.o PanelDevice.o \
Drawable.o Map.o \
GameHost.o MeteoGameHost.o \
BluetoothState.o CustomInputManager.o InputManager.o InputState.o KeyboardState.o PanelState.o PassThroughInputManager.o UserInputManager.o \
BluetoothInputHandler.o KeyboardInputHandler.o PanelInputHandler.o KeyBindingStore.o \
BluetoothPhone.o Display.o FileStore.o Keyboard.o MainInterface.o Panel.o Peripheral.o PlatformStorage.o Storage.o StorageResourceStore.o DynamicPathResourceStore.o \
Scene.o \
GameThread.o \
DecoupledInterpolatingFramedClock.o FramedClock.o InterpolatingFramedClock.o OffsetFramedClock.o StopwatchClock.o ThrottledFramedClock.o TimeController.o \
LinearSpeedAdjuster.o SpeedAdjuster.o \
\
MeteoGame.o MeteoGameBase.o\
Ruleset.o RulesetInfo.o RulesetStore.o Judgement.o HasHitWindow.o \
Loader.o MeteoScene.o Intro.o Player.o PlayerLoader.o Playfield.o MainMenu.o PlaySongSelect.o SongSelect.o \
Event.o EventProcessorMaster.o HitObject.o ControlPoint.o NoteControlPoint.o NoteControlPointHitObject.o Effect.o LinearMapPitchShifter.o MapPitchShifter.o \
GameEvent.o StartGameEvent.o StopSystemEvent.o SystemEvent.o SystemEventHandler.o \
ResourceStoreWorkingSm.o SheetmusicConverter.o SheetmusicManager.o SheetmusicMetadata.o SheetmusicPostprocessor.o SheetmusicStore.o WorkingSheetmusic.o \
SimpleSmDecoder.o SmDecoder.o FileReader.o Pattern.o PatternGenerator.o \
SheetmusicSelectPanel.o \
\
Instrument.o HasPitch.o MeteoPiano.o Piano.o PianoInputManager.o \
\
FileSystem.o ProgramInitializer.o \
\
MeteoGameDesktop.o Program.o MeteoLightBoardV1.o bcm2835.o MeteoBluetoothDevice.o MeteoDisplayDevice.o MeteoKeyboardDevice.o MeteoPanelDevice.o MeteoPanelBoardV1.o pi2c.o\
\
MeteorGame.o MeteorConfigManager.o MeteorInputManager.o MeteorPlayfield.o MeteorRuleset.o MeteorRulesetExecutor.o MeteorJudgement.o MeteorHitWindow.o \
MeteorEventProcessorMaster.o MeteorControlPointHitObject.o \
ExplodeEffect.o ExplodeEffectMapper.o FallEffect.o FallEffectMapper.o GlowLineEffect.o GlowLineEffectMapper.o TargetLineEffect.o TargetLineEffectMapper.o \
ExplodeMapAlgorithm.o FallMapAlgorithm.o GlowLineMapAlgorithm.o TargetLineMapAlgorithm.o \
MeteorSheetmusicConverter.o MeteorSheetmusicPostprocessor.o MeteorPatternGenerator.o \
MeteorTimeController.o 


#Program.o Game.o \
#FrameworkConfigManager.o \
#Map.o renderer.o \
#Player.o Playfield.o Session.o Ruleset.o RulesetInfo.o \
#Scheduler.o Event.o EventProcessorMaster.o GameEvent.o StartGameEvent.o ControlPoint.o NoteControlPoint.o Effect.o \
#SystemEvent.o StopSystemEvent.o SystemEventHandler.o \
#SheetmusicConverter.o SheetmusicManager.o SheetmusicMetadata.o SheetmusicPostprocessor.o WorkingSheetmusic.o \
#SmDecoder.o SimpleSmDecoder.o FileReader.o Pattern.o PatternGenerator.o \
#WS2812v10Renderer.o dma.o pwm.o ws2811.o mailbox.o pcm.o rpihw.o \
#MeteorGame.o MeteorConfigManager.o MeteorPlayfield.o MeteorRuleset.o MeteorRulesetExecutor.o \
#ExplodeEffect.o ExplodeEffectMapper.o FallEffect.o FallEffectMapper.o GlowLineEffect.o GlowLineEffectMapper.o TargetLineEffect.o TargetLineEffectMapper.o \
#ExplodeMapAlgorithm.o FallMapAlgorithm.o GlowLineMapAlgorithm.o TargetLineMapAlgorithm.o \
#MeteorSheetmusicConverter.o MeteorPatternGenerator.o \
#FileSystem.o MtoObject.o ProgramInitializer.o \
#Cachable.o ChildAddable.o HasParent.o Loadable.o LoadStateHandler.o \
#Updater.o Updatable.o



all:$(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $@ -pthread -lstdc++fs -lbass
	
.c.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $< 	
	
.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $< 

clean:
	rm -f *.o $(TARGET)