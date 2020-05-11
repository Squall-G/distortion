/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SliderLookAndFeel     : public LookAndFeel_V4
{
public:
    void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override
    {
        float diameter=jmin(width,height);
        float radius=diameter/2;
        float centreX=x+width/2;
        float centreY=y+height/2;
        float rx=centreX-radius;
        float ry=centreY-radius;
        float angle=rotaryStartAngle+(sliderPos*(rotaryEndAngle-rotaryStartAngle));
        
        Rectangle<float> dialArea(rx,ry,diameter,diameter);
        
        g.setColour(Colours::red);
        g.drawRect(dialArea);
        g.fillEllipse(dialArea);
        
        g.setColour(Colours::green);
        g.fillEllipse(centreX, centreY, 5, 5);
        
        Path dialTick;
        dialTick.addRectangle(-5, -radius, -10, radius*0.33f);
        
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centreX,centreY));
        
        g.setColour(Colours::yellow);
        g.drawEllipse(rx, ry, diameter, diameter, 5.0f);
    }
};
class ButtonLookAndFeel     : public LookAndFeel_V4
{
public:
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override
    {
        auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                          .withMultipliedAlpha      (button.isEnabled() ? 0.9f : 0.5f);

        if (isButtonDown || isMouseOverButton)
            baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f);

        auto flatOnLeft   = button.isConnectedOnLeft();
        auto flatOnRight  = button.isConnectedOnRight();
        auto flatOnTop    = button.isConnectedOnTop();
        auto flatOnBottom = button.isConnectedOnBottom();

        auto width  = button.getWidth()  - 1.0f;
        auto height = button.getHeight() - 1.0f;

        if (width > 0 && height > 0)
        {
            auto cornerSize = jmin (15.0f, jmin (width, height) * 0.45f);
            auto lineThickness = cornerSize    * 0.1f;
            auto halfThickness = lineThickness * 0.5f;

            Path outline;
            outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                         cornerSize, cornerSize,
                                         ! (flatOnLeft  || flatOnTop),
                                         ! (flatOnRight || flatOnTop),
                                         ! (flatOnLeft  || flatOnBottom),
                                         ! (flatOnRight || flatOnBottom));

            auto outlineColour = Colours::black;

            g.setColour (baseColour);
            g.fillPath (outline);

            if (! button.getToggleState())
            {
                g.setColour (outlineColour);
                g.strokePath (outline, PathStrokeType (lineThickness));
            }
        }
    }
    
    Font getButtonFont(Button& button)
    {
        Font comboFont("Montserrat","Regular",14.0f);
        return comboFont;
    }
};
class ComboLookAndFeel     : public LookAndFeel_V4
{
public:
    Font getComboBoxFont(ComboBox& combo)
    {
        Font comboFont("Montserrat","Regular",14.0f);
        return comboFont;
    }
};
class DistortionAudioProcessorEditor  : public AudioProcessorEditor,
                                        public TextButton::Listener
{
public:
    DistortionAudioProcessorEditor (DistortionAudioProcessor&, AudioProcessorValueTreeState&);
    ~DistortionAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> distortionTypeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> switchToneAttachment;

private:
    SliderLookAndFeel sliderLook;
    ButtonLookAndFeel buttonLook;
    ComboLookAndFeel comboLook;
    
    Slider inputGainSlider, outputGainSlider, toneSlider;
    ComboBox distortionType;
    TextButton switchTone;
    
    DistortionAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
};
