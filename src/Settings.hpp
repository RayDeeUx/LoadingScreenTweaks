#pragma once

#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

class MyButtonSettingV3 : public SettingV3 {
public:
	static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
		auto res = std::make_shared<MyButtonSettingV3>();
		auto root = checkJson(json, "MyButtonSettingV3");
		res->init(key, modID, root);
		res->parseNameAndDescription(root);
		res->parseEnableIf(root);
		return root.ok(std::static_pointer_cast<SettingV3>(res));
	}
	bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }
	bool isDefaultValue() const override {
        return true;
    }
    void reset() override {}
	SettingNodeV3* createNode(float width) override;
};

class MyButtonSettingNodeV3 : public SettingNodeV3 {
private:
	std::string m_desc = "";
protected:
	void onConfigDirButton(CCObject*) {
		file::openFolder(Mod::get()->getConfigDir());
		#ifndef GEODE_IS_MOBILE
		if (!CCKeyboardDispatcher::get()->getShiftKeyPressed()) return;
		FLAlertLayer::create(
			"Config Directory Opened!",
			m_desc,
			"Aight"
		)->show();
		#endif
	}
	bool init(std::shared_ptr<MyButtonSettingV3> setting, float width) {
        if (!SettingNodeV3::init(setting, width)) return false;
		this->setContentSize({ width, 40.f });
		std::string name = setting->getName()->value_or("Click me to open the Config Directory!");
		m_desc = setting->getDescription()->value_or("<cy>(Hopefully.)</c>\nThis setting does literally nothing new except being a button that acts as a shortcut to open the config directory for this mod (which is kind of important for using this mod properly, because this is where you will store your \"New Best\" sound effects), because the legendary developers in the Geode Team themselves forgot to add a shortcut button that would have done this exact same thing, and I have gotten far too many folks (rightfully) asking me to find the config directory when using some of my other mods, and I hate repeating the same things over and over again, and I don't know if the next major update for Geode will also repeat this mistake where HJfod forgets to include a config directory shortcut button, so this setting and this button are here for extra insurance, so please enjoy this extra button, I guess. Anyway, by the time you're finished reading this, your device should be finished with opening the config directory; press \"Aight\" to close this window.");
		auto theLabel = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");

		// copied a bit from louis ck jr
		theLabel->setScale(.35f);
		theLabel->limitLabelWidth(300.f, .35f, .25f); // added by Ery. max width is 346.f

		auto theLabelAsAButton = CCMenuItemSpriteExtra::create(theLabel, this, menu_selector(MyButtonSettingNodeV3::onConfigDirButton));

		theLabelAsAButton->setPositionX(0);

		theMenu->addChild(theLabelAsAButton);
		theMenu->setPosition(width / 2, 20.f);

		this->addChild(theMenu);

		return true;
	}
};