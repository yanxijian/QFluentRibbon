#include "qfluentribbon/ribbon_window.hpp"

#include "qfluentribbon/backstage.hpp"
#include "qfluentribbon/key_tip.hpp"
#include "qfluentribbon/ribbon_bar.hpp"
#include "qfluentribbon/theme_bridge.hpp"

#include <QResizeEvent>
#include <QShowEvent>

namespace qfluentribbon
{
	RibbonWindow::RibbonWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		m_ribbon = new RibbonBar(this);
		setMenuWidget(m_ribbon);

		m_backstage = new Backstage(this);
		m_backstage->hide();
		connect(m_backstage, &Backstage::opened, this, &RibbonWindow::syncBackstageGeometry);

		m_keyTips = new KeyTipService(this);
	}

	void RibbonWindow::setThemeBridge(ThemeBridge* bridge)
	{
		if (m_ribbon)
		{
			m_ribbon->setThemeBridge(bridge);
		}
		if (m_backstage)
		{
			m_backstage->setThemeBridge(bridge);
		}
	}

	void RibbonWindow::resizeEvent(QResizeEvent* event)
	{
		QMainWindow::resizeEvent(event);
		syncBackstageGeometry();
	}

	void RibbonWindow::showEvent(QShowEvent* event)
	{
		QMainWindow::showEvent(event);
		syncBackstageGeometry();
	}

	void RibbonWindow::syncBackstageGeometry()
	{
		if (!m_backstage)
		{
			return;
		}
		QWidget* content = centralWidget();
		if (!content)
		{
			const int top = m_ribbon ? m_ribbon->height() : 0;
			m_backstage->setGeometry(0, top, width(), qMax(1, height() - top));
			return;
		}
		const QPoint topLeft = content->mapTo(this, QPoint(0, 0));
		m_backstage->setGeometry(QRect(topLeft, content->size()));
		m_backstage->raise();
	}
} // namespace qfluentribbon
