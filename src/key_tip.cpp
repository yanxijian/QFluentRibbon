#include "qfluentribbon/key_tip.hpp"

#include "qfluentribbon/quick_access_bar.hpp"
#include "qfluentribbon/ribbon_bar.hpp"
#include "qfluentribbon/ribbon_group.hpp"
#include "qfluentribbon/ribbon_tab.hpp"
#include "qfluentribbon/ribbon_window.hpp"
#include "qtheme/api.hpp"

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QSet>
#include <QTabBar>
#include <QToolButton>

namespace qfluentribbon
{
	namespace
	{
		constexpr const char* kProp = "qfr.keyTip";

		class KeyTipBadge final : public QLabel
		{
		public:
			explicit KeyTipBadge(QWidget* parent = nullptr)
				: QLabel(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus)
			{
				setAttribute(Qt::WA_ShowWithoutActivating);
				setAlignment(Qt::AlignCenter);
				setMargin(3);
			}

			void polish()
			{
				const QColor bg = qtheme::api::color(
					QStringLiteral("ribbon"), QStringLiteral("keytip.bg"),
					qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("accent"), QColor(QStringLiteral("#0078D4"))));
				const QColor fg = qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("keytip.fg"), QColor(Qt::white));
				QPalette pal = palette();
				pal.setColor(QPalette::Window, bg);
				pal.setColor(QPalette::WindowText, fg);
				setPalette(pal);
				setAutoFillBackground(true);
			}
		};

		QRect badgeAnchor(QWidget* widget, int tabIndex)
		{
			if (!widget)
			{
				return {};
			}
			if (auto* tabs = qobject_cast<QTabBar*>(widget); tabs && tabIndex >= 0)
			{
				const QRect r = tabs->tabRect(tabIndex);
				return QRect(tabs->mapToGlobal(r.topLeft()), r.size());
			}
			return QRect(widget->mapToGlobal(QPoint(0, 0)), widget->size());
		}
	} // namespace

	void KeyTip::set(QAction* action, const QString& tip)
	{
		if (action)
		{
			action->setProperty(kProp, tip.toUpper());
		}
	}

	void KeyTip::set(QWidget* widget, const QString& tip)
	{
		if (widget)
		{
			widget->setProperty(kProp, tip.toUpper());
		}
	}

	QString KeyTip::get(const QAction* action)
	{
		return action ? action->property(kProp).toString() : QString();
	}

	QString KeyTip::get(const QWidget* widget)
	{
		return widget ? widget->property(kProp).toString() : QString();
	}

	KeyTipService::KeyTipService(RibbonWindow* window)
		: QObject(window)
		, m_window(window)
	{
		if (qApp)
		{
			qApp->installEventFilter(this);
		}
	}

	void KeyTipService::activate()
	{
		if (m_active || !m_window)
		{
			return;
		}
		m_active = true;
		m_buffer.clear();
		collectTargets();
		refreshBadges();
	}

	void KeyTipService::deactivate()
	{
		if (!m_active)
		{
			return;
		}
		m_active = false;
		m_buffer.clear();
		clearBadges();
		m_targets.clear();
	}

	bool KeyTipService::eventFilter(QObject* watched, QEvent* event)
	{
		if (!m_window)
		{
			return QObject::eventFilter(watched, event);
		}

		if (event->type() == QEvent::ShortcutOverride || event->type() == QEvent::KeyPress)
		{
			auto* key = static_cast<QKeyEvent*>(event);
			if (!m_active && key->key() == Qt::Key_Alt && key->modifiers() == Qt::AltModifier)
			{
				m_altChord = true;
			}
			if (m_active)
			{
				if (key->key() == Qt::Key_Escape)
				{
					deactivate();
					return true;
				}
				if (key->key() == Qt::Key_Backspace)
				{
					if (!m_buffer.isEmpty())
					{
						m_buffer.chop(1);
						refreshBadges();
					}
					return true;
				}
				const QString ch = key->text().trimmed().toUpper();
				if (ch.size() == 1 && ch.at(0).isLetterOrNumber())
				{
					if (handleTyped(ch))
					{
						return true;
					}
				}
			}
		}
		else if (event->type() == QEvent::KeyRelease)
		{
			auto* key = static_cast<QKeyEvent*>(event);
			if (key->key() == Qt::Key_Alt)
			{
				if (m_altChord && key->modifiers() == Qt::NoModifier && m_window->isActiveWindow())
				{
					m_altChord = false;
					if (m_active)
					{
						deactivate();
					}
					else
					{
						activate();
					}
					return true;
				}
				m_altChord = false;
			}
		}
		else if (event->type() == QEvent::WindowDeactivate && watched == m_window)
		{
			deactivate();
		}
		return QObject::eventFilter(watched, event);
	}

	void KeyTipService::collectTargets()
	{
		clearBadges();
		m_targets.clear();
		RibbonBar* bar = m_window ? m_window->ribbonBar() : nullptr;
		if (!bar)
		{
			return;
		}

		QSet<QString> used;

		if (QTabBar* tabs = bar->tabBar())
		{
			for (int i = 0; i < tabs->count(); ++i)
			{
				const QString text = tabs->tabText(i);
				QString preferred = tabs->tabData(i).toString();
				if (preferred.isEmpty())
				{
					preferred = KeyTip::get(tabs);
				}
				Target t;
				t.widget = tabs;
				t.tabIndex = i;
				t.tip = uniqueTip(preferred, text, used);
				m_targets.append(t);
			}
		}

		if (QuickAccessBar* qat = bar->quickAccessBar())
		{
			const auto buttons = qat->findChildren<QToolButton*>();
			for (QToolButton* button : buttons)
			{
				if (!button->isVisible() || !button->defaultAction())
				{
					continue;
				}
				QAction* action = button->defaultAction();
				Target t;
				t.widget = button;
				t.action = action;
				t.tip = uniqueTip(KeyTip::get(action), action->text(), used);
				m_targets.append(t);
			}
		}

		RibbonTab* tab = bar->tabAt(bar->currentIndex());
		if (tab)
		{
			for (RibbonGroup* group : tab->groups())
			{
				// Include nested gallery tool buttons (not only direct children).
				for (QToolButton* button : group->findChildren<QToolButton*>())
				{
					if (!button->isVisible())
					{
						continue;
					}
					QAction* action = button->defaultAction();
					const QString fallback = action ? action->text() : button->text();
					const QString preferred = action ? KeyTip::get(action) : KeyTip::get(button);
					Target t;
					t.widget = button;
					t.action = action;
					t.tip = uniqueTip(preferred, fallback, used);
					m_targets.append(t);
				}
			}
		}
	}

	void KeyTipService::clearBadges()
	{
		for (Target& t : m_targets)
		{
			delete t.badge;
			t.badge = nullptr;
		}
	}

	void KeyTipService::refreshBadges()
	{
		for (Target& t : m_targets)
		{
			const bool match = m_buffer.isEmpty() || t.tip.startsWith(m_buffer);
			if (!match)
			{
				if (t.badge)
				{
					t.badge->hide();
				}
				continue;
			}
			if (!t.badge)
			{
				auto* badge = new KeyTipBadge(nullptr);
				badge->polish();
				t.badge = badge;
			}
			auto* badge = static_cast<KeyTipBadge*>(t.badge);
			badge->setText(t.tip);
			badge->adjustSize();
			const QRect anchor = badgeAnchor(t.widget, t.tabIndex);
			badge->move(anchor.center().x() - badge->width() / 2, anchor.top() - badge->height() + 4);
			badge->show();
			badge->raise();
		}
	}

	QString KeyTipService::uniqueTip(const QString& preferred, const QString& fallback, QSet<QString>& used) const
	{
		QString tip = preferred.toUpper();
		if (tip.isEmpty())
		{
			tip = suggestTip(fallback, 0);
		}
		QString candidate = tip;
		int n = 1;
		while (used.contains(candidate))
		{
			candidate = suggestTip(fallback, n++);
			if (n > 40)
			{
				candidate = tip + QString::number(n);
				break;
			}
		}
		used.insert(candidate);
		return candidate;
	}

	QString KeyTipService::suggestTip(const QString& text, int disambiguate) const
	{
		QString base;
		for (QChar ch : text)
		{
			if (ch.isLetterOrNumber())
			{
				base = QString(ch).toUpper();
				break;
			}
		}
		if (base.isEmpty())
		{
			base = QStringLiteral("X");
		}
		if (disambiguate <= 0)
		{
			return base;
		}
		return base + QString::number(disambiguate);
	}

	bool KeyTipService::handleTyped(const QString& ch)
	{
		m_buffer += ch;
		int matches = 0;
		const Target* exact = nullptr;
		for (const Target& t : m_targets)
		{
			if (t.tip.startsWith(m_buffer))
			{
				++matches;
				if (t.tip == m_buffer)
				{
					exact = &t;
				}
			}
		}
		if (matches == 0)
		{
			m_buffer.chop(1);
			return true;
		}
		refreshBadges();
		if (exact && matches == 1)
		{
			activateTarget(*exact);
			deactivate();
		}
		else if (exact && exact->tip == m_buffer)
		{
			// Unique exact match among prefixes still continuing — activate if only exact equals buffer and no longer tips.
			bool longer = false;
			for (const Target& t : m_targets)
			{
				if (t.tip.startsWith(m_buffer) && t.tip.size() > m_buffer.size())
				{
					longer = true;
					break;
				}
			}
			if (!longer)
			{
				activateTarget(*exact);
				deactivate();
			}
		}
		return true;
	}

	bool KeyTipService::activateTarget(const Target& target)
	{
		if (target.tabIndex >= 0 && m_window && m_window->ribbonBar())
		{
			m_window->ribbonBar()->setCurrentIndex(target.tabIndex);
			return true;
		}
		if (target.action)
		{
			target.action->trigger();
			return true;
		}
		if (auto* button = qobject_cast<QToolButton*>(target.widget))
		{
			button->animateClick();
			return true;
		}
		return false;
	}
} // namespace qfluentribbon
