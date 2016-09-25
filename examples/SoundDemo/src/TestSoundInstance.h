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
    SoundHandle* handle;

    TestSoundInstance()
    {
        handle = SoundSystem::get()->createHandle();
        handle->add(resources.getT<ResSound>("track_48000_stereo")->getSound());
        //handle->add(resources.getT<ResSound>("win_round")->getSound());

        //handle->setLoop(true);
        handle->play();

        //snd = splayer.play("track_44100_mono", PlayOptions().loop(false));

        addButton("pause", "pause");
        addButton("resume", "resume");
    }

    void doUpdate(const UpdateState& us)
    {
        handle->update();
    }

    void clicked(string id)
    {
        if (id == "pause")
        {
            handle->pause();
            // snd->fadeOutPause(300);
        }
        if (id == "resume")
        {
            handle->play();
            // snd->fadeIn(300);
        }
    }
};
