#include "qfluentribbon/ribbon_group.hpp"

#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"

#include <QAction>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QToolButton>
#include <QVariant>
#include <QtGlobal>

namespace qfluentribbon
{
	namespace
	{
		int iconPx(RibbonItemSize size)
		{
			// Logical store metrics: QToolButton scales via devicePixelRatio.
			// scaledMetric here would double-apply Engine dpiScale on high-DPI screens.
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

	void RibbonGroup::addWidget(QWidget* widget)
	{
		if (!widget || m_extraWidgets.contains(widget))
		{
			return;
		}
		widget->setParent(this);
		m_extraWidgets.append(widget);
		widget->show();
		updateGeometry();
		relayoutButtons();
	}

	void RibbonGroup::setItemSize(RibbonItemSize size)
	{
		if (m_simplified && size == RibbonItemSize::Large)
		{
			size = RibbonItemSize::Medium;
		}
		if (m_itemSize == size)
		{
			return;
		}
		m_itemSize = size;
		rebuildButtons();
	}

	void RibbonGroup::setSimplified(bool simplified)
	{
		if (m_simplified == simplified)
		{
			return;
		}
		m_simplified = simplified;
		if (m_simplified && m_itemSize == RibbonItemSize::Large)
		{
			m_itemSize = RibbonItemSize::Medium;
		}
		rebuildButtons();
		updateGeometry();
		update();
	}

	void RibbonGroup::setDialogLauncher(QAction* action)
	{
		if (m_launcherAction == action)
		{
			return;
		}
		m_launcherAction = action;
		ensureLauncherButton();
		relayoutButtons();
		update();
	}

	layout::GroupWidthHints RibbonGroup::widthHints() const
	{
		layout::GroupWidthHints hints;
		const int pad = qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6);
		const int n = qMax(0, m_actions.size());
		const int launch = launcherReserve();
		int extra = 0;
		for (QWidget* w : m_extraWidgets)
		{
			extra += w->sizeHint().width() + 4;
		}
		if (n == 0 && extra == 0)
		{
			hints.large = hints.medium = hints.small = pad * 2 + 48 + launch;
			return hints;
		}
		const int baseCount = qMax(1, n);
		hints.large = pad * 2 + baseCount * (iconPx(RibbonItemSize::Large) + 28) + launch + extra;
		hints.medium = pad * 2 + baseCount * 72 + launch + extra;
		hints.small = pad * 2 + baseCount * 36 + launch + extra;
		hints.large = qMax(hints.large, 64 + launch + extra);
		hints.medium = qMax(hints.medium, 48 + launch + extra);
		hints.small = qMax(hints.small, 36 + launch + extra);
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
		if (m_launcherButton)
		{
			const QColor fg =
				qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg.secondary"), palette().placeholderText().color());
			QPalette pal = m_launcherButton->palette();
			pal.setColor(QPalette::ButtonText, fg);
			m_launcherButton->setPalette(pal);
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
		const int borderW = qMax(1, qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));

		if (titleH > 0)
		{
			const int launch = launcherReserve();
			p.setPen(fg);
			p.drawText(QRect(0, height() - titleH, width() - launch, titleH), Qt::AlignCenter, m_title);
		}
		p.fillRect(QRect(width() - borderW, 4, borderW, height() - qMax(titleH, 8) - 4), border);
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
			// Zero-width tip enables QEvent::ToolTip so ScreenTipFilter can run.
			button->setToolTip(QStringLiteral("\u200B"));
			applyButtonStyle(button);
			button->show();
			m_buttons.append(button);
		}
		ensureLauncherButton();
		updateGeometry();
		relayoutButtons();
		update();
	}

	void RibbonGroup::ensureLauncherButton()
	{
		if (!m_launcherAction)
		{
			if (m_launcherButton)
			{
				delete m_launcherButton;
				m_launcherButton = nullptr;
			}
			return;
		}
		if (!m_launcherButton)
		{
			m_launcherButton = new QToolButton(this);
			m_launcherButton->setObjectName(QStringLiteral("qfr.RibbonGroup.launcher"));
			m_launcherButton->setAutoRaise(true);
			m_launcherButton->setFocusPolicy(Qt::TabFocus);
			m_launcherButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
			m_launcherButton->setText(QStringLiteral("\u231F"));
			m_launcherButton->setToolTip(QStringLiteral("\u200B"));
			connect(m_launcherButton, &QToolButton::clicked, this,
					[this]()
					{
						if (m_launcherAction)
						{
							m_launcherAction->trigger();
						}
						emit dialogLauncherClicked();
					});
		}
		m_launcherButton->setDefaultAction(nullptr);
		m_launcherButton->setText(QStringLiteral("\u231F"));
		m_launcherButton->setProperty("qfr.launcherAction", QVariant::fromValue(m_launcherAction));
		m_launcherButton->setVisible(!m_simplified);
		m_launcherButton->setEnabled(m_launcherAction->isEnabled());
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
		if (m_simplified)
		{
			return 0;
		}
		return qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("group.titleHeight"), 18);
	}

	int RibbonGroup::contentHeight() const
	{
		const QString key = m_simplified ? QStringLiteral("group.height.simplified") : QStringLiteral("group.height");
		const int fallback = m_simplified ? 40 : 88;
		const int groupH = qtheme::api::scaledMetric(QStringLiteral("ribbon"), key, fallback);
		return qMax(24, groupH - titleBandHeight());
	}

	int RibbonGroup::launcherReserve() const
	{
		return (m_launcherAction && !m_simplified) ? qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("icon.small"), 16) : 0;
	}

	void RibbonGroup::relayoutButtons()
	{
		const int pad = qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6);
		const int titleH = titleBandHeight();
		const int areaH = qMax(1, height() - titleH - (m_simplified ? 0 : pad));
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
			const int y = (m_simplified ? 0 : pad) + (areaH - bh) / 2;
			button->setGeometry(x, y, bw, bh);
			x += bw + 4;
		}

		for (QWidget* extra : m_extraWidgets)
		{
			const QSize hint = extra->sizeHint();
			const int ew = hint.width();
			const int eh = qMin(areaH, hint.height());
			const int y = (m_simplified ? 0 : pad) + (areaH - eh) / 2;
			extra->setGeometry(x, y, ew, eh);
			x += ew + 4;
		}

		if (m_launcherButton && m_launcherButton->isVisible())
		{
			const int side = 14;
			m_launcherButton->setGeometry(width() - side - 2, height() - titleH + (titleH - side) / 2, side, side);
			m_launcherButton->raise();
		}
	}
} // namespace qfluentribbon
