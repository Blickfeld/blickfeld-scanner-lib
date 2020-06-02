/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#pragma once

#include <vector>
#include <stdint.h>

namespace blickfeld {
namespace protocol {
namespace data {

struct point_t {
	uint32_t id;
	float x;
	float y;
	float z;
	float range;
	uint16_t intensity;
	uint16_t ambient_light_level;
};

struct frame_t {
	uint64_t id;
	std::vector<point_t> points;
	float frame_rate;
	uint64_t start_time_ns;
};

}
}
}
