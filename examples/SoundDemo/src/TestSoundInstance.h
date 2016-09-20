#pragma once
#include "test.h"
#include "ResSound.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "core/file.h"

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
        snd = splayer.play("track_44100_mono", PlayOptions().loop(false));

        addButton("fop", "fadeOut -> Pause");
        addButton("for", "resume -> fadeIn");
    }

    void clicked(string id)
    {
        if (id == "fop")
        {
            snd->fadeOutPause(300);
        }
        if (id == "for")
        {
            snd->fadeIn(300);
        }
    }
};
