/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    setSize (370, 325);
    //MIAMI VICE
    Colour rosa=Colour(248,144,231);
    Colour azzurro=Colour(11,211,211);
    Colour nero=Colour(0,0,0);
    //MIAMI VICE BRIGHTS
    Colour bluB=Colour(4,93,237);
    Colour violaB=Colour(157,77,255);
    Colour rosaB=Colour(255,77,240);
    
    //pointers
    inputGainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"inputGain",inputGainSlider));
    outputGainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"outputGain",outputGainSlider));
    toneAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"toneControl",toneSlider));
    distortionTypeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState,"distortionType",distortionType));
    switchToneAttachment.reset(new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState,"switchTone",switchTone));

    distortionType.addItem("Hard Clipping", 1);
    distortionType.addItem("Soft Clipping", 2);
    distortionType.addItem("Soft Clipping Exponential", 3);
    distortionType.addItem("Full Wave Rectifier", 4);
    distortionType.addItem("Half Wave Rectifier", 5);
    distortionType.setSelectedId(1);
    distortionType.setLookAndFeel(&comboLook);
    distortionType.setColour(ComboBox::backgroundColourId, nero);
    //distortionType.setColour(ComboBox::arrowColourId, rosa);
    distortionType.setColour(ComboBox::textColourId, rosa);
    addAndMakeVisible(distortionType);
    
    //input Gain
    inputGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    inputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    inputGainSlider.setColour(Slider::thumbColourId, rosa);
    inputGainSlider.setColour(Slider::textBoxOutlineColourId, nero);
    inputGainSlider.setRange(1, 48);
    inputGainSlider.setTextValueSuffix(" dB");
    inputGainSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(inputGainSlider);
    
    //output Gain
    outputGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    outputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    outputGainSlider.setColour(Slider::thumbColourId, rosa);
    outputGainSlider.setColour(Slider::textBoxOutlineColourId, nero);
    //outputGainSlider.setColour(Slider::trackColourId, rosa);
    outputGainSlider.setRange(-48, 12);
    outputGainSlider.setTextValueSuffix(" dB");
    outputGainSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(outputGainSlider);
    
    //tone slider
    //toneSlider.setLookAndFeel(&sliderLook);
    toneSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    toneSlider.setColour(Slider::thumbColourId, rosa);
    toneSlider.setColour(Slider::textBoxOutlineColourId, nero);
    toneSlider.setRange(20, 20000);
    toneSlider.setTextValueSuffix(" Hz");
    toneSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(toneSlider);
    
    //switch tone
    switchTone.setColour(TextButton::buttonColourId, rosaB);
    switchTone.setColour(TextButton::buttonOnColourId, violaB);
    switchTone.setColour(TextButton::textColourOnId, Colours::white);
    switchTone.setColour(TextButton::textColourOffId, Colours::white);
    switchTone.setClickingTogglesState(true);
    switchTone.setSize(60, 30);
    switchTone.addListener(this);
    switchTone.setLookAndFeel(&buttonLook);
    addAndMakeVisible(switchTone);
    
    if(*valueTreeState.getRawParameterValue("switchTone")==0)
        switchTone.setButtonText("Before Distortion");
    if(*valueTreeState.getRawParameterValue("switchTone")==1)
        switchTone.setButtonText("After Distortion");
    
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
    inputGainAttachment.reset();
    outputGainAttachment.reset();
    toneAttachment.reset();
    distortionTypeAttachment.reset();
    distortionType.setLookAndFeel(nullptr);
    inputGainSlider.setLookAndFeel(nullptr);
    switchTone.setLookAndFeel(nullptr);
    switchTone.removeListener(this);
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (Graphics& g)
{
    Rectangle<int> titleArea (0,0,getWidth(),50);
    
    Font titleFont("Rage Italic","Medium",45.0f);
    Font subTitleFont("Montserrat","Regular",14.0f);

    //MIAMI VICE
    Colour rosa=Colour(248,144,231);
    Colour azzurro=Colour(11,211,211);
    Colour nero=Colour(0,0,0);
    
    Rectangle<int> inputGrect;
    Rectangle<int> outputGrect;
    Rectangle<int> toneRect;
    //Rectangle<int> typeRect;
    Rectangle<int> inputGtitle;
    Rectangle<int> outputGtitle;
    Rectangle<int> toneTitle;
    
    inputGrect.setBounds(5, 80, 100, 240);
    outputGrect.setBounds(265, 80, 100, 240);
    toneRect.setBounds(110, 80, 150, 240);
    //typeRect.setBounds(5, 55, 360, 20);
    
    inputGtitle=inputGrect.removeFromTop(20);
    outputGtitle=outputGrect.removeFromTop(20);
    toneTitle=toneRect.removeFromTop(20);
    
    //BRIGHTS
    /*
    //azzurro
    g.setColour(azzurroB);
    g.fillAll();
    
    //nero
    g.setColour(nero);
    g.fillRect(titleArea);
    //g.fillRect(5, 55, 360, 30);
    g.fillRect(inputGrect);
    g.fillRect(outputGrect);
    g.fillRect(toneRect);
    g.fillRect(inputGtitle);
    g.fillRect(outputGtitle);
    g.fillRect(toneTitle);
    
    //azzurro
    g.setColour(azzurroB);
    
    //rosa
    g.setColour(rosaB);
    g.setFont(titleFont);
    titleArea.removeFromLeft(5);
    g.drawText("Distortion", titleArea, Justification::left);
    */

    //MIAMI VICE
    //azzurro
    g.setColour(azzurro);
    g.fillAll();
    
    //nero
    g.setColour(nero);
    g.fillRect(titleArea);
    //g.fillRect(5, 55, 360, 30);
    g.fillRect(inputGrect);
    g.fillRect(outputGrect);
    g.fillRect(toneRect);
    g.drawRect(inputGtitle);
    g.drawRect(outputGtitle);
    g.drawRect(toneTitle);
    
    //azzurro
    g.setColour(azzurro);
    
    //rosa
    g.setColour(rosa);
    g.setFont(titleFont);
    titleArea.removeFromLeft(5);
    g.drawText("Distortion", titleArea, Justification::left);
    
    g.setColour(nero);
    g.setFont(subTitleFont);
    g.drawText("input Gain", inputGtitle, Justification::centred);
    g.drawText("cutoff", toneTitle, Justification::centred);
    g.drawText("output Gain", outputGtitle, Justification::centred);
}

void DistortionAudioProcessorEditor::resized()
{
    
    Rectangle<int> area=getLocalBounds();
    Rectangle<int> titleArea;
    
    titleArea=area.removeFromTop(50);
    
    Rectangle<int> distortionTypeArea;
    Rectangle<int> inputGainArea;
    Rectangle<int> toneArea;
    Rectangle<int> switchArea;
    Rectangle<int> outputGainArea;
    
    distortionTypeArea.setBounds(5, 55, 360, 20);
    inputGainArea.setBounds(5, 100, 100, 220);
    switchArea.setBounds(155, 110, 60, 30);
    toneArea.setBounds(110, 150, 150, 170);
    outputGainArea.setBounds(265, 100, 100, 220);
    
    distortionType.setBounds(distortionTypeArea);
    inputGainSlider.setBounds(inputGainArea);
    switchTone.setBounds(switchArea);
    toneSlider.setBounds(toneArea);
    outputGainSlider.setBounds(outputGainArea);
}

void DistortionAudioProcessorEditor::buttonClicked(Button* button)
{
    if (switchTone.getToggleState())
    {
        switchTone.setButtonText("After Distortion");
    }
    else
    {
        switchTone.setButtonText("Before Distortion");
    }
        
}
