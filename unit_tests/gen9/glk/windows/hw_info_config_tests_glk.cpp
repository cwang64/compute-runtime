/*
 * Copyright (C) 2018-2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "runtime/os_interface/windows/os_interface.h"
#include "unit_tests/os_interface/windows/hw_info_config_win_tests.h"

using namespace NEO;
using namespace std;

using HwInfoConfigTestWindowsGlk = HwInfoConfigTestWindows;

GLKTEST_F(HwInfoConfigTestWindowsGlk, whenCallAdjustPlatformThenDoNothing) {
    EXPECT_EQ(IGFX_GEMINILAKE, productFamily);
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    outHwInfo = pInHwInfo;
    hwInfoConfig->adjustPlatformForProductFamily(&outHwInfo);

    int ret = memcmp(&outHwInfo.pPlatform, &pInHwInfo.pPlatform, sizeof(PLATFORM));
    EXPECT_EQ(0, ret);
}
