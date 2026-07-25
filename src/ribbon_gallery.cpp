#include "qfluentribbon/ribbon_gallery.hpp"

#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"

#include <QAction>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QToolButton>

namespace qfluentribbon
{
	RibbonGallery::RibbonGallery(QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(QStringLiteral("qfr.RibbonGallery"));
		setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	}

	void RibbonGallery::setThemeBridge(ThemeBridge* bridge)
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
			connect(m_bridge, &ThemeBridge::ribbonTokensChanged, this, &RibbonGallery::polishFromStore);
		}
		polishFromStore();
	}

	int RibbonGallery::addItem(const QString& text, const QIcon& icon, QAction* action)
	{
		Item item;
		item.text = text;
		item.icon = icon;
		item.action = action;
		m_items.append(item);
		rebuild();
		return m_items.size() - 1;
	}

	QSize RibbonGallery::sizeHint() const
	{
		const int n = qMax(1, m_items.size());
		return {n * itemWidth() + 8, itemHeight() + 8};
	}

	QSize RibbonGallery::minimumSizeHint() const
	{
		return {itemWidth() + 8, itemHeight() + 8};
	}

	void RibbonGallery::polishFromStore()
	{
		const int icon = qtheme::api::metric(QStringLiteral("ribbon"), QStringLiteral("icon.medium"), 16);
		for (Item& item : m_items)
		{
			if (item.button)
			{
				item.button->setIconSize(QSize(icon, icon));
			}
		}
		updateGeometry();
		relayout();
		update();
	}

	void RibbonGallery::paintEvent(QPaintEvent* event)
	{
		Q_UNUSED(event);
		QPainter p(this);
		const QColor border = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("border"), palette().mid().color());
		const int borderW = qMax(1, qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));
		p.setPen(border);
		p.drawRect(rect().adjusted(0, 0, -borderW, -borderW));
	}

	void RibbonGallery::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		relayout();
	}

	void RibbonGallery::rebuild()
	{
		for (int i = 0; i < m_items.size(); ++i)
		{
			Item& item = m_items[i];
			if (item.button)
			{
				continue;
			}
			auto* button = new QToolButton(this);
			button->setAutoRaise(true);
			button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
			button->setFocusPolicy(Qt::TabFocus);
			button->setToolTip(QStringLiteral("\u200B"));
			const QIcon icon = !item.icon.isNull() ? item.icon : (item.action ? item.action->icon() : QIcon());
			const QString text = !item.text.isEmpty() ? item.text : (item.action ? item.action->text() : QString());
			button->setIcon(icon);
			button->setText(text);
			connect(button, &QToolButton::clicked, this,
					[this, i]()
					{
						emit itemClicked(i);
						if (i >= 0 && i < m_items.size() && m_items[i].action)
						{
							m_items[i].action->trigger();
						}
					});
			button->show();
			item.button = button;
		}
		polishFromStore();
	}

	void RibbonGallery::relayout()
	{
		const int pad = 4;
		const int iw = itemWidth();
		const int ih = itemHeight();
		int x = pad;
		const int y = qMax(0, (height() - ih) / 2);
		for (Item& item : m_items)
		{
			if (!item.button)
			{
				continue;
			}
			item.button->setGeometry(x, y, iw, ih);
			x += iw + 2;
		}
	}

	int RibbonGallery::itemWidth() const
	{
		return qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("gallery.item.width"), 56);
	}

	int RibbonGallery::itemHeight() const
	{
		return qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("gallery.item.height"), 64);
	}
} // namespace qfluentribbon
