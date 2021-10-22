/*

	Created by William Kalfelz @ Wusik.com (c) 2021
	This file is free to be used with any project.

*/
//
// ------------------------------------------------------------------------------------------------------------------------- //
#pragma once
#include "PluginProcessor.h"
//
// -------------------------------------------------------------------------------------------------------------------------------
class LookAndFeelEx : public LookAndFeel_V4
{
public:
	static const Font getCustomFont()
	{
		static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::lucon_ttf, BinaryData::lucon_ttfSize);
		return Font(typeface);
	}
	//
	static const Font getCustomFontBold()
	{
		static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::lucon_ttf, BinaryData::lucon_ttfSize);
		return Font(typeface);
	}
	//
	Typeface::Ptr getTypefaceForFont(const Font& f) override
	{
		return getCustomFont().getTypeface();
	}
	//
	bool areLinesDrawnForTreeView(TreeView&) override
	{
		return true;
	}
	//
	void drawTreeviewPlusMinusBox(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isOpen, bool isMouseOver) override
	{
		Path p;
		p.addTriangle(0.0f, 0.0f, 1.0f, isOpen ? 0.0f : 0.5f, isOpen ? 0.5f : 0.0f, 1.0f);
		//
		g.setColour(Colours::red.withAlpha(0.66f));
		g.fillPath(p, p.getTransformToScaleToFit(area.reduced(2, area.getHeight() / 4), true));
	}
	//
	Font getPopupMenuFont() override
	{
		return getCustomFont().withHeight(24.0f * ratioValue);
	}
	//
	void drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, const bool isSeparator, const bool isActive, const bool isHighlighted, const bool isTicked, const bool hasSubMenu, const String& text, const String& shortcutKeyText, const Drawable* icon, const Colour* const textColourToUse) override
	{
		Colour textColour = Colours::white.withAlpha(0.90f);
		//
		if (isSeparator)
		{
			auto r = area.reduced(5, 0);
			r.removeFromTop(roundToInt((r.getHeight() * 0.5f) - 0.5f));

			g.setColour(textColour.withAlpha(0.3f));
			g.fillRect(r.removeFromTop(1));
		}
		else
		{
			//Colour textColour = (textColourToUse == nullptr ? textColour : *textColourToUse);

			auto r = area.reduced(1);

			if (isHighlighted && isActive)
			{
				g.setColour(Colour::fromString("FFFFECC1").withAlpha(0.22f));
				g.fillRect(r);
				g.setColour(Colours::white);
			}
			else
			{
				g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
			}

			r.reduce(jmin(5, area.getWidth() / 20), 0);

			auto font = getPopupMenuFont();

			auto maxFontHeight = r.getHeight() / 1.3f;

			if (font.getHeight() > maxFontHeight)
				font.setHeight(maxFontHeight);

			g.setFont(font);

			auto iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();

			if (icon != nullptr)
			{
				icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
				r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
			}
			else if (isTicked)
			{
				auto tick = getTickShape(1.0f);
				g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
			}

			if (hasSubMenu)
			{
				auto arrowH = 0.6f * getPopupMenuFont().getAscent();

				auto x = static_cast<float> (r.removeFromRight((int)arrowH).getX());
				auto halfH = static_cast<float> (r.getCentreY());

				Path path;
				path.startNewSubPath(x, halfH - arrowH * 0.5f);
				path.lineTo(x + arrowH * 0.6f, halfH);
				path.lineTo(x, halfH + arrowH * 0.5f);

				g.strokePath(path, PathStrokeType(2.0f));
			}

			r.removeFromRight(3);
			g.drawFittedText(text, r, Justification::centredLeft, 1);

			if (shortcutKeyText.isNotEmpty())
			{
				auto f2 = font;
				f2.setHeight(f2.getHeight() * 0.75f);
				f2.setHorizontalScale(0.95f);
				g.setFont(f2);

				g.drawText(shortcutKeyText, r, Justification::centredRight, true);
			}
		}
	}
	//
	Font getAlertWindowTitleFont() override { return getCustomFont().withHeight(32.0f * ratioValue); }
	Font getAlertWindowMessageFont() override { return getCustomFont().withHeight(20.0f * ratioValue); }
	Font getAlertWindowFont() override { return getCustomFont().withHeight(22.0f * ratioValue); }
	//
	void drawButtonText(Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		Font font = getCustomFont().withHeight(button.getHeight() * 0.62f * ratioValue);
		g.setFont(font);
		g.setColour(Colours::white.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

		if (shouldDrawButtonAsHighlighted) g.setColour(Colour::fromString("FFFFEEEE"));

		const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
		const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

		const int fontHeight = roundToInt(font.getHeight() * 0.52f);
		const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		const int textWidth = button.getWidth() - leftIndent - rightIndent;

		int leJustification = Justification::centred;
		if (button.getConnectedEdgeFlags() == TextButton::ConnectedOnLeft) leJustification = Justification::right;
		if (button.getConnectedEdgeFlags() == TextButton::ConnectedOnRight) leJustification = Justification::left;

		if (textWidth > 0)
			g.drawFittedText(button.getButtonText(),
				leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
				leJustification, 2);
	}
	//
	double ratioValue = 1.0;
};