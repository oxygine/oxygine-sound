#pragma once
#include "oxygine/oxygine-include.h"
#include "oxygine/core/Object.h"
#include <list>

namespace oxygine
{
    class ResSound;

    class Sound
    {
    public:
        Sound() : _parent(0) {}
        virtual ~Sound() {}

        ResSound*   getRes() { return _parent;}
        void        setRes(ResSound* rs) { _parent = rs; }
        virtual int getDuration() const = 0;
    private:
        ResSound* _parent;
    };
}