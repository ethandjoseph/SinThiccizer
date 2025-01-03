#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "OvertonePassFilter.h"
#include "JUVerb.h"
#include "ImpulseResponder.h"
#include "Waveshaper.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void setParameters(
		juce::ADSR::Parameters adsrParameters,
		juce::ADSR::Parameters filterAdsrParameters,
		float newSinDelay,
		float juverbSize,
		float juverbDamping,
		float juverbWidth,
		bool juverbFreeze,
		float juverbDry,
		float juverbWet,
		float irDry,
		float irWet,
		float balance,
		float newSinGain,
		float newNoiseGain,
		float synth_gain,
		float thicc_saturation_gain,
		float thicc_gain
		);
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	int getCurrentMidiNoteNumber() const;
	void setIR(juce::File file);
private:
	juce::AudioBuffer<float> synthBuffer;
	juce::AudioBuffer<float> noiseBuffer;
	juce::AudioBuffer<float> oscBuffer;
	juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };
	juce::Random random;
	OvertonePassFilter overtonePassFilter;
	OvertonePassFilter thiccOvertonePassFilter;
	juce::ADSR adsr;
	juce::ADSR sinAdsr;
	juce::ADSR::Parameters adsrParameters;
	juce::ADSR::Parameters sinAdsrParameters;

	juce::AudioBuffer<float> juverbBuffer;
	JUVerb juVerb;

	juce::AudioBuffer<float> irBuffer;
	ImpulseResponder conv;

	float balance = 0.5f;
	float juverbBalanceGain = 0.5f;
	float irBalanceGain = 0.5f;

	float noiseGain = 1.0f;
	float sinGain = 0.8f;
	juce::dsp::Gain<float> oscGain;

	juce::dsp::Gain<float> synthDryGain;

	juce::AudioBuffer<float> thiccBuffer;
	juce::dsp::Gain<float> saturationGain;
	Waveshaper waveshaper;
	juce::dsp::Gain<float> saturationCompensationGain;

	juce::ADSR filterAdsr;
	juce::ADSR::Parameters filterAdsrParameters;

	juce::dsp::Gain<float> thiccGain;

	juce::AudioBuffer<float> sumBuffer;
	
	int currentMidiNoteNumber{ -1 };
	bool isPrepared{ false };
};