#include "qfluentribbon/ribbon_bar.hpp"

#include "qfluentribbon/quick_access_bar.hpp"
#include "qfluentribbon/ribbon_tab.hpp"
#include "qfluentribbon/screen_tip.hpp"
#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"
#include "qtheme/engine.hpp"
#include "qtheme/store.hpp"

#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QTabBar>

namespace qfluentribbon
{
	RibbonBar::RibbonBar(QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(QStringLiteral("qfr.RibbonBar"));
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		m_qat = new QuickAccessBar(this);
		m_qat->setObjectName(QStringLiteral("qfr.RibbonBar.qat"));

		m_tabBar = new QTabBar(this);
		m_tabBar->setObjectName(QStringLiteral("qfr.RibbonBar.tabs"));
		m_tabBar->setExpanding(false);
		m_tabBar->setDrawBase(false);
		m_tabBar->setUsesScrollButtons(true);

		m_stack = new QStackedWidget(this);
		m_stack->setObjectName(QStringLiteral("qfr.RibbonBar.stack"));

		connect(m_tabBar, &QTabBar::currentChanged, this,
				[this](int index)
				{
					if (m_stack)
					{
						m_stack->setCurrentIndex(index);
					}
					emit currentChanged(index);
				});
		connect(m_qat, &QuickAccessBar::actionsChanged, this,
				[this]()
				{
					rebuildChrome();
					update();
				});

		ScreenTip::install(this);
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
		if (m_qat)
		{
			m_qat->setThemeBridge(bridge);
		}
		for (RibbonTab* tab : m_tabs)
		{
			tab->setThemeBridge(bridge);
		}
		polishFromStore();
	}

	RibbonTab* RibbonBar::addTab(const QString& title)
	{
		auto* tab = new RibbonTab(title, m_stack);
		tab->setThemeBridge(m_bridge);
		tab->setSimplified(m_simplified);
		m_tabs.append(tab);
		m_stack->addWidget(tab);
		m_tabBar->addTab(title);
		if (m_tabs.size() == 1)
		{
			setCurrentIndex(0);
		}
		rebuildChrome();
		return tab;
	}

	RibbonTab* RibbonBar::tabAt(int index) const
	{
		if (index < 0 || index >= m_tabs.size())
		{
			return nullptr;
		}
		return m_tabs.at(index);
	}

	int RibbonBar::tabCount() const
	{
		return m_tabs.size();
	}

	int RibbonBar::currentIndex() const
	{
		return m_tabBar ? m_tabBar->currentIndex() : -1;
	}

	void RibbonBar::setCurrentIndex(int index)
	{
		if (!m_tabBar || index < 0 || index >= m_tabs.size())
		{
			return;
		}
		m_tabBar->setCurrentIndex(index);
		m_stack->setCurrentIndex(index);
	}

	void RibbonBar::setSimplified(bool simplified)
	{
		if (m_simplified == simplified)
		{
			return;
		}
		m_simplified = simplified;
		for (RibbonTab* tab : m_tabs)
		{
			tab->setSimplified(simplified);
		}
		syncBarHeightMetric();
		updateGeometry();
		rebuildChrome();
		if (parentWidget())
		{
			parentWidget()->updateGeometry();
		}
		emit simplifiedChanged(m_simplified);
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
		const QColor bg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color());
		const QColor panel = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), palette().base().color());
		const QColor fg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg"), palette().windowText().color());

		QPalette barPal = palette();
		barPal.setColor(QPalette::Window, bg);
		barPal.setColor(QPalette::Base, bg);
		barPal.setColor(QPalette::WindowText, fg);
		barPal.setColor(QPalette::ButtonText, fg);
		setPalette(barPal);
		setAutoFillBackground(true);

		if (m_qat)
		{
			m_qat->polishFromStore();
		}
		if (m_tabBar)
		{
			QPalette tabPal = m_tabBar->palette();
			tabPal.setColor(QPalette::Window, bg);
			tabPal.setColor(QPalette::Button, bg);
			tabPal.setColor(QPalette::WindowText, fg);
			tabPal.setColor(QPalette::ButtonText, fg);
			m_tabBar->setPalette(tabPal);
			m_tabBar->setAutoFillBackground(true);
		}
		if (m_stack)
		{
			QPalette stackPal = m_stack->palette();
			stackPal.setColor(QPalette::Window, panel);
			stackPal.setColor(QPalette::Base, panel);
			m_stack->setPalette(stackPal);
			m_stack->setAutoFillBackground(true);
		}

		for (RibbonTab* tab : m_tabs)
		{
			tab->polishFromStore();
		}

		syncBarHeightMetric();
		updateGeometry();
		rebuildChrome();
		update();
	}

	void RibbonBar::paintEvent(QPaintEvent* event)
	{
		Q_UNUSED(event);
		QPainter p(this);
		const QColor bg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color());
		const QColor border = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("border"), palette().mid().color());
		const QColor accent = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("accent"), QColor(QStringLiteral("#0078D4")));
		const int borderW = qMax(1, qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));
		const int qatH = qatRowHeight();
		const int tabH = tabRowHeight();

		p.fillRect(rect(), bg);
		p.fillRect(QRect(0, height() - borderW, width(), borderW), border);

		const int idx = currentIndex();
		if (idx >= 0 && m_tabBar)
		{
			const QRect tabRect = m_tabBar->tabRect(idx).translated(m_tabBar->pos());
			const int underlineW = qMax(24, tabRect.width() - 16);
			const int underlineX = tabRect.center().x() - underlineW / 2;
			p.fillRect(QRect(underlineX, qatH + tabH - 3, underlineW, 3), accent);
		}
	}

	void RibbonBar::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		rebuildChrome();
	}

	void RibbonBar::rebuildChrome()
	{
		const int qatH = qatRowHeight();
		const int tabH = tabRowHeight();
		const int panelH = panelHeight();
		if (m_qat)
		{
			m_qat->setGeometry(0, 0, width(), qatH);
			m_qat->raise();
		}
		if (m_tabBar)
		{
			m_tabBar->setGeometry(0, qatH, width(), tabH);
			m_tabBar->raise();
		}
		if (m_stack)
		{
			m_stack->setGeometry(0, qatH + tabH, width(), panelH);
		}
		setFixedHeight(qatH + tabH + panelH);
	}

	void RibbonBar::syncBarHeightMetric()
	{
		if (!m_bridge || !m_bridge->engine() || !m_bridge->engine()->store())
		{
			return;
		}
		auto* store = m_bridge->engine()->store();
		store->beginUpdate();
		store->setMetric(QStringLiteral("ribbon"), QStringLiteral("bar.height"), barHeight());
		store->endUpdate();
	}

	int RibbonBar::qatRowHeight() const
	{
		return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("qat.height"), 26);
	}

	int RibbonBar::tabRowHeight() const
	{
		return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("tab.height"), 32);
	}

	int RibbonBar::panelHeight() const
	{
		if (m_simplified)
		{
			return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("group.height.simplified"), 40);
		}
		return qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("group.height"), 88);
	}

	int RibbonBar::barHeight() const
	{
		return qatRowHeight() + tabRowHeight() + panelHeight();
	}
} // namespace qfluentribbon
