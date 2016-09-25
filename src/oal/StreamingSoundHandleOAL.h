#pragma once
#include "Sound_oal.h"

namespace oxygine
{
	class StreamingSoundHandleOAL : public SoundHandleOAL
	{
	public:
		StreamingSoundHandleOAL(SoundStream*);

	protected:
		void _xplay() override;
		void _xpause() override;
		void _xresume() override;
		void _xupdate() override;

		SoundStream *_stream;
	};

}
