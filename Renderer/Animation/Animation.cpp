#include "Animation.h"
#include <thread>
#include <chrono>

Keyframe::Keyframe(const uint64_t& delta_t, const Transform& transform_delta)
:delta_t(delta_t),transform_delta(transform_delta)
{
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	this->keyframes.push_back(keyframe);
	this->duration += keyframe.delta_t;
	if(this->keyframes.size() == 1)
	{
		// NOTE(Tiago): fist keyframe added. Start time = 0
		this->keyframes[0].start_time = 0;
	}else
	{
		// NOTE(Tiago): not the first keyframe. Start time = Start Time of Previous Keyframe + previous keyframe duration
		const size_t keyframe_index = this->keyframes.size() - 1;
		const uint64_t start_time = this->keyframes[keyframe_index-1].start_time + this->keyframes[keyframe_index-1].delta_t; 
		this->keyframes[keyframe_index].start_time = start_time;
	}
}

void Animation::Play()
{
	using namespace std::chrono;

	this->playback_head = 0;
	this->current_keyframe_index = 0;

	std::thread play_thread([this](){
			high_resolution_clock::time_point prev_time = high_resolution_clock::now();
			while(this->playback_head <= this->duration)
			{
				high_resolution_clock::time_point curr_time = high_resolution_clock::now();
				uint64_t delta = duration_cast<std::chrono::nanoseconds>(curr_time - prev_time).count();

				//updates playback head
				this->playback_head += delta;

				// checks if animation should end
				if(this->playback_head > this->duration)
				{
					return;
				}

				//updates the current keyframe
				while(this->playback_head > this->keyframes[this->current_keyframe_index].start_time)
				{
					this->current_keyframe_index++;
				}

				//compute the current transform
				this->current_transform = Animation::Interpolate(
					this->keyframes[this->current_keyframe_index],
					this->keyframes[this->current_keyframe_index + 1],
					this->playback_head);

				prev_time = high_resolution_clock::now();
			}
		});
	play_thread.detach();
}

Transform Animation::Interpolate(const Keyframe& start_keyframe, const Keyframe& end_keyframe, uint64_t playback_head)
{
	return {};
}