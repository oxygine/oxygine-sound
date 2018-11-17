#pragma once
#include "test.h"
#include "oxygine-sound.h"

extern SoundPlayer splayer;

class TestCoord: public Test
{
public:

    spColorRectSprite sprite;
    spSoundInstance snd;

    TestCoord()
    {
        _content->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &TestCoord::onEvent));
        _content->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &TestCoord::onEvent));
        _content->addEventListener(TouchEvent::MOVE, CLOSURE(this, &TestCoord::onEvent));
    }

    ~TestCoord()
    {

    }

    void onEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);


        Vector2 center = _content->getSize() / 2;
        Vector2 dir = te->localPosition - center;
        Vector3 pos3d = Vector3(dir.x, dir.y, 0) / 1000.0f;



        if (ev->type == TouchEvent::TOUCH_DOWN)
        {
            spColorRectSprite spr;
            spr = new ColorRectSprite;
            spr->setSize(20, 20);
            spr->attachTo(_content);
            spr->setTouchEnabled(false);
            spr->setAnchor(0.5f, 0.5f);
            spr->setPosition(te->localPosition);
            sprite = spr;

            snd = splayer.play("amb_bird_2", PlayOptions().loop().position3d(pos3d));

#ifdef OX_HAS_CPP11
            snd->setDoneCallback([spr](Event*)
            {
                spr->detach();
            });
#endif
        }

        if (!snd)
            return;

        if (ev->type == TouchEvent::TOUCH_UP)
        {
            snd->setLoop(false);
            sprite = 0;
            snd = 0;
        }

        if (ev->type == TouchEvent::MOVE)
        {
            snd->setPosition3D(pos3d);
            sprite->setPosition(te->localPosition);
        }
    }
};
