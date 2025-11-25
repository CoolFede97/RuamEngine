#include "AudioSource.h"
#include "RuamUtils.h"
#include "Camera.h"

AudioSource::AudioSource(const unsigned int object_id, const std::string& audio) 
	: m_audio_path(audio), Component(object_id) {
	EASY_FUNCTION("AudioSource Constructor")
}

AudioSource::AudioSource(const nlohmann::json& j, const unsigned int obj_id) 
	: AudioSource(obj_id, static_cast<std::string>(j["m_audio_path"])) {}

void AudioSource::start() {
	EASY_FUNCTION("AudioSourceStart");
	try {
		m_source.generate();
	} catch (AudioSystem::AL::al_error e) {
		std::cerr << "Error generating source, " << e.what() << '\n';
	}

	glm::vec3 zero(0.0f, 0.0f, 0.0f);

	try {
		m_source.setParam(AL_PITCH, 1);
		m_source.setParam(AL_GAIN, 1);
		m_source.setParam(AL_POSITION, zero);
		m_source.setParam(AL_VELOCITY, zero);
		m_source.setParam(AL_LOOPING, AL_TRUE);
		m_source.setParam(AL_SOURCE_RELATIVE, AL_TRUE);
	} catch (AudioSystem::AL::al_error e) {
		std::cerr << "Error setting source parameters, " << e.what() << '\n';
	}


	try {
		m_buffer.generate();
	} catch (AudioSystem::AL::al_error e) {
		std::cerr << "Error generating buffer, " << e.what() << '\n';
	}

	load(m_audio_path);

	play();
}

const AudioSystem::AL::Source& AudioSource::source() {
	return m_source;
}

void AudioSource::load(const std::string& path) {
	EASY_FUNCTION("AudioSourceLOAD");
	m_audio_path = path;
	AudioSystem::pool.queue([this]() {
		m_buffer_ready = false;
		m_wave.reset();
		m_wave = std::make_unique<Wave>(m_audio_path.c_str(), true);
		loadBuffer(m_wave);
	});
}

void AudioSource::loadBuffer(std::unique_ptr<Wave>& wave) {
	try {
		m_buffer.setData(m_wave->openal_fmt(), reinterpret_cast<char*>(m_wave->data()), m_wave->size(), m_wave->rate());
		m_source.bind(m_buffer);
		m_buffer_ready = true;
		m_buf_wait.notify_one();
	} catch (AudioSystem::AL::al_error e) {
		std::cerr << "Error setting data, " << e.what() << '\n';
		std::cerr << "Wave:\n";
		std::cerr << "FMT: " << m_wave->openal_fmt() << '\n';
		std::cerr << "Size: " << m_wave->size() << '\n';
		std::cerr << "rate: " << m_wave->rate() << '\n';
	}
}

void AudioSource::play() {
	AudioSystem::pool.queue([this]() {
		EASY_BLOCK("AudioSourcePlay")
		while (!m_buffer_ready) {}
		m_source.play();
		EASY_END_BLOCK
	});
}

void AudioSource::pause() {
	AudioSystem::pool.queue([this]() {
		EASY_BLOCK("AudioSourcePause")
		while (!m_buffer_ready) {}
		m_source.pause();
		EASY_END_BLOCK
	});
}

void AudioSource::stop() {
	AudioSystem::pool.queue([this]() {
		EASY_BLOCK("AudioSourceStop")
		while (!m_buffer_ready) {}
		m_source.stop();
		EASY_END_BLOCK
	});
}

void AudioSource::setLooping(bool state) {
	auto set = (state) ? AL_TRUE : AL_FALSE;
	m_source.setParam(AL_LOOPING, set); // FIX: CRASHES
}

bool AudioSource::isLooping() {
	ALint status = false;
	m_source.get(AL_LOOPING, &status);
	return status;
}

void AudioSource::update() {
	try {
		m_source.setParam(AL_POSITION,
					RuamEngine::Camera::GetMainCamera()->object()->transform().position() - object()->transform().position()
					);
	} catch(AudioSystem::AL::al_error err) {
	}
}

int AudioSource::status() {
	return m_source.state();
}

void AudioSource::setVolume(float vol) {
	if (vol > m_max_volume) {
		m_source.setParam(AL_MAX_GAIN, vol);
		m_max_volume = vol;
	}
	m_source.setParam(AL_GAIN, vol);
}

float AudioSource::volume() {
	m_source.get(AL_GAIN, &m_volume);
	return m_volume;
}

REGISTER_COMPONENT(AudioSource);
