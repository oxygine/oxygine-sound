cmake_minimum_required (VERSION 2.6)
project (OXYGINESOUND)

set(OXYGINESOUND_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tremor/Tremor
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libogg/include
	)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message(status ${OXYGINESOUND_INCLUDE_DIRS})

set(OXYGINESOUND_DEFINITIONS	
	-DOXYGINE_SOUND=1)

set(SRC
	src/Channel.h
	src/MemoryStream.cpp
	src/MemoryStream.h	
	src/ResSound.cpp
	src/ResSound.h
	src/Sound.h
	src/SoundInstance.cpp
	src/SoundInstance.h
	src/SoundPlayer.cpp
	src/SoundPlayer.h
	src/SoundSystem.h

	src/null/SoundSystemNull.cpp
	src/null/SoundSystemNull.h
	src/null/ChannelNull.cpp
	src/null/ChannelNull.h
	src/null/SoundNull.cpp
	src/null/SoundNull.h
	)

if(EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(SRC ${SRC}
		src/emscripten/SoundSystemEmscripten.cpp
		src/emscripten/SoundSystemEmscripten.h
		src/emscripten/ChannelEmscripten.cpp
		src/emscripten/ChannelEmscripten.h
		src/emscripten/SoundEmscripten.cpp
		src/emscripten/SoundEmscripten.h
	)

else(EMSCRIPTEN)

	add_subdirectory(dependencies/tremor tremor)
	
	set (SRC ${SRC}
		src/OggStream.cpp
		src/OggStream.h

		src/oal/Channel_oal.cpp
		src/oal/Channel_oal.h
		src/oal/SoundSystem_oal.cpp
		src/oal/SoundSystem_oal.h
		src/oal/Sound_oal.cpp
		src/oal/Sound_oal.h
		)

endif(EMSCRIPTEN)


add_library(oxygine-sound STATIC ${SRC})

if(EMSCRIPTEN)
	set(OXYGINESOUND_LIBS
		oxygine-sound)	

	set(linkFlags "-Wno-warn-absolute-paths")

	set_target_properties(oxygine-sound PROPERTIES LINK_FLAGS "${linkFlags}")

else(EMSCRIPTEN)
	set(OXYGINESOUND_INCLUDE_DIRS ${OXYGINESOUND_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openal/include)

	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	
		set(OXYGINESOUND_LIBS oxygine-sound -lopenal tremor -logg)

	else()

		set(LIBOGG libogg_static)

		if (MSVC)
			if (MSVC_VERSION EQUAL "1900")
				set(LIBOGG ${LIBOGG}_vs2013)
			endif()
		endif()	

		set(OXYGINESOUND_LIBS
			${CMAKE_CURRENT_SOURCE_DIR}/libs/win32/OpenAL32.lib
			${CMAKE_CURRENT_SOURCE_DIR}/libs/win32/${LIBOGG}.lib		
			tremor
			oxygine-sound)

	endif()
endif(EMSCRIPTEN)

include_directories(${OXYGINESOUND_INCLUDE_DIRS})




target_link_libraries(oxygine-sound -lopenal)
target_link_libraries(oxygine-sound -logg)


#target_link_libraries(oxyginemagicparticles)
set(OXYGINESOUND_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}/src)

set(OXYGINESOUND_INCLUDE_DIRS ${OXYGINESOUND_INCLUDE_DIRS} PARENT_SCOPE)
set(OXYGINESOUND_LIBS ${OXYGINESOUND_LIBS} PARENT_SCOPE)
set(OXYGINESOUND_DEFINITIONS ${OXYGINESOUND_DEFINITIONS} PARENT_SCOPE)