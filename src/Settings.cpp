#include "Settings.hpp"

SettingNodeV3* MyButtonSettingV3::createNode(float width) {
    return MyButtonSettingV3::create(this, width);
}

// i had to include this cpp file because C++ is a legendary language!