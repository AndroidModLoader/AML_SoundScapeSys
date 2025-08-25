LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc

### Mod Filename
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_MODULE := SoundScapeSys
else
	LOCAL_MODULE := SoundScapeSys64
endif

### Compile Args
LOCAL_CFLAGS    += -O3 -mfloat-abi=softfp -mfpu=neon -DNDEBUG -std=c11
LOCAL_CXXFLAGS  += -O3 -mfloat-abi=softfp -mfpu=neon -DNDEBUG -std=c++17

### Source Files
LOCAL_SRC_FILES := main.cpp soundscape.cpp sounddef.cpp soundsys.cpp mod/logger.cpp mod/config.cpp miniaudio/miniaudio.c \
                   moduletemplate.cpp modules/gtasa.cpp

### Libraries
LOCAL_LDLIBS += -llog

include $(BUILD_SHARED_LIBRARY)