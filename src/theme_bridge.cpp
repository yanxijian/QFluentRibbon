#include "qfluentribbon/theme_bridge.hpp"

#include "qfluentribbon/ribbon_tokens.hpp"

namespace qfluentribbon
{
	ThemeBridge::ThemeBridge(QObject* parent)
		: QObject(parent)
	{
	}

	void ThemeBridge::ensureRibbonTokens(const QColor& window, const QColor& surface, const QColor& stroke, const QColor& text,
										 const QColor& accent, const QColor& tertiary, const QColor& accentText)
	{
		tokens::seedDefaults(window, surface, stroke, text, accent, tertiary, accentText);
		emit ribbonTokensChanged();
	}

	void ThemeBridge::notifyTokensChanged()
	{
		emit ribbonTokensChanged();
	}
} // namespace qfluentribbon
