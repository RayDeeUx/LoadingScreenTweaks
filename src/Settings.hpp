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
		std::string name = setting->getName();
		m_desc = setting->getDescription();
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