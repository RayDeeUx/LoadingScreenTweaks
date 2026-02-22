#include <Geode/modify/LoadingLayer.hpp>
#include "Settings.hpp"
#include <random>

using namespace geode::prelude;

std::vector<std::string> quotes;
std::vector<std::string> customQuotes;

void addSettingToQuotes(std::string settingName, bool quotationMarks = true) {
	bool settingEnabled = Mod::get()->getSettingValue<bool>(settingName);
	bool logging = Mod::get()->getSettingValue<bool>("logging");
	if (logging) log::info("{} enabled: {}", settingName, settingEnabled);
	if (!settingEnabled) return;
	auto settingAsFileName = fmt::format("{}.txt", settingName);
	auto filePath = geode::utils::string::pathToString(Mod::get()->getResourcesDir() / settingAsFileName);
	std::ifstream file(filePath);
	std::string placeholderString;
	while (std::getline(file, placeholderString)) {
		std::string forPushingIntoVector = placeholderString;
		if (quotationMarks) forPushingIntoVector = fmt::format("\"{}\"", placeholderString);
		quotes.push_back(forPushingIntoVector);
		if (logging) log::info("added quote from {}: {}", settingName, forPushingIntoVector);
	}
}

$on_mod(Loaded) {
	(void) Mod::get()->registerCustomSettingType("configdir", &MyButtonSettingV3::parse);
	// code adapted with permission from dialouge handler original author thesillydoggo: https://discord.com/channels/911701438269386882/911702535373475870/1212633554345918514

	auto path3 = (Mod::get()->getConfigDir() / "custom.txt").string();
	if (!std::filesystem::exists(path3)) {
		std::string content = R"(System32
Decrypting MS Paint...
lorem ipsum
each line is a new message for the loading screen)";
		(void) utils::file::writeString(path3, content);
	}

	if (Mod::get()->getSettingValue<bool>("customSplashText") && !Mod::get()->getSettingValue<bool>("hideSplashText")) {
		addSettingToQuotes("default", false);
		addSettingToQuotes("stanleyCeleste");
		addSettingToQuotes("snl50");
		addSettingToQuotes("futurama", false);

		if (Mod::get()->getSettingValue<bool>("custom")) {
			auto pathCustom = geode::utils::string::pathToString(Mod::get()->getConfigDir() / "custom.txt");
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
				quotes.push_back(str);
				customQuotes.push_back(str);
			} // technically i can write two one-time use boolean variables to allow people to toggle these things on and off as they please without the quotes adding themselves multiple times into the vector, but i'd rather add the "restart required" barrier just to be extra safe
		}
	}
}

std::string grabRandomQuote(std::vector<std::string> vector) {
	if (vector.empty()) return "";
	static std::mt19937_64 engine(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, vector.size() - 1);
	return vector.at(dist(engine));
}

class $modify(MyLoadingLayer, LoadingLayer) {
	struct Fields {
		CCLabelBMFont* m_loadStepLabel = nullptr;
		CCLabelBMFont* m_loadPercentLabel = nullptr;
	};
	void fakeUpdateFunction(float dt) {
		const Fields* fields = m_fields.self();
		if (fields->m_loadStepLabel) {
			fields->m_loadStepLabel->setString(fmt::format("Load Step: {}/14", m_loadStep).c_str());
		}
		if (fields->m_loadPercentLabel) {
			fields->m_loadPercentLabel->setString(fmt::format("Loaded: {:.2f}%", m_sliderBar->getContentWidth() / 210.f * 100.f).c_str());
		}
	}
	bool init(bool fromReload) {
		if (!LoadingLayer::init(fromReload)) return false;
		if (!Mod::get()->getSettingValue<bool>("enabled")) return true;
		this->schedule(schedule_selector(MyLoadingLayer::fakeUpdateFunction));

		if (CCNode* node = getChildByID("text-area")) node->setVisible(!Mod::get()->getSettingValue<bool>("hideSplashText"));
		if (CCNode* node = getChildByID("progress-slider")) node->setVisible(!Mod::get()->getSettingValue<bool>("hideProgressBar"));
		if (CCNode* node = getChildByID("cocos2d-logo")) node->setVisible(!Mod::get()->getSettingValue<bool>("hideCocosAndFmod"));
		if (CCNode* node = getChildByID("fmod-logo")) node->setVisible(!Mod::get()->getSettingValue<bool>("hideCocosAndFmod"));
		if (CCSprite* sprite = typeinfo_cast<CCSprite*>(getChildByID("bg-texture"))) {
			sprite->setOpacity(Mod::get()->getSettingValue<int64_t>("backgroundOpacity"));
			sprite->setColor(Mod::get()->getSettingValue<ccColor3B>("backgroundColor"));
		}

		Fields* fields = m_fields.self();

		const float loadingProgressTextOffsetX = std::clamp<float>(static_cast<float>(Mod::get()->getSettingValue<double>("loadingProgressTextOffsetX")), 0.f, 100.f);
		const float loadingProgressTextOffsetY = std::clamp<float>(static_cast<float>(Mod::get()->getSettingValue<double>("loadingProgressTextOffsetY")), 0.f, 100.f);

		if (Mod::get()->getSettingValue<bool>("loadStepText")) {
			CCLabelBMFont* loadStepLabel = CCLabelBMFont::create("Load Step: Unknown", "goldFont.fnt");
			fields->m_loadStepLabel = loadStepLabel;
			loadStepLabel->setAnchorPoint({0.f, 0.f});
			loadStepLabel->setPosition({2.5f + loadingProgressTextOffsetX, 2.5f + loadingProgressTextOffsetY});
			loadStepLabel->setScale(.25f);
			loadStepLabel->setID("load-step-label"_spr);
			this->addChild(loadStepLabel);
		}

		if (Mod::get()->getSettingValue<bool>("loadingProgressText")) {
			CCLabelBMFont* loadingPercentLabel = CCLabelBMFont::create("Loaded: Unknown", "goldFont.fnt");
			fields->m_loadPercentLabel = loadingPercentLabel;
			loadingPercentLabel->setAnchorPoint({0.f, 0.f});
			loadingPercentLabel->setPosition({2.5f + loadingProgressTextOffsetX, 2.5f + loadingProgressTextOffsetY});
			loadingPercentLabel->setScale(.25f);
			loadingPercentLabel->setID("loading-percent-label"_spr);
			this->addChild(loadingPercentLabel);
			if (Mod::get()->getSettingValue<bool>("loadStepText") && fields->m_loadStepLabel) {
				fields->m_loadStepLabel->setPositionY(fields->m_loadStepLabel->getPositionY() + 7.5f);
			}
		}

		if (!Mod::get()->getSettingValue<bool>("customSplashText") || fromReload) return true;

		CCNode* textArea = m_textArea ? m_textArea : getChildByID("text-area");
		if (!textArea || !textArea->isVisible()) return true;
		std::string theString = "";
		if (!customQuotes.empty() && Mod::get()->getSettingValue<bool>("customTextsOnly")) theString = grabRandomQuote(customQuotes);
		else theString = grabRandomQuote(quotes);
		if (theString.empty()) return true;

		textArea->setVisible(false);
		std::string desiredFont = "goldFont.fnt";
		auto fontID = Mod::get()->getSettingValue<int64_t>("customFont");
		if (fontID == -2) {
			desiredFont = "chatFont.fnt";
		} else if (fontID == -1) {
			desiredFont = "bigFont.fnt";
		} else if (fontID != 0) {
			desiredFont = fmt::format("gjFont{:02d}.fnt", fontID);
		}
		CCLabelBMFont* line = CCLabelBMFont::create(theString.c_str(), desiredFont.c_str());
		line->setPosition(textArea->getPosition());
		if (fontID != 0 && Mod::get()->getSettingValue<bool>("customFontGoldColor")) line->setColor({254, 207, 6});
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
		line->setID("custom-splash-text"_spr);
		this->addChild(line);
		return true;
	}
};