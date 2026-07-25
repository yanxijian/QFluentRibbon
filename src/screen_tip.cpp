#include "qfluentribbon/screen_tip.hpp"

#include "qtheme/api.hpp"

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QFont>
#include <QGuiApplication>
#include <QHelpEvent>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QScreen>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

namespace qfluentribbon
{
	namespace
	{
		constexpr const char* kTitleProp = "qfr.screenTipTitle";
		constexpr const char* kBodyProp = "qfr.screenTipBody";

		QPoint clampTipPos(const QPoint& preferred, const QSize& tipSize, const QPoint& anchorGlobal)
		{
			QScreen* screen = QGuiApplication::screenAt(anchorGlobal);
			if (!screen)
			{
				screen = QGuiApplication::primaryScreen();
			}
			if (!screen)
			{
				return preferred;
			}
			const QRect avail = screen->availableGeometry();
			QRect tip(preferred, tipSize);
			if (tip.right() > avail.right())
			{
				tip.moveRight(avail.right());
			}
			if (tip.left() < avail.left())
			{
				tip.moveLeft(avail.left());
			}
			if (tip.bottom() > avail.bottom())
			{
				// Flip above the cursor / control when the bottom edge would clip.
				tip.moveBottom(qMin(preferred.y() - 8, avail.bottom()));
			}
			if (tip.top() < avail.top())
			{
				tip.moveTop(avail.top());
			}
			return tip.topLeft();
		}

		class TipPopup final : public QWidget
		{
		public:
			explicit TipPopup(QWidget* parent = nullptr)
				: QWidget(parent, Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus)
			{
				setAttribute(Qt::WA_ShowWithoutActivating);
				setAttribute(Qt::WA_TransparentForMouseEvents);
				auto* layout = new QVBoxLayout(this);
				layout->setContentsMargins(10, 8, 10, 8);
				layout->setSpacing(4);
				m_title = new QLabel(this);
				m_body = new QLabel(this);
				m_body->setWordWrap(true);
				m_body->setMaximumWidth(280);
				layout->addWidget(m_title);
				layout->addWidget(m_body);
			}

			void setContent(const QString& title, const QString& body)
			{
				m_title->setText(title);
				m_body->setText(body);
				m_body->setVisible(!body.isEmpty());
				polish();
				adjustSize();
			}

			void polish()
			{
				const QColor bg = qtheme::api::color(
					QStringLiteral("ribbon"), QStringLiteral("screentip.bg"),
					qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), QColor(QStringLiteral("#FFFFFF"))));
				const QColor fg = qtheme::api::color(
					QStringLiteral("ribbon"), QStringLiteral("screentip.fg"),
					qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg"), QColor(QStringLiteral("#1A1A1A"))));
				const QColor fgSec = qtheme::api::color(
					QStringLiteral("ribbon"), QStringLiteral("screentip.fg.secondary"),
					qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("fg.secondary"), QColor(QStringLiteral("#8D8D8D"))));
				const QColor border = qtheme::api::color(
					QStringLiteral("ribbon"), QStringLiteral("screentip.border"),
					qtheme::api::color(QStringLiteral("ribbon"), QStringLiteral("border"), QColor(QStringLiteral("#D1D1D1"))));

				QPalette pal = palette();
				pal.setColor(QPalette::Window, bg);
				pal.setColor(QPalette::WindowText, fg);
				setPalette(pal);
				setAutoFillBackground(true);

				QFont titleFont = font();
				titleFont.setBold(true);
				m_title->setFont(titleFont);
				QPalette titlePal = m_title->palette();
				titlePal.setColor(QPalette::WindowText, fg);
				m_title->setPalette(titlePal);

				QPalette bodyPal = m_body->palette();
				bodyPal.setColor(QPalette::WindowText, fgSec);
				m_body->setPalette(bodyPal);

				m_border = border;
				update();
			}

		protected:
			void paintEvent(QPaintEvent* event) override
			{
				QWidget::paintEvent(event);
				QPainter p(this);
				p.setPen(m_border);
				p.drawRect(rect().adjusted(0, 0, -1, -1));
			}

		private:
			QLabel* m_title = nullptr;
			QLabel* m_body = nullptr;
			QColor m_border;
		};
	} // namespace

	void ScreenTip::set(QAction* action, const QString& title, const QString& description)
	{
		if (!action)
		{
			return;
		}
		action->setProperty(kTitleProp, title);
		action->setProperty(kBodyProp, description);
		// Keep native tooltip empty so Qt's default tip does not fight ScreenTip.
		action->setToolTip(QString());
	}

	QString ScreenTip::title(const QAction* action)
	{
		if (!action)
		{
			return {};
		}
		const QVariant v = action->property(kTitleProp);
		if (v.isValid() && !v.toString().isEmpty())
		{
			return v.toString();
		}
		return action->text();
	}

	QString ScreenTip::description(const QAction* action)
	{
		if (!action)
		{
			return {};
		}
		return action->property(kBodyProp).toString();
	}

	void ScreenTip::install(QWidget* host)
	{
		if (!host || !qApp)
		{
			return;
		}
		if (host->property("qfr.screenTipInstalled").toBool())
		{
			return;
		}
		host->setProperty("qfr.screenTipInstalled", true);
		auto* filter = new ScreenTipFilter(host);
		qApp->installEventFilter(filter);
	}

	ScreenTipFilter::ScreenTipFilter(QWidget* host)
		: QObject(host)
		, m_host(host)
	{
	}

	ScreenTipFilter::~ScreenTipFilter()
	{
		delete m_popup;
		m_popup = nullptr;
	}

	bool ScreenTipFilter::eventFilter(QObject* watched, QEvent* event)
	{
		if (!m_host)
		{
			return QObject::eventFilter(watched, event);
		}

		if (event->type() == QEvent::ToolTip)
		{
			auto* widget = qobject_cast<QWidget*>(watched);
			if (!widget || (!m_host->isAncestorOf(widget) && widget != m_host))
			{
				return QObject::eventFilter(watched, event);
			}
			auto* help = static_cast<QHelpEvent*>(event);
			showFor(widget, help->globalPos());
			return true;
		}

		if (event->type() == QEvent::Leave || event->type() == QEvent::Hide || event->type() == QEvent::MouseButtonPress)
		{
			auto* widget = qobject_cast<QWidget*>(watched);
			if (widget && (m_host->isAncestorOf(widget) || widget == m_host))
			{
				hideTip();
			}
		}
		return QObject::eventFilter(watched, event);
	}

	void ScreenTipFilter::showFor(QWidget* widget, const QPoint& globalPos)
	{
		QToolButton* button = nullptr;
		for (QWidget* w = widget; w; w = w->parentWidget())
		{
			button = qobject_cast<QToolButton*>(w);
			if (button)
			{
				break;
			}
			if (w == m_host)
			{
				break;
			}
		}
		if (!button)
		{
			hideTip();
			return;
		}

		QAction* action = button->defaultAction();
		if (!action)
		{
			action = button->property("qfr.launcherAction").value<QAction*>();
		}
		if (!action)
		{
			hideTip();
			return;
		}

		const QString tipTitle = ScreenTip::title(action);
		const QString tipBody = ScreenTip::description(action);
		if (tipTitle.isEmpty() && tipBody.isEmpty())
		{
			hideTip();
			return;
		}

		if (!m_popup)
		{
			m_popup = new TipPopup(nullptr);
		}
		auto* tip = static_cast<TipPopup*>(m_popup);
		tip->setContent(tipTitle, tipBody);
		const QPoint preferred = globalPos + QPoint(12, 16);
		tip->move(clampTipPos(preferred, tip->size(), globalPos));
		tip->show();
		tip->raise();
	}

	void ScreenTipFilter::hideTip()
	{
		if (m_popup)
		{
			m_popup->hide();
		}
	}
} // namespace qfluentribbon
