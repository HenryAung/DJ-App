#include "MainComponent.h"
using namespace std; 

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(playButton); 
    addAndMakeVisible(stopButton); 
    addAndMakeVisible(volSlider); 

    playButton.addListener(this); 
    stopButton.addListener(this); 
    volSlider.addListener(this); 

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    phase = 0.0; 
    dphase = 0.001; 
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample); 
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample); 

    for (int i = 0; i < bufferToFill.numSamples; ++i) {
        
        double sample = fmod(phase, 0.2); 
        
        leftChannel[i] = sample; 
        rightChannel[i] = sample; 

        phase += dphase; 
    }
    //bufferToFill.clearActiveBufferRegion();

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.


    double rowH = getHeight() / 5;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH); 
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);

}


void MainComponent::buttonClicked(juce::Button* button) {
    
    
    if (button == &playButton) {
        DBG("Play button was clicked"); 
    }

    if (button == &stopButton) {
        DBG("Stop button was clicked"); 
    }
    
}; 

void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volSlider) {
        dphase = volSlider.getValue() * 0.001; 
    }
}