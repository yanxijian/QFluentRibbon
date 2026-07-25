#include "qfluentribbon/ribbon_window.hpp"

#include "qfluentribbon/ribbon_bar.hpp"
#include "qfluentribbon/theme_bridge.hpp"

namespace qfluentribbon
{
	RibbonWindow::RibbonWindow(QWidget* parent)
		: QMainWindow(parent)
	{
		m_ribbon = new RibbonBar(this);
		setMenuWidget(m_ribbon);
	}

	void RibbonWindow::setThemeBridge(ThemeBridge* bridge)
	{
		if (m_ribbon)
		{
			m_ribbon->setThemeBridge(bridge);
		}
	}
} // namespace qfluentribbon
