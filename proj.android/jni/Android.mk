LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp\
                   ../../Classes/LVSplashScene/LVSplashScene.cpp\
                   ../../Classes/LVPlayScene/LVPlayScene.cpp\
                   ../../Classes/LVGameOverScene/LVGameOverScene.cpp\
                   ../../Classes/LVAstronaut/LVAstronautSprite.cpp\
                   ../../Classes/LVAstronaut/LVAstronautManager.cpp\
                   ../../Classes/LVSpaceBuilding/LVSpaceBuilding.cpp\
                   ../../Classes/LVLiftBox/LVLiftBox.cpp\






LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes\
							$(LOCAL_PATH)/../../Classes/LVSplashScene\
							$(LOCAL_PATH)/../../Classes/LVPlayScene\
							$(LOCAL_PATH)/../../Classes/LVGameOverScene\
							$(LOCAL_PATH)/../../Classes/LVAstronaut\
							$(LOCAL_PATH)/../../Classes/LVSpaceBuilding\
							$(LOCAL_PATH)/../../Classes/LVLiftBox\




# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
