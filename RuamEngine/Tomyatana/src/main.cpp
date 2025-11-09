#include "AudioSystem.hpp"

#include "Wave.hpp"
#include <iostream>
#include <cstdio>

using namespace AudioSystem;
/*
	Wave wave(m_audio_path.c_str(), true);
	m_source.generate();

	glm::vec3 zero(0, 0, 0);

	m_source.setParam(AL_PITCH, 1);
	m_source.setParam(AL_GAIN, 1);
	m_source.setParam(AL_POSITION, zero);
	m_source.setParam(AL_VELOCITY, zero);
	m_source.setParam(AL_LOOPING, AL_FALSE);


	m_buffer.generate();
	m_buffer.setData(wave.openal_fmt(), reinterpret_cast<char*>(wave.data()), wave.size(), wave.rate());

	m_source.bind(m_buffer);

	m_source.play();
*/

int main(int argc, char* argv[]) {
	Wave wave(argv[1], false);

	std::cerr << "hola\n";
	AudioSystem::init();
	std::cerr << "vamo\n";

	glm::vec3 listPos = {0.0f, 0.0f, 0.0f};

	AL::Listener::setParam(AL_POSITION, listPos);
	AL::Listener::setParam(AL_VELOCITY, listPos);
	std::cerr << "vamo\n";

	AL::Source source;
	source.generate();
	std::cerr << "src gen\n";

	glm::vec3 sourcePos = {0.0f, 0.0f, 0.0f};
	glm::vec3 sourceVel = {0.0f, 0.0f, 0.0f};

	source.setParam(AL_PITCH, 1);
	source.setParam(AL_GAIN, 1);
	source.setParam(AL_POSITION, sourcePos);
	source.setParam(AL_VELOCITY, sourceVel);
	source.setParam(AL_LOOPING, AL_FALSE);
	std::cerr << "src params\n";

	AL::Buffer buf;
	buf.generate();
	std::cerr << "buf gen\n";
	buf.setData(wave.openal_fmt(), reinterpret_cast<char*>(wave.data()), wave.size(), wave.rate());
	std::cerr << "buf data\n";

	source.bind(buf);
	std::cerr << "src bind\n";

	source.play();
	std::cerr << "src play\n";

	double i = 0;
	while (source.state() == AL_PLAYING) {
		i += 0.000001;
		sourceVel.x = std::sin(i/10);
		sourceVel.y = std::tan(i/10);
		sourceVel.z = std::cos(i/10);
		source.setParam(AL_POSITION, sourceVel);
	}
	std::cout << "Clearing\n";

	source.destroy();
	buf.destroy();
	AudioSystem::shutdown();
}
