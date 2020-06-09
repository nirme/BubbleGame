#include "MusicController.h"


// MusicController::

MusicController::MusicFadeAnimatorListener::MusicFadeAnimatorListener(MusicController *_owner, float _maxVolume) :
	core::Animator::Listener(),
	owner(_owner),
	fadeOutPlayer(nullptr),
	fadeInPlayer(nullptr),
	maxVolume(_maxVolume)
{};


MusicController::MusicFadeAnimatorListener::~MusicFadeAnimatorListener()
{};


void MusicController::MusicFadeAnimatorListener::progress(float _value)
{
	if (_value < 1.0f)
	{
		if (fadeOutPlayer)
			fadeOutPlayer->setVolume(maxVolume * (1.0f - _value));

		if (fadeInPlayer)
			fadeInPlayer->setVolume(maxVolume * (_value));
	}
	else
	{
		if (fadeOutPlayer)
			fadeOutPlayer->pause();

		if (fadeInPlayer)
			fadeInPlayer->setVolume(maxVolume);

		owner->animationComplete();
	}
};



MusicController::PlayersListener::PlayersListener(MusicController *_owner, SoundSet *_listenerSet) :
		core::SoundPlayer::Listener(),
		owner(_owner),
		listenerSet(_listenerSet)
{};


void MusicController::PlayersListener::onSoundEnd()
{
	if (listenerSet == owner->currentSet)
		owner->playNextSong();
	else
	{
		listenerSet->player->freePlayer();
		listenerSet->player = nullptr;
		listenerSet->currentSound = -1;
	}

};


MusicController::PlayersListener::~PlayersListener()
{};


MusicController::SoundSet::SoundSet(const std::string &_name, MusicController *_owner) : name(_name),
	player(nullptr),
	playerListener(_owner, this),
	currentSound(-1)
{};






void MusicController::fadeMusic(core::SoundPlayer *_fadeOutPlayer, core::SoundPlayer *_fadeInPlayer)
{
	if (++isAnimating == 1)
	{
		fadeController = core::ControllerManager::getSingleton().createFrameTimeController(
				std::static_pointer_cast<core::ControllerValue>(fadeAnimatorControllerValue));
	}
	else
	{
		if (fadeAnimatorListener.fadeOutPlayer &&
			fadeAnimatorListener.fadeOutPlayer != _fadeOutPlayer &&
			fadeAnimatorListener.fadeOutPlayer != _fadeInPlayer)
			fadeAnimatorListener.fadeOutPlayer->pause();

		if (fadeAnimatorListener.fadeInPlayer &&
			fadeAnimatorListener.fadeInPlayer != _fadeOutPlayer &&
			fadeAnimatorListener.fadeInPlayer != _fadeInPlayer)
			fadeAnimatorListener.fadeInPlayer->pause();
	}

	fadeAnimatorControllerValue->getAnimator()->reset();
	fadeAnimatorControllerValue->resetTime();

	fadeAnimatorListener.fadeOutPlayer = _fadeOutPlayer;
	fadeAnimatorListener.fadeInPlayer = _fadeInPlayer;

	fadeAnimatorControllerValue->set(0.0f);
};


void MusicController::animationComplete()
{
	if (--isAnimating == 0)
	{
		core::ControllerManager::getSingleton().removeController(fadeController, true);
		fadeController = nullptr;
	}
};


void MusicController::playNextSong()
{
	if (!currentSet->musicList.size())
		return;

	if (!currentSet->player)
	{
		currentSet->player = soundSystem->getPlayer(core::SoundSystem::MaxSoundPriority, false);
		assert(currentSet->player && "sound system must have an available player for music");
	}
	else if (currentSet->player->getPlayerState() != core::PS_STOPPED)
	{
		currentSet->player->resume();
		return;
	}


	unsigned int nextSoundIndex = std::rand();
	if (currentSet->currentSound < currentSet->musicList.size())
	{
		if (currentSet->musicList.size() == 1)
			nextSoundIndex = 0;
		else
		{
			nextSoundIndex %= currentSet->musicList.size() - 1;
			if (nextSoundIndex == currentSet->currentSound)
				nextSoundIndex = (nextSoundIndex + 1) % currentSet->musicList.size();
		}
	}
	else
		nextSoundIndex %= currentSet->musicList.size();

	currentSet->currentSound = nextSoundIndex;
	currentSet->musicList[nextSoundIndex]->load();
	currentSet->player->playSound(currentSet->musicList[nextSoundIndex], &currentSet->playerListener);
};


MusicController::MusicController(core::SoundSystem *_soundSystem) :
	isAnimating(0),
	fadeAnimatorListener(this, 1.0f),
	fadeAnimatorControllerValue(std::make_shared<core::AnimationControllerValue>(nullptr, 1.0f)),
	fadeController(nullptr),
	soundSystem(_soundSystem),
	currentSet(nullptr)
{
	std::unique_ptr<core::Animator> animator = std::make_unique<core::AnimatorEaseInOutSine>(core::Animator::AM_ONCE, MusicFadeTime);
	animator->registerListener(&fadeAnimatorListener);
	fadeAnimatorControllerValue->setAnimator(animator);
};


void MusicController::registerSound(const std::string &_setName, core::SoundPtr _musicPtr)
{
	SoundSet *usedSet = nullptr;
	auto findRes = soundSetsMap.find(_setName);
	if (findRes != soundSetsMap.end())
		usedSet = &(*findRes).second;
	else
	{
		/*
		usedSet = &(*(soundSetsMap.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(_setName),
				std::forward_as_tuple(_setName, this)).first)).second;
		*/

		// c++17 and up
		usedSet = &(*(soundSetsMap.try_emplace(_setName, _setName, this).first)).second;
	}

	usedSet->musicList.push_back(_musicPtr);
	_musicPtr->load();
};

void MusicController::playSet(const std::string &_setName)
{
	SoundSet *previousSet = currentSet;
	auto findRes = soundSetsMap.find(_setName);
	currentSet = (findRes != soundSetsMap.end()) ? &(*findRes).second : nullptr;

	playNextSong();
	fadeMusic(previousSet ? previousSet->player : nullptr, currentSet ? currentSet->player : nullptr);
};
