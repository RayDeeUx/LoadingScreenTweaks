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
			getChildOfType<TextArea>(this, 0)->setVisible(false); // hide loading screen splash text
		if (Mod::get()->getSettingValue<bool>("hideProgressBar"))
			getChildOfType<CCSprite>(this, 6)->setVisible(false); // hide progress bar
		if (Mod::get()->getSettingValue<bool>("hideCocosAndFmod")) {
			getChildOfType<CCSprite>(this, 3)->setVisible(false); //hide cocos
			getChildOfType<CCSprite>(this, 4)->setVisible(false); //hide fmod
		}
		return true;
	}
};