#include "qfluentribbon/ribbon_group.hpp"

#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"

#include <QAction>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QToolButton>
#include <QtGlobal>

namespace qfluentribbon
{
	namespace
	{
		int iconPx(RibbonItemSize size)
		{
			switch (size)
			{
			case RibbonItemSize::Large:
				return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("icon.large"), 32);
			case RibbonItemSize::Medium:
				return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("icon.medium"), 16);
			case RibbonItemSize::Small:
				return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("icon.small"), 16);
			}
			return 16;
		}
	} // namespace

	RibbonGroup::RibbonGroup(const QString& title, QWidget* parent)
		: QWidget(parent)
		, m_title(title)
	{
		setObjectName(QStringLiteral("qfr.RibbonGroup"));
		setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	}

	void RibbonGroup::setTitle(const QString& title)
	{
		if (m_title == title)
		{
			return;
		}
		m_title = title;
		update();
		updateGeometry();
	}

	void RibbonGroup::setThemeBridge(ThemeBridge* bridge)
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
			connect(m_bridge, &ThemeBridge::ribbonTokensChanged, this, &RibbonGroup::polishFromStore);
		}
		polishFromStore();
	}

	QAction* RibbonGroup::addAction(QAction* action)
	{
		if (!action)
		{
			return nullptr;
		}
		if (!m_actions.contains(action))
		{
			m_actions.append(action);
			rebuildButtons();
		}
		return action;
	}

	QAction* RibbonGroup::addAction(const QString& text)
	{
		auto* action = new QAction(text, this);
		return addAction(action);
	}

	void RibbonGroup::setItemSize(RibbonItemSize size)
	{
		if (m_itemSize == size)
		{
			return;
		}
		m_itemSize = size;
		rebuildButtons();
	}

	layout::GroupWidthHints RibbonGroup::widthHints() const
	{
		layout::GroupWidthHints hints;
		const int pad = qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6);
		const int n = qMax(1, m_actions.size());
		// Approximate footprints used by collapse (keep in sync with rebuildButtons sizing).
		hints.large = pad * 2 + n * (iconPx(RibbonItemSize::Large) + 28);
		hints.medium = pad * 2 + n * 72;
		hints.small = pad * 2 + n * 36;
		hints.large = qMax(hints.large, 64);
		hints.medium = qMax(hints.medium, 48);
		hints.small = qMax(hints.small, 36);
		return hints;
	}

	QSize RibbonGroup::sizeHint() const
	{
		const layout::GroupWidthHints hints = widthHints();
		int w = hints.large;
		switch (m_itemSize)
		{
		case RibbonItemSize::Medium:
			w = hints.medium;
			break;
		case RibbonItemSize::Small:
			w = hints.small;
			break;
		case RibbonItemSize::Large:
		default:
			break;
		}
		return {w, contentHeight() + titleBandHeight()};
	}

	QSize RibbonGroup::minimumSizeHint() const
	{
		return {widthHints().small, sizeHint().height()};
	}

	void RibbonGroup::polishFromStore()
	{
		for (QToolButton* button : m_buttons)
		{
			applyButtonStyle(button);
		}
		updateGeometry();
		update();
		relayoutButtons();
	}

	void RibbonGroup::paintEvent(QPaintEvent* event)
	{
		Q_UNUSED(event);
		QPainter p(this);
		const QColor border = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("border"), palette().mid().color());
		const QColor fg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg.secondary"), palette().placeholderText().color());
		const int titleH = titleBandHeight();
		const int borderW = qMax(1, qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));

		p.setPen(fg);
		p.drawText(QRect(0, height() - titleH, width(), titleH), Qt::AlignCenter, m_title);
		p.fillRect(QRect(width() - borderW, 4, borderW, height() - titleH - 8), border);
	}

	void RibbonGroup::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		relayoutButtons();
	}

	void RibbonGroup::rebuildButtons()
	{
		qDeleteAll(m_buttons);
		m_buttons.clear();

		for (QAction* action : m_actions)
		{
			auto* button = new QToolButton(this);
			button->setDefaultAction(action);
			button->setAutoRaise(true);
			button->setFocusPolicy(Qt::TabFocus);
			applyButtonStyle(button);
			button->show();
			m_buttons.append(button);
		}
		updateGeometry();
		relayoutButtons();
		update();
	}

	void RibbonGroup::applyButtonStyle(QToolButton* button) const
	{
		if (!button)
		{
			return;
		}
		const int px = iconPx(m_itemSize);
		button->setIconSize(QSize(px, px));
		switch (m_itemSize)
		{
		case RibbonItemSize::Large:
			button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
			break;
		case RibbonItemSize::Medium:
			button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			break;
		case RibbonItemSize::Small:
			button->setToolButtonStyle(actionHasIcon(button) ? Qt::ToolButtonIconOnly : Qt::ToolButtonTextOnly);
			break;
		}
	}

	bool RibbonGroup::actionHasIcon(const QToolButton* button) const
	{
		return button && button->defaultAction() && !button->defaultAction()->icon().isNull();
	}

	int RibbonGroup::titleBandHeight() const
	{
		return 18;
	}

	int RibbonGroup::contentHeight() const
	{
		const int groupH = qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("group.height"), 88);
		return qMax(40, groupH - titleBandHeight());
	}

	void RibbonGroup::relayoutButtons()
	{
		const int pad = qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6);
		const int titleH = titleBandHeight();
		const int areaH = qMax(1, height() - titleH - pad);
		int x = pad;
		for (QToolButton* button : m_buttons)
		{
			const QSize hint = button->sizeHint();
			int bw = hint.width();
			int bh = hint.height();
			switch (m_itemSize)
			{
			case RibbonItemSize::Large:
				bw = qMax(bw, iconPx(RibbonItemSize::Large) + 8);
				bh = qMin(areaH, qMax(bh, iconPx(RibbonItemSize::Large) + 20));
				break;
			case RibbonItemSize::Medium:
				bw = qMax(bw, 64);
				bh = qMin(areaH, qMax(bh, 28));
				break;
			case RibbonItemSize::Small:
				bw = qMax(bw, 28);
				bh = qMin(areaH, qMax(bh, 24));
				break;
			}
			const int y = pad + (areaH - bh) / 2;
			button->setGeometry(x, y, bw, bh);
			x += bw + 4;
		}
	}
} // namespace qfluentribbon
