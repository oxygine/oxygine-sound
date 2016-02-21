#pragma once
#include <string>
#include <vector>
#include "core/ref_counter.h"
#include "core/oxygine.h"

namespace oxygine
{
    class Resource;
    class ResSound;
    class Resources;
    class Channel;
    struct sound_desc;

    using namespace std;

    class PlayOptions
    {
    public:
        PlayOptions() : _looped(false), _pitch(1.0f), _fadeIn(0), _fadeOut(0), _paused(false), _volume(-1.0f), _seek(0) {}

        PlayOptions& loop() { _looped = true; return *this; }
        PlayOptions& pitch(float v)  { _pitch = v; return *this; }
        PlayOptions& volume(float v)  { _volume = v; return *this; }
        PlayOptions& fade(timeMS fadeIn, timeMS fadeOut = 0) { _fadeIn = fadeIn; _fadeOut = fadeOut; return *this; }
        PlayOptions& pause() { _paused = true; return *this; }
        PlayOptions& seek(timeMS ms) { _seek = ms; return *this; }


        float _pitch;
        float _volume;
        timeMS _fadeIn;
        timeMS _fadeOut;
        bool _paused;
        bool _looped;
        timeMS _seek;
    };



    DECLARE_SMART(SoundInstance, spSoundInstance);


    class SoundPlayer
    {
    public:
        static void initialize();
        static void free();

        SoundPlayer();
        ~SoundPlayer();


        spSoundInstance play(Resource*, const PlayOptions& = PlayOptions());
        spSoundInstance play(const std::string& id, const PlayOptions& = PlayOptions());
        spSoundInstance continuePlay(Resource*, Channel*, const PlayOptions& = PlayOptions());

        spSoundInstance getSoundByIndex(int index);
        int             getSoundsNum()const {return (int)_sounds.size();}
        unsigned int    getTime() const;

        void pause();
        void resume();

        void stopByID(const string& id);
        void stop();

        void fadeOut(int ms);

        void update();

        void setVolume(float v);
        void setResources(Resources* res);

        bool IsPaused() { return _paused; }


    private:
        friend class SoundInstance;
        void removeSoundInstance(SoundInstance*);
        spSoundInstance prepareSound(Resource* res, Channel* channel, const PlayOptions& opt);

        static void _onSoundDone(void* This, Channel* channel, const sound_desc& desc);

        static void _onSoundAboutDone(void* This, Channel* channel, const sound_desc& desc);
        void onSoundAboutDone(SoundInstance* soundInstance, Channel* channel, const sound_desc& desc);

        Resources* _resources;
        float _volume;

        typedef std::vector<spSoundInstance> playingSounds;
        playingSounds _sounds;

        timeMS _time;
        timeMS _lastUpdateTime;

        bool _paused;
    };
}