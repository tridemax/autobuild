#include "platform.h"
#include "AutoBuilder.h"


namespace AutoBuild
{
	//-------------------------------------------------------------------------------------------------
	AutoBuilder::AudioConverter()
	{
	}

	//-------------------------------------------------------------------------------------------------
	AudioConverter::~AudioConverter()
	{
	}

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter::DecodeAudio(Auxiliary::IMemoryStream& inputStream, AudioFormat audioFormat, AudioCodecs::AudioFile& decodedAudio)
	{
		m_lastError.clear();

		switch (audioFormat)
		{
		case AudioFormat::AudioWave: // Wave with RIFF header
			{
				AudioCodecs::WaveDecoder waveDecoder;

				if (!waveDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Wave in Riff";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioFlac: // FLAC with FLAC header
			{
				AudioCodecs::FlacDecoder flacDecoder;

				if (!flacDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Flac";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioMpeg: // MPEG layer-3 with id3v2 header
			if (!AudioCodecs::MpegDecoder::DecodeStream(inputStream, decodedAudio))
			{
				assert(false);
				return false;
			}
			break;

		case AudioFormat::AudioOpus: // Opus audio in Ogg container
			{
				AudioCodecs::OpusDecoder opusDecoder;

				if (!opusDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Opus in Ogg";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioVorbis: // Vorbis audio in Ogg container
			{
				AudioCodecs::VorbisDecoder vorbisDecoder;

				if (!vorbisDecoder.DecodeStream(inputStream, decodedAudio))
				{
					m_lastError = "Failed to decode input data as Vorbis in Ogg";
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioUnknown: // Unknown audio format
			{
				// Test for opus stream
				AudioCodecs::OpusDecoder opusDecoder;

				if (opusDecoder.TestOpen(inputStream))
				{
					if (!opusDecoder.DecodeStream(inputStream, decodedAudio))
					{
						assert(false);
						return false;
					}
					return true;
				}

				// Test for vorbis stream
				AudioCodecs::VorbisDecoder vorbisDecoder;

				if (vorbisDecoder.TestStream(inputStream))
				{
					if (!vorbisDecoder.DecodeStream(inputStream, decodedAudio))
					{
						assert(false);
						return false;
					}
					return true;
				}

				// Unsupported ogg stream
				assert(false);
				return false;
			}
			break;
		}

		// Test decoded audio for consistency
		if (!decodedAudio.TestConsistency())
		{
			assert(false);
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	bool AudioConverter::EncodeAudio(AudioCodecs::AudioFile& inputAudio, Auxiliary::IStream& outputStream, AudioFormat audioFormat)
	{
		m_lastError.clear();

		switch (audioFormat)
		{
		case AudioFormat::AudioWave: // Wave with RIFF header
			{
				AudioCodecs::WaveEncoder waveEncoder;

				if (!waveEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioFlac: // FLAC with FLAC header
			{
				AudioCodecs::FlacEncoder flacEncoder;

				if (!flacEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioMpeg: // MPEG layer-3 with id3v2 header
			{
				AudioCodecs::MpegEncoder mpegEncoder;

				if (!mpegEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioOpus: // Opus audio in Ogg container
			{
				AudioCodecs::OpusEncoder opusEncoder;

				if (!opusEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		case AudioFormat::AudioVorbis: // Vorbis audio in Ogg container
			{
				AudioCodecs::VorbisEncoder vorbisEncoder;

				if (!vorbisEncoder.EncodeSamples(inputAudio, outputStream))
				{
					assert(false);
					return false;
				}
			}
			break;

		default: // Unknown audio format
//			Logging::Error(loggingEvent, "Unsupported audio format.");
			assert(false);
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------------------------------
	const std::string& AudioConverter::LastError()
	{
		return m_lastError;
	}
}
