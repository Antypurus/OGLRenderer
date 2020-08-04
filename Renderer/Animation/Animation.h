/* date = August 4th 2020 0:38 am */
#pragma once
#include <cstdint>
#include "../Transform.h"
#include <vector>

struct Keyframe
{
	uint64_t end_time = 0;
	uint64_t delta_t = 0;
	Transform transform_delta;
	
	Keyframe() = default;
	Keyframe(const uint64_t& delta_t, const Transform& transform_delta);
	Keyframe(const Keyframe& other);
	void operator=(const Keyframe& other);
};

struct Animation
{
	uint64_t playback_head = 0;
	uint64_t current_keyframe_index =0;
	uint64_t duration = 0;
	std::vector<Keyframe> keyframes;
	Transform current_transform;
	
	public:
	Animation() = default;
	void AddKeyframe(const Keyframe& keyframe);
	void Play();
	
	static Transform Interpolate(const Keyframe& start_keyframe, const Keyframe& end_keyframe, uint64_t playback_head);
};