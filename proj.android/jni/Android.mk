LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/BattleLayers/Joystick.cpp \
                   ../../Classes/BattleLayers/MainLayer.cpp \
                   ../../Classes/BattleLayers/HUDLayer.cpp \
                   ../../Classes/BattleLayers/SettingsLayer.cpp \
                   ../../Classes/BattleLayers/EndGameStats.cpp \
                   ../../Classes/BattleLogic/LogicalShip.cpp \
                   ../../Classes/BattleLogic/LogicalWeapon.cpp \
                   ../../Classes/BattleLogic/Spawner.cpp \
                   ../../Classes/BattleLogic/BattleManager.cpp \
                   ../../Classes/BattleLogic/Skills/NormalAttack.cpp \
                   ../../Classes/BattleLogic/Skills/DoTAttack.cpp \
                   ../../Classes/AI/AIBase.cpp \
                   ../../Classes/Physics/BodyBase.cpp \
                   ../../Classes/Physics/ShipBase.cpp \
                   ../../Classes/Physics/BulletBase.cpp \
                   ../../Classes/Scenes/BattleScene.cpp \
                   ../../Classes/Scenes/MainMenuScene.cpp \
                   ../../Classes/Scenes/LevelSelectScene.cpp \
                   ../../Classes/Shaders/Shaders.cpp \
                   ../../Classes/GameLogic/GameManager.cpp \
                   ../../Classes/GameLogic/LevelInfo.cpp \
                   ../../Classes/PopupMenu.cpp \
                   ../../Classes/AppDelegate.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
