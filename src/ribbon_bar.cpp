#include "qfluentribbon/ribbon_bar.hpp"

#include "qfluentribbon/theme_bridge.hpp"

#include "qtheme/api.hpp"
#include "qtheme/engine.hpp"

#include <QPainter>
#include <QPaintEvent>

namespace qfluentribbon
{
	RibbonBar::RibbonBar(QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(QStringLiteral("qfr.RibbonBar"));
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		m_statusText = QStringLiteral("RibbonBar (M0 placeholder)");
	}

	void RibbonBar::setThemeBridge(ThemeBridge* bridge)
	{
		if (m_bridge == bridge)
		{
			return;
		}
		if (m_bridge)
		{
			disconnect(m_bridge, nullptr, this, nullptr);
		}
		m_bridge = bridge;
		if (m_bridge)
		{
			connect(m_bridge, &ThemeBridge::ribbonTokensChanged, this, &RibbonBar::polishFromStore);
		}
		polishFromStore();
	}

	void RibbonBar::setStatusText(const QString& text)
	{
		if (m_statusText == text)
		{
			return;
		}
		m_statusText = text;
		update();
	}

	QSize RibbonBar::sizeHint() const
	{
		return {640, barHeight()};
	}

	QSize RibbonBar::minimumSizeHint() const
	{
		return {200, barHeight()};
	}

	void RibbonBar::polishFromStore()
	{
		updateGeometry();
		update();
	}

	int RibbonBar::barHeight() const
	{
		return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("bar.height"), 120);
	}

	void RibbonBar::paintEvent(QPaintEvent* event)
	{
		Q_UNUSED(event);
		QPainter p(this);
		p.setRenderHint(QPainter::Antialiasing, false);

		const QColor bg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color());
		const QColor panel = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), palette().base().color());
		const QColor border = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("border"), palette().mid().color());
		const QColor fg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg"), palette().windowText().color());
		const QColor fgSec =
			qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg.secondary"), palette().placeholderText().color());
		const QColor accent = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("accent"), QColor(QStringLiteral("#0078D4")));
		const int tabH = qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("tab.height"), 32);
		const int borderW = qMax(1, qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));

		p.fillRect(rect(), bg);

		const QRect tabRow(0, 0, width(), tabH);
		p.fillRect(tabRow, bg);
		p.setPen(fg);
		p.drawText(tabRow.adjusted(12, 0, -12, 0), Qt::AlignVCenter | Qt::AlignLeft, QStringLiteral("Home   Insert   View"));

		const QRect panelRow(0, tabH, width(), height() - tabH);
		p.fillRect(panelRow, panel);
		p.setPen(fg);
		p.drawText(panelRow.adjusted(12, 8, -12, -8), Qt::AlignLeft | Qt::AlignTop, m_statusText);

		QString skin = QStringLiteral("(no engine)");
		if (m_bridge && m_bridge->engine())
		{
			skin = m_bridge->engine()->currentSkin();
		}
		p.setPen(fgSec);
		p.drawText(panelRow.adjusted(12, 28, -12, -8), Qt::AlignLeft | Qt::AlignTop,
				   QStringLiteral("Skin: %1  ·  colors from ThemeStore (no QSS)").arg(skin));

		p.fillRect(QRect(0, height() - borderW, width(), borderW), border);
		p.fillRect(QRect(12, tabH - 3, 36, 3), accent);
	}
} // namespace qfluentribbon
