#pragma once
#include "test.h"
#include "ResSound.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "core/file.h"
#include "Sound.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

extern SoundPlayer splayer;

class TestSoundInstance : public Test
{
public:

    spSprite orange;
    spSoundInstance snd;

    TestSoundInstance()
    {
		//snd = splayer.play("track_48000_stereo");


        //snd = splayer.play("track_44100_mono", PlayOptions().loop(false));
		addButton("set_streaming", "set streaming sound");
		addButton("set_static", "set static sound");

		addButton("loop=1", "loop = true");
		addButton("loop=0", "loop = false");
		//addButton("set_not_looped", )

		addButton("play", "play");
		addButton("pause", "pause");
		addButton("resume", "resume");
		addButton("stop", "stop");

		addButton("fadeIn", "fade in");
		addButton("fadeOut", "fade out");
    }

	void soundEvent(Event*)
	{
		notify("done");
	}

	void set(spSoundInstance s)
	{
		if (snd)
			snd->stop();
		snd = s;
		s->setDoneCallback(CLOSURE(this, &TestSoundInstance::soundEvent));
	}
    void clicked(string id)
	{
		if (id == "set_streaming")
		{
			set(splayer.play("track_44100_mono", PlayOptions().pause()));
		}

		if (id == "set_static")
		{
			set(splayer.play("win_round", PlayOptions().pause()));
		}

		if (id == "loop=1")
			snd->setLoop(true);

		if (id == "loop=0")
			snd->setLoop(false);

		if (id == "play")
		{
			snd->play();
		}

		if (id == "pause")
		{
			snd->pause();
		}

		if (id == "resume")
		{
			snd->resume();
		}

		if (id == "stop")
		{
			snd->stop();
		}


		if (id == "fadeIn")
		{
			snd->fadeIn(1000);
		}

		if (id == "fadeOut")
		{
			snd->fadeOut(1000);
		}

    }
};
