#include "qfluentribbon/quick_access_bar.hpp"

#include "qfluentribbon/theme_bridge.hpp"
#include "qtheme/api.hpp"

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QSettings>
#include <QToolButton>
#include <QVariant>
#include <QtGlobal>

namespace qfluentribbon
{
	QuickAccessBar::QuickAccessBar(QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(QStringLiteral("qfr.QuickAccessBar"));
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		setContextMenuPolicy(Qt::DefaultContextMenu);
	}

	void QuickAccessBar::setThemeBridge(ThemeBridge* bridge)
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
			connect(m_bridge, &ThemeBridge::ribbonTokensChanged, this, &QuickAccessBar::polishFromStore);
		}
		polishFromStore();
	}

	bool QuickAccessBar::addAction(QAction* action)
	{
		if (!action || m_actions.contains(action))
		{
			return false;
		}
		m_actions.append(action);
		rebuildButtons();
		emit actionsChanged();
		return true;
	}

	bool QuickAccessBar::removeAction(QAction* action)
	{
		if (!action || !m_actions.contains(action))
		{
			return false;
		}
		m_actions.removeAll(action);
		rebuildButtons();
		emit actionsChanged();
		return true;
	}

	bool QuickAccessBar::contains(QAction* action) const
	{
		return action && m_actions.contains(action);
	}

	void QuickAccessBar::clear()
	{
		if (m_actions.isEmpty())
		{
			return;
		}
		m_actions.clear();
		rebuildButtons();
		emit actionsChanged();
	}

	void QuickAccessBar::saveState(QSettings& settings, const QString& key) const
	{
		settings.setValue(key, actionIds(m_actions));
	}

	int QuickAccessBar::restoreState(QSettings& settings, const QHash<QString, QAction*>& catalog, const QString& key)
	{
		const QStringList ids = settings.value(key).toStringList();
		const QList<QAction*> resolved = resolveIds(ids, catalog);
		if (resolved == m_actions)
		{
			return resolved.size();
		}
		m_actions = resolved;
		rebuildButtons();
		emit actionsChanged();
		return resolved.size();
	}

	QString QuickAccessBar::actionId(const QAction* action)
	{
		if (!action)
		{
			return {};
		}
		const QVariant prop = action->property("qfr.actionId");
		if (prop.isValid() && !prop.toString().isEmpty())
		{
			return prop.toString();
		}
		return action->objectName();
	}

	QStringList QuickAccessBar::actionIds(const QList<QAction*>& actions)
	{
		QStringList ids;
		ids.reserve(actions.size());
		for (QAction* action : actions)
		{
			const QString id = actionId(action);
			if (!id.isEmpty())
			{
				ids.append(id);
			}
		}
		return ids;
	}

	QList<QAction*> QuickAccessBar::resolveIds(const QStringList& ids, const QHash<QString, QAction*>& catalog)
	{
		QList<QAction*> out;
		out.reserve(ids.size());
		for (const QString& id : ids)
		{
			QAction* action = catalog.value(id, nullptr);
			if (action && !out.contains(action))
			{
				out.append(action);
			}
		}
		return out;
	}

	QSize QuickAccessBar::sizeHint() const
	{
		return {200, barHeight()};
	}

	QSize QuickAccessBar::minimumSizeHint() const
	{
		return {40, barHeight()};
	}

	void QuickAccessBar::polishFromStore()
	{
		const QColor bg =
			qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("qat.bg"),
							   qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color()));
		QPalette pal = palette();
		pal.setColor(QPalette::Window, bg);
		pal.setColor(QPalette::Base, bg);
		setPalette(pal);
		setAutoFillBackground(true);

		const int icon = qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("icon.small"), 16);
		for (QToolButton* button : m_buttons)
		{
			button->setIconSize(QSize(icon, icon));
		}
		updateGeometry();
		relayoutButtons();
		update();
	}

	void QuickAccessBar::paintEvent(QPaintEvent* event)
	{
		Q_UNUSED(event);
		QPainter p(this);
		const QColor bg =
			qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("qat.bg"),
							   qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color()));
		const QColor border = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("border"), palette().mid().color());
		const int borderW = qMax(1, qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));
		p.fillRect(rect(), bg);
		p.fillRect(QRect(0, height() - borderW, width(), borderW), border);
	}

	void QuickAccessBar::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		relayoutButtons();
	}

	void QuickAccessBar::contextMenuEvent(QContextMenuEvent* event)
	{
		QToolButton* hit = nullptr;
		for (QToolButton* button : m_buttons)
		{
			if (button->geometry().contains(event->pos()))
			{
				hit = button;
				break;
			}
		}
		if (!hit || !hit->defaultAction())
		{
			QWidget::contextMenuEvent(event);
			return;
		}

		QAction* action = hit->defaultAction();
		QMenu menu(this);
		QAction* remove = menu.addAction(QStringLiteral("Remove from Quick Access Toolbar"));
		if (menu.exec(event->globalPos()) == remove)
		{
			(void)removeAction(action);
		}
	}

	void QuickAccessBar::rebuildButtons()
	{
		qDeleteAll(m_buttons);
		m_buttons.clear();

		const int icon = qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("icon.small"), 16);
		for (QAction* action : m_actions)
		{
			auto* button = new QToolButton(this);
			button->setDefaultAction(action);
			button->setAutoRaise(true);
			button->setFocusPolicy(Qt::TabFocus);
			button->setToolButtonStyle(Qt::ToolButtonIconOnly);
			button->setIconSize(QSize(icon, icon));
			button->setToolTip(QStringLiteral("\u200B"));
			button->show();
			m_buttons.append(button);
		}
		updateGeometry();
		relayoutButtons();
		update();
	}

	void QuickAccessBar::relayoutButtons()
	{
		const int pad = qMax(2, qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("group.padding"), 6) / 2);
		const int h = qMax(1, height());
		const int icon = qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("icon.small"), 16);
		const int sideCap = qMax(icon + 4, h - 2);
		int x = pad;
		for (QToolButton* button : m_buttons)
		{
			const int side = qMin(h - 2, sideCap);
			button->setGeometry(x, (h - side) / 2, side, side);
			x += side + 2;
		}
	}

	int QuickAccessBar::barHeight() const
	{
		return qtheme::api::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("qat.height"), 26);
	}
} // namespace qfluentribbon
