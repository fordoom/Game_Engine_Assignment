#include <ClanLib/sound.h>
#include <list>

using namespace clan;

class SoundSession
{
public:
	SoundSession() : is_null(true), unique_id(++max_unique_id), fade_filter(1.0f)  {}

	int unique_id;
	static int max_unique_id;
	bool is_null;
	SoundBuffer_Session session;
	std::string name;

	EchoFilter echo_filter;
	InverseEchoFilter inverse_echo_filter;
	FadeFilter fade_filter;

};