#include "qfluentribbon/theme_bridge.hpp"

#include "qtheme/api.hpp"
#include "qtheme/engine.hpp"
#include "qtheme/store.hpp"

#include <QColor>
#include <QString>

namespace qfluentribbon
{
	namespace
	{
		QColor storeColor(const qtheme::ThemeStore* store, const QString& group, const QString& role, const QColor& fallback)
		{
			const qtheme::ColorValue cv = store->color(group, role, fallback);
			return cv.ok ? cv.value : fallback;
		}
	} // namespace

	ThemeBridge::ThemeBridge(QObject* parent)
		: QObject(parent)
	{
	}

	void ThemeBridge::bind(qtheme::Engine* engine)
	{
		if (m_engine == engine)
		{
			return;
		}
		if (m_engine)
		{
			disconnect(m_engine, nullptr, this, nullptr);
		}
		m_engine = engine;
		qtheme::api::bind(engine);
		if (!m_engine)
		{
			return;
		}
		connect(m_engine, &qtheme::Engine::skinChanged, this, &ThemeBridge::onSkinChanged);
		connect(m_engine, &qtheme::Engine::accentChanged, this, &ThemeBridge::onAccentChanged);
		connect(m_engine, &qtheme::Engine::colorSchemeChanged, this, &ThemeBridge::onColorSchemeChanged);
		ensureRibbonTokens();
	}

	void ThemeBridge::ensureRibbonTokens()
	{
		if (!m_engine || !m_engine->store())
		{
			return;
		}
		seedDefaults(m_engine->store());
		emit ribbonTokensChanged();
	}

	void ThemeBridge::onSkinChanged(const QString& previous, const QString& current)
	{
		Q_UNUSED(previous);
		Q_UNUSED(current);
		ensureRibbonTokens();
	}

	void ThemeBridge::onAccentChanged(const QColor& accent)
	{
		Q_UNUSED(accent);
		ensureRibbonTokens();
	}

	void ThemeBridge::onColorSchemeChanged()
	{
		ensureRibbonTokens();
	}

	void ThemeBridge::seedDefaults(qtheme::ThemeStore* store)
	{
		store->beginUpdate();

		if (!store->hasMetric(QStringLiteral("ribbon"), QStringLiteral("bar.height")))
		{
			store->setMetric(QStringLiteral("ribbon"), QStringLiteral("bar.height"), 120);
		}
		if (!store->hasMetric(QStringLiteral("ribbon"), QStringLiteral("tab.height")))
		{
			store->setMetric(QStringLiteral("ribbon"), QStringLiteral("tab.height"), 32);
		}
		if (!store->hasMetric(QStringLiteral("ribbon"), QStringLiteral("group.height")))
		{
			store->setMetric(QStringLiteral("ribbon"), QStringLiteral("group.height"), 88);
		}
		if (!store->hasMetric(QStringLiteral("ribbon"), QStringLiteral("group.padding")))
		{
			store->setMetric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6);
		}
		if (!store->hasMetric(QStringLiteral("ribbon"), QStringLiteral("border.width")))
		{
			store->setMetric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1);
		}

		// Always refresh chrome colors from the active palette so light/dark/hc switches stay coherent.
		const QColor window = storeColor(store, QStringLiteral("palette"), QStringLiteral("window"), QColor(QStringLiteral("#F3F3F3")));
		const QColor surface = storeColor(store, QStringLiteral("palette"), QStringLiteral("surface"), QColor(QStringLiteral("#FFFFFF")));
		const QColor stroke = storeColor(store, QStringLiteral("palette"), QStringLiteral("stroke"), QColor(QStringLiteral("#D1D1D1")));
		const QColor text = storeColor(store, QStringLiteral("palette"), QStringLiteral("text"), QColor(QStringLiteral("#1A1A1A")));
		const QColor accent = storeColor(store, QStringLiteral("palette"), QStringLiteral("accent"), QColor(QStringLiteral("#0078D4")));
		const QColor tertiary =
			storeColor(store, QStringLiteral("palette"), QStringLiteral("text.tertiary"), QColor(QStringLiteral("#8D8D8D")));

		store->setColor(QStringLiteral("ribbon"), QStringLiteral("bg"), window);
		store->setColor(QStringLiteral("ribbon"), QStringLiteral("tab.row.bg"), window);
		store->setColor(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), surface);
		store->setColor(QStringLiteral("ribbon"), QStringLiteral("border"), stroke);
		store->setColor(QStringLiteral("ribbon"), QStringLiteral("fg"), text);
		store->setColor(QStringLiteral("ribbon"), QStringLiteral("fg.secondary"), tertiary);
		store->setColor(QStringLiteral("ribbon"), QStringLiteral("accent"), accent);

		store->endUpdate();
	}
} // namespace qfluentribbon
