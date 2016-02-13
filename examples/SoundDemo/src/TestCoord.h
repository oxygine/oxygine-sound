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

class TestCoord: public Test
{
public:
    
    spSprite orange; 
    spSoundInstance snd;

    TestCoord()
    {
       orange = new ColorRectSprite;
       orange->setColor (Color::Orange);
       orange->setSize (Vector2 (800, 400));
       orange->setPosition (Vector2 (960 / 2 - 800 / 2, 640 / 2 - 400 / 2));

       orange->addEventListener(TouchEvent::OVER, CLOSURE(this, &TestCoord::onEvent));
       orange->addEventListener(TouchEvent::MOVE, CLOSURE(this, &TestCoord::onEvent));
       orange->addEventListener(TouchEvent::OUT, CLOSURE(this, &TestCoord::onEvent));

       orange->setInputChildrenEnabled (false);
       orange->attachTo(ui);
     }

    void onEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        if (ev->type == TouchEvent::OVER)
        {
            orange->setColor(Color::Green);
            snd = splayer.play("amb_bird_2", PlayOptions().loop());
        }
        else if (ev->type == TouchEvent::OUT)
        {
            orange->setColor(Color::Orange);
            splayer.stop();

        } else { // TouchEvent::MOVE

            Vector2 const center = orange->getSize() / 2;
            Vector2 const dir    = te->localPosition - center;
  
            snd->getChannel()->setCoord (dir.x / 10000, dir.y / 10000);
        }
    }
};
