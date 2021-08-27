/*
*	@file SoundBuffer.cpp
*	
*	Audio and OpenAL Related things
*
*	Implementation File
*/

#include "SoundBuffer.h"
#include "sndfile.h"
#include <inttypes.h>

#include "utils/al_error_marcro_db.h"
#include "Log.h"

namespace en
{
namespace aux
{
	ALuint LoadSoundEffect(const char* filename)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		/* Open the audio file and check that it's usable. */
		sndfile = sf_open(filename, SFM_READ, &sfinfo);
		if (!sndfile)
		{
			BASE_ERROR("Could not open audio in {0}: {1}\n", filename, sf_strerror(sndfile));
			//fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
			return 0;
		}
		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			BASE_ERROR("Bad sample count in {0} ({1})\n", filename, sfinfo.frames);
			//fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
			sf_close(sndfile);
			return 0;
		}

		/* Get the sound format, and figure out the OpenAL format */
		format = AL_NONE;
		if (sfinfo.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (sfinfo.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = 0x20022;
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = 0x20032;
		}
		if (!format)
		{
			BASE_ERROR("Unsupported channel count: {0}\n", sfinfo.channels);
			//fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
			sf_close(sndfile);
			return 0;
		}

		/* Decode the whole audio file to a buffer. */
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			BASE_ERROR("Failed to read samples in {0} ({1})", filename, num_frames);
			//fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		/* Buffer the audio data into a new buffer object, then free the data and
		 * close the file.
		 */
		buffer = 0;
		ALCall(alGenBuffers(1, &buffer));
		err = alGetError();
		if(!buffer || err != AL_NO_ERROR)
			throw std::exception("Could not generate buffer");
		ALCall(alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate));

		free(membuf);
		sf_close(sndfile);

		/* Check if an error occured, and clean up if so. */
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			ALCall(fprintf(stderr, "OpenAL Error: %s\n", alGetString(err)));
			if (buffer && alIsBuffer(buffer))
				ALCall(alDeleteBuffers(1, &buffer));
			return 0;
		}

		//p_SoundEffectBuffers.push_back(buffer);  // add to the list of known buffers

		return buffer;
	}
}
}