#include <Geode/Geode.hpp>
#include <Geode/binding/LoadingLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/TextArea.hpp>
#include <Geode/modify/CCSprite.hpp>

using namespace geode::prelude;

class $modify(LoadingLayer) {
	bool init (bool p0) {
		if (!LoadingLayer::init(p0)) return false;
		if (!(Mod::get()->getSettingValue<bool>("enabled"))) return true;
		if (Mod::get()->getSettingValue<bool>("hideSplashText"))
			getChildByID("text-area")->setVisible(false); // hide loading screen splash text
		if (Mod::get()->getSettingValue<bool>("hideProgressBar"))
			getChildByID("progress-slider")->setVisible(false); // hide progress bar
		if (Mod::get()->getSettingValue<bool>("hideCocosAndFmod")) {
			getChildByID("cocos2d-logo")->setVisible(false); // hide cocos
			getChildByID("fmod-logo")->setVisible(false); // hide fmod
		}
		return true;
	}
};
