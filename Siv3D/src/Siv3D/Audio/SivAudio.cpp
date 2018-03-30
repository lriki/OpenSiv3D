﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2018 Ryo Suzuki
//	Copyright (c) 2016-2018 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include "../Siv3DEngine.hpp"
# include "IAudio.hpp"
# include "../Profiler/IProfiler.hpp"

namespace s3d
{
	template <>
	AssetHandle<Audio::Handle>::~AssetHandle()
	{
		if (!Siv3DEngine::isActive())
		{
			return;
		}

		if (auto p = Siv3DEngine::GetAudio())
		{
			p->release(m_id);
		}
	}

	Audio::Audio()
		: m_handle(std::make_shared<AudioHandle>())
	{

	}

	Audio::Audio(Wave&& wave)
		: m_handle(std::make_shared<AudioHandle>(Siv3DEngine::GetAudio()->create(std::move(wave))))
	{
		ASSET_CREATION();
	}

	Audio::Audio(Wave&& wave, const Optional<AudioLoopTiming>& loop)
		: Audio(std::move(wave))
	{
		if (loop)
		{
			if (loop->endPos)
			{
				setLoop(Arg::loopBegin = loop->beginPos, Arg::loopEnd = loop->endPos);
			}
			else
			{
				setLoop(Arg::loopBegin = loop->beginPos);
			}
		}
	}

	Audio::Audio(Wave&& wave, const Arg::loop_<bool> loop)
		: Audio(std::move(wave))
	{
		if (*loop)
		{
			setLoop(true);
		}
	}

	Audio::Audio(Wave&& wave, const Arg::loopBegin_<uint64> loopBegin)
		: Audio(std::move(wave))
	{
		setLoop(loopBegin);
	}

	Audio::Audio(Wave&& wave, const Arg::loopBegin_<uint64> loopBegin, const Arg::loopEnd_<uint64> loopEnd)
		: Audio(std::move(wave))
	{
		setLoop(loopBegin, loopEnd);
	}

	Audio::Audio(Wave&& wave, const Arg::loopBegin_<SecondsF> loopBegin)
		: Audio(std::move(wave))
	{
		setLoop(loopBegin);
	}

	Audio::Audio(Wave&& wave, const Arg::loopBegin_<SecondsF> loopBegin, const Arg::loopEnd_<SecondsF> loopEnd)
		: Audio(std::move(wave))
	{
		setLoop(loopBegin, loopEnd);
	}

	Audio::Audio(const FilePath& path)
		: Audio(Wave(path))
	{

	}

	Audio::Audio(const FilePath& path, const Optional<AudioLoopTiming>& loop)
		: Audio(path)
	{
		if (loop)
		{
			if (loop->endPos)
			{
				setLoop(Arg::loopBegin = loop->beginPos, Arg::loopEnd = loop->endPos);
			}
			else
			{
				setLoop(Arg::loopBegin = loop->beginPos);
			}
		}
	}

	Audio::Audio(const FilePath& path, const Arg::loop_<bool> loop)
		: Audio(path)
	{
		if (*loop)
		{
			setLoop(true);
		}
	}

	Audio::Audio(const FilePath& path, const Arg::loopBegin_<uint64> loopBegin)
		: Audio(path)
	{
		setLoop(loopBegin);
	}

	Audio::Audio(const FilePath& path, const Arg::loopBegin_<uint64> loopBegin, const Arg::loopEnd_<uint64> loopEnd)
		: Audio(path)
	{
		setLoop(loopBegin, loopEnd);
	}

	Audio::Audio(const FilePath& path, const Arg::loopBegin_<SecondsF> loopBegin)
		: Audio(path)
	{
		setLoop(loopBegin);
	}

	Audio::Audio(const FilePath& path, const Arg::loopBegin_<SecondsF> loopBegin, const Arg::loopEnd_<SecondsF> loopEnd)
		: Audio(path)
	{
		setLoop(loopBegin, loopEnd);
	}

	Audio::~Audio()
	{

	}

	void Audio::release()
	{
		m_handle = std::make_shared<AudioHandle>();
	}

	bool Audio::isEmpty() const
	{
		return m_handle->id().isNullAsset();
	}

	AudioID Audio::id() const
	{
		return m_handle->id();
	}

	bool Audio::operator ==(const Audio& audio) const
	{
		return m_handle->id() == audio.m_handle->id();
	}

	bool Audio::operator !=(const Audio& audio) const
	{
		return m_handle->id() != audio.m_handle->id();
	}

	uint32 Audio::samplingRate() const
	{
		return Siv3DEngine::GetAudio()->samplingRate(m_handle->id());
	}

	size_t Audio::samples() const
	{
		return Siv3DEngine::GetAudio()->samples(m_handle->id());
	}

	void Audio::setLoop(const bool loop)
	{
		Siv3DEngine::GetAudio()->setLoop(
			m_handle->id(),
			loop,
			0,
			loop ? samples() - 1 : 0);
	}

	void Audio::setLoop(const Arg::loopBegin_<uint64> loopBegin)
	{
		assert(*loopBegin < samples());

		const uint64 loopBeginSample = *loopBegin;
		const uint64 loopEndSample = samples() - 1;

		Siv3DEngine::GetAudio()->setLoop(
			m_handle->id(),
			true,
			loopBeginSample,
			loopEndSample);
	}

	void Audio::setLoop(const Arg::loopBegin_<uint64> loopBegin, const Arg::loopEnd_<uint64> loopEnd)
	{
		assert(*loopBegin < samples());
		assert(*loopBegin < *loopEnd);

		const uint64 loopBeginSample = *loopBegin;
		const uint64 loopEndSample = std::min<uint64>(*loopEnd, samples() - 1);

		Siv3DEngine::GetAudio()->setLoop(
			m_handle->id(),
			true,
			loopBeginSample,
			loopEndSample);
	}

	void Audio::setLoop(const Arg::loopBegin_<SecondsF> loopBegin)
	{
		const uint64 loopBeginSample = static_cast<uint64>(samplingRate() * std::max(loopBegin->count(), 0.0));
		const uint64 loopEndSample = samples() - 1;

		assert(loopBeginSample < loopEndSample);

		Siv3DEngine::GetAudio()->setLoop(
			m_handle->id(),
			true,
			loopBeginSample,
			loopEndSample);
	}

	void Audio::setLoop(const Arg::loopBegin_<SecondsF> loopBegin, const Arg::loopEnd_<SecondsF> loopEnd)
	{
		const uint64 loopBeginSample = static_cast<uint64>(samplingRate() * std::max(loopBegin->count(), 0.0));
		const uint64 loopEndSample = std::min<uint64>(static_cast<uint64>(samplingRate() * std::max(loopEnd->count(), 0.0)), samples() - 1);

		assert(loopBeginSample < loopEndSample);

		Siv3DEngine::GetAudio()->setLoop(
			m_handle->id(),
			true,
			loopBeginSample,
			loopEndSample);
	}

	bool Audio::play(const SecondsF& fadeinDuration) const
	{
		return Siv3DEngine::GetAudio()->play(m_handle->id(), fadeinDuration);
	}

	void Audio::pause(const SecondsF& fadeoutDuration) const
	{
		return Siv3DEngine::GetAudio()->pause(m_handle->id(), fadeoutDuration);
	}

	void Audio::stop(const SecondsF& fadeoutDuration) const
	{
		Siv3DEngine::GetAudio()->stop(m_handle->id(), fadeoutDuration);
	}

	void Audio::playOneShot(const double volume, const double pitch) const
	{
		Siv3DEngine::GetAudio()->playOneShot(m_handle->id(), volume, pitch);
	}

	void Audio::stopAllShots() const
	{
		Siv3DEngine::GetAudio()->stopAllShots(m_handle->id());
	}

	bool Audio::isPlaying() const
	{
		return Siv3DEngine::GetAudio()->isPlaying(m_handle->id());
	}

	bool Audio::isPaused() const
	{
		return Siv3DEngine::GetAudio()->isPaused(m_handle->id());
	}

	int64 Audio::posSample() const
	{
		return Siv3DEngine::GetAudio()->posSample(m_handle->id());
	}

	int64 Audio::streamPosSample() const
	{
		return Siv3DEngine::GetAudio()->streamPosSample(m_handle->id());
	}

	double Audio::lengthSec() const
	{
		return static_cast<double>(samples()) / samplingRate();
	}

	const Wave& Audio::getWave() const
	{
		return Siv3DEngine::GetAudio()->getWave(m_handle->id());
	}

	int64 Audio::samplesPlayed() const
	{
		return Siv3DEngine::GetAudio()->samplesPlayed(m_handle->id());
	}

	void Audio::setVolumeLR(const double left, const double right) const
	{
		const std::pair<double, double> volume =
		{
			Clamp(left, 0.0, 1.0),
			Clamp(right, 0.0, 1.0)
		};

		Siv3DEngine::GetAudio()->setVolume(m_handle->id(), volume);
	}

	void Audio::setSpeed(const double speed) const
	{
		Siv3DEngine::GetAudio()->setSpeed(m_handle->id(), speed);
	}

	void Audio::setSpeedBySemitone(const int32 semitone) const
	{
		setSpeed(std::exp2(semitone / 12.0));
	}

	double Audio::getSpeed() const
	{
		return Siv3DEngine::GetAudio()->getSpeed(m_handle->id());
	}

	double Audio::getMinSpeed() const
	{
		return Siv3DEngine::GetAudio()->getMinMaxSpeed(m_handle->id()).first;
	}

	double Audio::getMaxSpeed() const
	{
		return Siv3DEngine::GetAudio()->getMinMaxSpeed(m_handle->id()).second;
	}
}