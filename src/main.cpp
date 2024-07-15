#include <Geode/modify/LoadingLayer.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

using namespace geode::prelude;

std::vector<std::string> quotes;

$on_mod(Loaded) {
	// code adapted with permission from dialouge handler original author thesillydoggo: https://discord.com/channels/911701438269386882/911702535373475870/1212633554345918514 --erymanthus | raydeeux
	
	auto path3 = (Mod::get()->getConfigDir() / "custom.txt").string();
	if (!std::filesystem::exists(path3)) {
		std::string content = R"(System32
Decrypting MS Paint...
lorem ipsum
each line is a new message for the loading screen)";
		utils::file::writeString(path3, content);
	}
	
	if (Mod::get()->getSettingValue<bool>("customSplashText") && !Mod::get()->getSettingValue<bool>("hideSplashText")) {
		auto path = (Mod::get()->getResourcesDir() / "default.txt").string();
		std::ifstream file(path);
		std::string placeHolder;
		while (std::getline(file, placeHolder)) { quotes.push_back(placeHolder); }
	
		if (Mod::get()->getSettingValue<bool>("stanleyCeleste")) {
			auto pathParableMount = (Mod::get()->getResourcesDir() / "stanleyCeleste.txt").string();
			std::ifstream file(pathParableMount);
			std::string stanCele;
			while (std::getline(file, stanCele)) {
				std::string mySteamWishlist = fmt::format("\"{}\"", stanCele);
				quotes.push_back(mySteamWishlist);
			}
		}
		
		if (Mod::get()->getSettingValue<bool>("custom")) {
			auto pathCustom = (Mod::get()->getConfigDir() / "custom.txt").string();
			std::ifstream file(pathCustom);
			std::string str;
			while (std::getline(file, str)) {
				if (str.starts_with("\"") && str.ends_with("\"")) {
					str = str.replace(0, 1, "\'\'");
				} else if (str.starts_with("\'") && str.ends_with("\'")) {
					str = str.replace(0, 2, "\"");
				}
				if (!Mod::get()->getSavedValue<bool>("noHyphens")) {
					str = fmt::format("- {} -", str);
				}
			} // technically i can write two one-time use boolean variables to allow people to toggle these things on and off as they please without the quotes adding themselves multiple times into the vector, but i'd rather add the "restart required" barrier just to be extra safe
		}
	}
}

std::string grabRandomQuote() {
	std::mt19937 randomSeed(std::random_device{}());
	std::shuffle(quotes.begin(), quotes.end(), randomSeed);
	return quotes.front();
}

class $modify(LoadingLayer) {
	bool init (bool fromReload) {
		if (!LoadingLayer::init(fromReload)) return false;
		if (!(Mod::get()->getSettingValue<bool>("enabled"))) return true;
		getChildByID("text-area")->setVisible(!Mod::get()->getSettingValue<bool>("hideSplashText")); // hide loading screen splash text
		getChildByID("progress-slider")->setVisible(!Mod::get()->getSettingValue<bool>("hideProgressBar")); // hide progress bar
		getChildByID("cocos2d-logo")->setVisible(!Mod::get()->getSettingValue<bool>("hideCocosAndFmod")); // hide cocos
		getChildByID("fmod-logo")->setVisible(!Mod::get()->getSettingValue<bool>("hideCocosAndFmod")); // hide fmod
		if (Mod::get()->getSettingValue<bool>("customSplashText") && !fromReload) {
			if (auto textArea = typeinfo_cast<TextArea*>(getChildByID("text-area"))) {
				if (!textArea->isVisible()) return true;
				std::string theString = grabRandomQuote();
				textArea->setVisible(false);
				auto line = CCLabelBMFont::create(theString.c_str(), "goldFont.fnt");
				line->setPosition({textArea->getPositionX(), textArea->getPositionY()});
				#ifdef GEODE_IS_DESKTOP
				if (!Mod::get()->getSettingValue<bool>("multiline")) {
					line->limitLabelWidth(420.f, textArea->getScale(), .25f);
				} else {
					line->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
					line->setWidth(420.f);
					float desiredScale = 72.f / theString.length();
					if (desiredScale > 1.f) desiredScale = 1.f;
					line->setScale(textArea->getScale() * desiredScale);
				}
				#else
				line->limitLabelWidth(420.f, textArea->getScale(), .25f);
				#endif
				this->addChild(line);
			}
		}
		return true;
	}
};