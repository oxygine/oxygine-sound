#pragma once
#include "test.h"
#include "ResSound.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "core/file.h"
#include "ResSound.h"

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

extern Resources resources;
extern SoundPlayer splayer;

class TestSoundFormats: public Test
{
public:
    TestSoundFormats()
    {
        Resources::resources lst;
        resources.collect(lst);
        for (size_t i = 0; i < lst.size(); ++i)
        {
            ResSound* rs = dynamic_cast<ResSound*>(lst[i].get());
            if (!rs)
                continue;
            if (rs->getName().find("track_") != 0 &&
                    rs->getName().find("success_") != 0)
                continue;

            string s = rs->getName();
            for (size_t i = 0; i < s.size(); ++i)
                if (s[i] == '_')
                    s[i] = ' ';
            addButton(rs->getName(), s);
        }

        addButton("stop", "stop");
        addButton("pause", "pause");
        addButton("resume", "resume");
        addButton("fadeOut", "fadeOut");
    }

    void clicked(string id)
    {
        if (id == "stop")
        {
            splayer.stop();
            return;
        }
        if (id == "pause")
        {
            splayer.pause();
            return;
        }
        if (id == "resume")
        {
            splayer.resume();
            return;
        }
        if (id == "fadeOut")
        {
            splayer.fadeOut(1000);
            return;
        }

        spSoundInstance snd = splayer.play(id, PlayOptions().loop());
        if (!snd)
            return;
        snd->setDoneCallback(CLOSURE(this, &TestSoundFormats::soundDone));
        snd->setAboutDoneCallback(CLOSURE(this, &TestSoundFormats::soundAboutDone));
    }

    void soundAboutDone(Event* ev)
    {
        SoundInstance::SoundEvent* event = (SoundInstance::SoundEvent*)ev;
        this->notify("about done");
    }


    void soundDone(Event* ev)
    {
        SoundInstance::SoundEvent* event = (SoundInstance::SoundEvent*)ev;
        this->notify("done");
        /*
        SoundInstance *ins = event->instance;
        spSoundInstance instance = splayer.play(ins->getDesc().id, false, 0, 0, -1, false, ins->getChannel());
        instance->setAboutDoneCallback(CLOSURE(this, &TestSoundFormats::soundDone));
        */
    }
};