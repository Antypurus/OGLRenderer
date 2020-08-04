#include "Animation.h"
#include <thread>
#include <chrono>

Keyframe::Keyframe(const uint64_t& delta_t, const Transform& transform_delta)
	:delta_t(delta_t), transform_delta(transform_delta)
{
}

Keyframe::Keyframe(const Keyframe& other)
	: end_time(other.end_time), delta_t(other.delta_t), transform_delta(other.transform_delta)
{
}

void Keyframe::operator=(const Keyframe& other)
{
	this->end_time = other.end_time;
	this->delta_t = other.delta_t;
	this->transform_delta = other.transform_delta;
}

Animation::Animation()
{
	this->current_transform = {
							glm::vec3{0,0,0},
							glm::vec3{0,0,0},
							glm::vec3{0,0,0}
	};
}

Animation::~Animation()
{
	this->Stop();
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	this->keyframes.push_back(keyframe);
	this->duration += keyframe.delta_t;
	if (this->keyframes.size() == 1)
	{
		// NOTE(Tiago): fist keyframe added. Start time = 0
		this->keyframes[0].end_time = keyframe.delta_t;
	}
	else
	{
		// NOTE(Tiago): not the first keyframe. Start time = Start Time of Previous Keyframe + previous keyframe duration
		const size_t keyframe_index = this->keyframes.size() - 1;
		const uint64_t end_time = this->keyframes[keyframe_index - 1].end_time + keyframe.delta_t;
		this->keyframes[keyframe_index].end_time = end_time;
	}
}

void Animation::Play()
{
	using namespace std::chrono;
	this->playback_head = 0;
	this->current_keyframe_index = 0;
	this->current_transform = {
							glm::vec3{0,0,0},
							glm::vec3{0,0,0},
							glm::vec3{0,0,0}
	};
	this->should_stop = false;
	this->paused = false;
	this->animating = true;
	prev_time = time_point_cast<std::chrono::milliseconds>(high_resolution_clock::now()).time_since_epoch().count();
}

void Animation::Stop()
{
	this->paused = false;
	this->should_stop = true;
	this->animating = false;
}

void Animation::Pause()
{
	this->paused = true;
}

void Animation::Resume()
{
	using namespace std::chrono;
	this->paused = false;
	prev_time = time_point_cast<std::chrono::milliseconds>(high_resolution_clock::now()).time_since_epoch().count();
}

void Animation::PauseResumeToggle()
{
	if (this->paused)
	{
		this->Resume();
	}
	else
	{
		this->Pause();
	}
}

void Animation::AddAnimationEndCallback(const std::function<void()>& callback)
{
	this->animation_end_callbacks.push_back(callback);
}

void Animation::Update()
{
	using namespace std::chrono;

	if (this->animating)
	{
		if (this->should_stop)
		{
			this->animating = false;
			return;
		}

		if (paused)
		{
			return;
		}

		uint64_t current_time = time_point_cast<std::chrono::milliseconds>(high_resolution_clock::now()).time_since_epoch().count();

		uint64_t delta = current_time - prev_time;

		//updates playback head
		this->playback_head += delta;

		// checks if animation should end
		if (this->playback_head >= this->duration)
		{
			this->ExecuteCallbacks();
			this->animating = false;
			this->current_transform = this->keyframes[this->keyframes.size() - 1].transform_delta;
			return;
		}

		//updates the current keyframe
		while (this->playback_head > this->keyframes[this->current_keyframe_index].end_time)
		{
			this->current_keyframe_index++;
		}

		//compute the current transform
		if (this->current_keyframe_index > 0)
		{
			this->current_transform = Animation::Interpolate(
				this->keyframes[this->current_keyframe_index - 1],
				this->keyframes[this->current_keyframe_index],
				this->playback_head);
		}
		else {
			this->current_transform = Animation::Interpolate(
				{
					0,
					{
						glm::vec3{0,0,0},
						glm::vec3{0,0,0},
						glm::vec3{0,0,0}
					}
				},
				this->keyframes[this->current_keyframe_index],
				this->playback_head);
		}

		prev_time = current_time;
	}
}

Transform Animation::Interpolate(const Keyframe& start_keyframe, const Keyframe& end_keyframe, uint64_t playback_head)
{
	Transform start = start_keyframe.transform_delta;
	Transform end = end_keyframe.transform_delta;
	uint64_t len = end_keyframe.end_time - start_keyframe.end_time;
	uint64_t head = playback_head - start_keyframe.end_time;
	float index_delta = (float)head / (float)len;

	if (index_delta != 0.0)
	{
		glm::vec3 position = {
			start.position.x + (end.position.x - start.position.x) * index_delta,
			start.position.y + (end.position.y - start.position.y) * index_delta,
			start.position.z + (end.position.z - start.position.z) * index_delta
		};

		glm::vec3 rotation = {
			start.rotation.x + (end.rotation.x - start.rotation.x) * index_delta,
			start.rotation.y + (end.rotation.y - start.rotation.y) * index_delta,
			start.rotation.z + (end.rotation.z - start.rotation.z) * index_delta
		};

		glm::vec3 scale = {
			start.scale.x + (end.scale.x - start.scale.x) * index_delta,
			start.scale.y + (end.scale.y - start.scale.y) * index_delta,
			start.scale.z + (end.scale.z - start.scale.z) * index_delta
		};

		return { position,scale,rotation };
	}
	else
	{
		return {};
	}
}

void Animation::ExecuteCallbacks()
{
	for (auto& callback : animation_end_callbacks)
	{
		callback();
	}
}
