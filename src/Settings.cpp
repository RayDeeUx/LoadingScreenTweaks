#include "Settings.hpp"

SettingNodeV3* MyButtonSettingV3::createNode(float width) {
	return MyButtonSettingNodeV3::create();
}

// i had to include this cpp file because C++ is a legendary language!