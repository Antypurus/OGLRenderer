/* date = August 4th 2020 0:38 am */
#pragma once
#include <cstdint>
#include "../Transform.h"
#include <vector>

struct Keyframe
{
	uint32_t start_time = 0;
	uint32_t delta_t = 0;
	Transform transform_delta;
	
	Keyframe() = default;
	Keyframe(const uint32_t& delta_t, const Transform& transform_delta);
};

struct Animation
{
	uint32_t playback_head = 0;
	uint32_t current_keyframe_index =0;
	std::vector<Keyframe> keyframes;
	Transform current_transform;
	
	public:
	Animation() = default;
	void AddKeyframe(const Keyframe& keyframe);
	void Play();
	
	static Transform Interpolate(const Keyframe& start_keyframe, const Keyframe& end_keyframe, uint32_t playback_head);
};