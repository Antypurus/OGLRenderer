#include "Animation.h"

Keyframe::Keyframe(const uint32_t& delta_t, const Transform& transform_delta)
:delta_t(delta_t),transform_delta(transform_delta)
{
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	this->keyframes.push_back(keyframe);
	if(this->keyframes.size() == 1)
	{
		// NOTE(Tiago): fist keyframe added. Start time = 0
		this->keyframes[0].start_time = 0;
	}else
	{
		// NOTE(Tiago): not the first keyframe. Start time = Start Time of Previous Keyframe + previous keyframe duration
		const uint32_t keyframe_index = this->keyframes.size() - 1;
		const uint32_t start_time = this->keyframes[keyframe_index-1].start_time + this->keyframes[keyframe_index-1].delta_t; 
		this->keyframes[keyframe_index].start_time = start_time;
	}
}

void Animation::Play()
{
	this->playback_head = 0;
	this->current_keyframe_index = 0;
}

Transform Animation::Interpolate(const Keyframe& start_keyframe, const Keyframe& end_keyframe, uint32_t playback_head)
{
	return {};
}