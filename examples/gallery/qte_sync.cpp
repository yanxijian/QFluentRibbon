#include "qte_sync.hpp"

#include "qfluentribbon/ribbon_tokens.hpp"
#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"
#include "qtheme/engine.hpp"
#include "qtheme/store.hpp"

#include <QColor>

GalleryQteSync::GalleryQteSync(qtheme::Engine* engine, qfluentribbon::ThemeBridge* bridge, QObject* parent)
	: QObject(parent)
	, m_engine(engine)
	, m_bridge(bridge)
{
	if (!m_engine || !m_bridge)
	{
		return;
	}
	connect(m_engine, &qtheme::Engine::packChanged, this, &GalleryQteSync::syncFromEngine);
	connect(m_engine, &qtheme::Engine::accentChanged, this, &GalleryQteSync::syncFromEngine);
	connect(m_engine, &qtheme::Engine::colorSchemeChanged, this, &GalleryQteSync::syncFromEngine);
	connect(m_engine, &qtheme::Engine::dpiScaleChanged, this, &GalleryQteSync::syncFromEngine);
	syncFromEngine();
}

void GalleryQteSync::syncFromEngine()
{
	if (!m_engine || !m_bridge)
	{
		return;
	}
	qfluentribbon::tokens::setDpiScale(qtheme::api::dpiScale());

	auto pick = [this](const QString& role, const QColor& fallback) -> QColor
	{
		if (qtheme::ThemeStore* store = m_engine->store())
		{
			const qtheme::ColorValue cv = store->color(QStringLiteral("palette"), role, fallback);
			return cv.ok ? cv.value : fallback;
		}
		return fallback;
	};

	m_bridge->ensureRibbonTokens(
		pick(QStringLiteral("window"), QColor(QStringLiteral("#F3F3F3"))),
		pick(QStringLiteral("surface"), QColor(QStringLiteral("#FFFFFF"))),
		pick(QStringLiteral("stroke"), QColor(QStringLiteral("#D1D1D1"))), pick(QStringLiteral("text"), QColor(QStringLiteral("#1A1A1A"))),
		pick(QStringLiteral("accent"), QColor(QStringLiteral("#0078D4"))),
		pick(QStringLiteral("text.tertiary"), QColor(QStringLiteral("#8D8D8D"))), pick(QStringLiteral("accent.text"), QColor(Qt::white)));
}
