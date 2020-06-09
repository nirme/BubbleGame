#pragma once

#include <memory>
#include <cstdlib>
#include <atomic>
#include "core/ControllerManager.h"
#include "core/Logger.h"
#include "core/Animator.h"
#include "core/AnimatorInstances.h"
#include "core/SoundSystem.h"
#include "core/SoundPlayer.h"


class MusicController
{
protected:
	static constexpr float MusicFadeTime = 1.2f;

	struct SoundSet;

	class MusicFadeAnimatorListener : public core::Animator::Listener
	{
	protected:
		MusicController *owner;

	public:
		core::SoundPlayer *fadeOutPlayer;
		core::SoundPlayer *fadeInPlayer;

		float maxVolume;

	public:
		MusicFadeAnimatorListener(MusicController *_owner, float _maxVolume = 1.0f);
		virtual ~MusicFadeAnimatorListener();
		virtual void progress(float _value);
	};


	class PlayersListener : public core::SoundPlayer::Listener
	{
	protected:
		MusicController *owner;
		SoundSet *listenerSet;

	public:
		PlayersListener(MusicController *_owner, SoundSet *_listenerSet);
		virtual void onSoundEnd();
		virtual ~PlayersListener();
	};

	struct SoundSet
	{
		std::string name;

		core::SoundPlayer* player;
		PlayersListener playerListener;
		std::vector<core::SoundPtr> musicList;
		unsigned int currentSound;

		SoundSet(const std::string &_name, MusicController *_owner);
	};



	std::atomic<unsigned short> isAnimating;
	MusicFadeAnimatorListener fadeAnimatorListener;
	core::AnimationControllerValuePtr fadeAnimatorControllerValue;
	//std::unique_ptr<core::Animator> fadeAnimatorPtr;
	core::ControllerPtr fadeController;

	core::SoundSystem *soundSystem;

	std::unordered_map<std::string, SoundSet> soundSetsMap;
	SoundSet* currentSet;


	void fadeMusic(core::SoundPlayer *_fadeOutPlayer, core::SoundPlayer *_fadeInPlayer);
	void animationComplete();
	void playNextSong();

public:

	MusicController(core::SoundSystem *_soundSystem);

	void registerSound(const std::string &_setName, core::SoundPtr _musicPtr);
	void playSet(const std::string &_setName);

};