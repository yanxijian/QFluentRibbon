#include "qfluentribbon/ribbon_tab.hpp"

#include "qfluentribbon/ribbon_group.hpp"
#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"

#include <QResizeEvent>
#include <QShowEvent>

namespace qfluentribbon
{
	RibbonTab::RibbonTab(const QString& title, QWidget* parent)
		: QWidget(parent)
		, m_title(title)
	{
		setObjectName(QStringLiteral("qfr.RibbonTab"));
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}

	void RibbonTab::setTitle(const QString& title)
	{
		m_title = title;
	}

	void RibbonTab::setThemeBridge(ThemeBridge* bridge)
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
			connect(m_bridge, &ThemeBridge::ribbonTokensChanged, this, &RibbonTab::polishFromStore);
		}
		for (RibbonGroup* group : m_groups)
		{
			group->setThemeBridge(bridge);
		}
		polishFromStore();
	}

	RibbonGroup* RibbonTab::addGroup(const QString& title)
	{
		auto* group = new RibbonGroup(title, this);
		group->setThemeBridge(m_bridge);
		m_groups.append(group);
		group->show();
		relayout();
		return group;
	}

	void RibbonTab::applyCollapse(int availableWidth)
	{
		QVector<layout::GroupWidthHints> hints;
		hints.reserve(m_groups.size());
		for (RibbonGroup* group : m_groups)
		{
			hints.append(group->widthHints());
		}
		const QVector<RibbonItemSize> sizes = layout::chooseUniformSizes(availableWidth, hints);
		for (int i = 0; i < m_groups.size() && i < sizes.size(); ++i)
		{
			m_groups[i]->setItemSize(sizes[i]);
		}
		relayout();
	}

	void RibbonTab::polishFromStore()
	{
		const QColor panel = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), palette().base().color());
		QPalette pal = palette();
		pal.setColor(QPalette::Window, panel);
		pal.setColor(QPalette::Base, panel);
		setPalette(pal);
		setAutoFillBackground(true);
		for (RibbonGroup* group : m_groups)
		{
			group->polishFromStore();
		}
		relayout();
		update();
	}

	void RibbonTab::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		applyCollapse(width());
	}

	void RibbonTab::showEvent(QShowEvent* event)
	{
		QWidget::showEvent(event);
		applyCollapse(width());
	}

	void RibbonTab::relayout()
	{
		const int pad = qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6);
		int x = pad;
		const int y = 0;
		const int h = qMax(1, height());
		for (RibbonGroup* group : m_groups)
		{
			const int w = group->sizeHint().width();
			group->setGeometry(x, y, w, h);
			x += w;
		}
	}
} // namespace qfluentribbon
