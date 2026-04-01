#pragma once

#include <cstdint>
#include <stdexcept>
#include <future>

#include "al-wrapper.h"

#include "dr_wav.h"

class format_error : public std::runtime_error {
public:
	format_error(char const* const msg) throw();
	char const* what() const throw();
};

class Wave {
public:
	Wave(const char* filename, bool to_mono=false);
	Wave(const std::string& filename, bool to_mono=false);
	Wave();
	~Wave();

	ALenum openal_fmt() const;
	int16_t* data();
	drwav_uint64 size() const;
	uint32_t rate() const;
	uint32_t channels() const;

private:
	int16_t* m_data;
	uint32_t m_channels;
	uint32_t m_sample_rate;
	drwav_uint64 m_total_samples;
};
