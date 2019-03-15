/*
  ==============================================================================

    ZoomedImageComponent.h
    Created: 12 Mar 2019 2:47:00pm
    Author:  Xingchen Wang

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class ZoomedImageComponent : public ImageComponent
{
public:    
    void clipImage(juce::Rectangle<int> rect)
    {
        auto clip = src.getClippedImage(rect);
        ImageComponent::setImage(clip);
    }
    
    void paint (Graphics& g) override
    {
        ImageComponent::paint(g);
    }
     
    virtual void mouseExit (const MouseEvent& event) override
    {
        mouseReleased = true;
    }
    
    virtual void mouseDrag(const MouseEvent& event) override
    {
        if (delta != 0)
        {
            auto dx = mouseReleased ? event.x : (event.x - mx);
            auto dy = mouseReleased ? event.y : (event.y - my);
            
            float ratio = 1;
            currentClip.setX(currentClip.getX() - dx / ratio < 0 ? 0 : currentClip.getX() - dx / ratio);
            currentClip.setY(currentClip.getY() - dy / ratio < 0 ? 0 : currentClip.getY() - dy / ratio);
            
            if (currentClip.getX() + currentClip.getWidth() > src.getWidth())
            {
                currentClip.setX(src.getWidth() - currentClip.getWidth());
            }
            
            
            if (currentClip.getY() + currentClip.getHeight() > src.getHeight())
            {
                currentClip.setY(src.getHeight() - currentClip.getHeight());
            }
            
            if (isMouseOver())
            {
                mx = event.x;
                my = event.y;
            }
            
            clipImage(currentClip);
        }
        
    }
    
    virtual void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) override
    {
        float maxdy = 0.8;
        if (delta + wheel.deltaY < 0)
        {
            delta = 0;
        }
        else if (delta + wheel.deltaY > maxdy)
        {
            delta = maxdy;
        }
        else
        {
            delta += wheel.deltaY;
        }
        
        if (delta > 0 && delta <= maxdy)
        {
            const auto ratio = 1.0 - delta;
            
            auto cw = src.getWidth();
            auto ch = src.getHeight();
            
            currentClip.setX(delta * 0.45 * cw);
            currentClip.setY(delta * 0.45 * ch);
            currentClip.setWidth(cw * ratio);
            currentClip.setHeight(ch * ratio);
            
            clipImage(currentClip);
        }
        
        if(delta <= 0)
        {
            clipImage(Rectangle<int>(0,0,src.getWidth(),src.getHeight()));
        }
        
        
    }
    
    virtual void mouseDown(const MouseEvent& event) override
    {
        mouseReleased = false;
        mx = event.x;
        my = event.y;
    }
    
    virtual void mouseUp(const MouseEvent& event) override
    {
        mouseReleased = true;
    }
    
    void setImage(Image img)
    {
        src = img.createCopy();
        ImageComponent::setImage(img);
    }
private:
    Image src;
    bool mouseReleased{true};
    float delta{ 0.0 };
    int mx{ 0 };
    int my{ 0 };
    juce::Rectangle<int> currentClip;
};
