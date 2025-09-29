#include "Wave.hpp"
#include <stdexcept>
#include "RuamUtils.h"

Wave::Wave(const char* filename, bool to_mono) {
	EASY_FUNCTION("WaveConstructor");
	EASY_BLOCK("ReadWaveFrames");
	m_data = drwav_open_file_and_read_pcm_frames_s16(filename, &m_channels, &m_sample_rate, &m_total_samples, nullptr);
	EASY_END_BLOCK;
	if (!m_data) {
		std::cerr << "Failure to read file " << filename << '\n';
	}
	if (!to_mono) return;
	EASY_BLOCK("Wave2Mono");
	if (m_channels > 2) 
		throw format_error("More than 2 channels is currently unsupported");
	else if (m_channels == 1) return;
	for (size_t i = 0; i < m_total_samples; i++) {
		m_data[i] = 0.5 * (m_data[i*2] + m_data[i*2+1]);
	}
	m_total_samples *= 0.5;
	m_channels = 1;
	EASY_END_BLOCK;
}

Wave::Wave(const std::string& filename, bool to_mono) : Wave(filename.c_str(), to_mono) {};

Wave::Wave() {}

Wave::~Wave() {
	drwav_free(m_data, nullptr);
}

drwav_uint64 Wave::size() const {
	return m_total_samples * m_channels * sizeof (int16_t);
}

int16_t* Wave::data() {
	return m_data;
}

uint32_t Wave::rate() const {
	return m_sample_rate;
}

uint32_t Wave::channels() const {
	return m_channels;
}

ALenum Wave::openal_fmt() const {
	bool stereo = m_channels > 1;
	return (stereo) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
}

format_error::format_error(char const* const msg) throw() : std::runtime_error(msg) {}

char const* format_error::what() const throw() {
	return std::exception::what();
}
