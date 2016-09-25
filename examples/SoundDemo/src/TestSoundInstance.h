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

		//addButton("set_loop", "set looped");
		//addButton("set_not_looped", )

		addButton("play", "play");
		addButton("pause", "pause");
		addButton("resume", "resume");

		addButton("fadeIn", "fade in");
		addButton("fadeOut", "fade out");
    }


    void clicked(string id)
	{
		if (id == "set_streaming")
		{
			snd = splayer.play("track_44100_mono", PlayOptions().pause());
		}

		if (id == "set_static")
		{
			snd = splayer.play("win_round", PlayOptions().pause());
		}

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
