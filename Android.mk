LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := dependencies/openal-android/obj/local/$(TARGET_ARCH_ABI)/libopenal.a
include $(PREBUILT_STATIC_LIBRARY) 



include $(CLEAR_VARS)

LOCAL_MODULE    := oxygine-sound_static
LOCAL_MODULE_FILENAME := liboxygine-sound

LOCAL_SRC_FILES := \
		src/MemoryStream.cpp \
		src/OggStream.cpp \
		src/ResSound.cpp \
		src/SoundInstance.cpp \
		src/SoundPlayer.cpp \
		src/oal/Channel_oal.cpp \
		src/oal/SoundSystem_oal.cpp \
		src/oal/Sound_oal.cpp \
		src/null/SoundSystemNull.cpp \
		src/null/SoundNull.cpp \
		src/null/ChannelNull.cpp


LOCAL_CFLAGS := -DOXYGINE_SOUND=1

OXYGINE_SRC := $(LOCAL_PATH)/../oxygine-framework/oxygine/src

LOCAL_C_INCLUDES := $(OXYGINE_SRC)/ \
					$(LOCAL_PATH)/src \
					$(LOCAL_PATH)/dependencies/openal-android/jni/OpenAL/include \
					$(LOCAL_PATH)/dependencies/tremor/Tremor \
					$(LOCAL_PATH)/dependencies/libogg/include

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_EXPORT_CFLAGS += $(LOCAL_CFLAGS)
LOCAL_WHOLE_STATIC_LIBRARIES := libogg openal tremor
LOCAL_EXPORT_LDLIBS     := -lOpenSLES -ldl -llog -Wl,-s

include $(BUILD_STATIC_LIBRARY)

$(call import-module, SDL)
$(call import-module, oxygine-sound/dependencies/libogg)
$(call import-module, oxygine-sound/dependencies/tremor)