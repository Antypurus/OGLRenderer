/* date = August 4th 2020 0:38 am */
#pragma once
#include <cstdint>
#include "../Transform.h"
#include <vector>
#include <functional>

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
	uint64_t current_keyframe_index = 0;
	uint64_t duration = 0;
	std::vector<Keyframe> keyframes;
	Transform current_transform;
	bool animating = false;
	std::vector<std::function<void()>> animation_end_callbacks;
private:
	uint64_t prev_time;
	bool should_stop = false;
	bool paused = false;

public:
	Animation();
	~Animation();
	void AddKeyframe(const Keyframe& keyframe);
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void PauseResumeToggle();
	void AddAnimationEndCallback(const std::function<void()>& callback);
	void Update();

	static Transform Interpolate(const Keyframe& start_keyframe, const Keyframe& end_keyframe, uint64_t playback_head);

private:
	void ExecuteCallbacks();
};