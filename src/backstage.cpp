#include "qfluentribbon/backstage.hpp"

#include "qfluentribbon/ribbon_tokens.hpp"
#include "qfluentribbon/theme_bridge.hpp"

#include <QKeyEvent>
#include <QListWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QShowEvent>
#include <QStackedWidget>
#include <QToolButton>

namespace qfluentribbon
{
	Backstage::Backstage(QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(QStringLiteral("qfr.Backstage"));
		setFocusPolicy(Qt::StrongFocus);
		hide();

		m_backButton = new QToolButton(this);
		m_backButton->setObjectName(QStringLiteral("qfr.Backstage.back"));
		m_backButton->setText(QStringLiteral("\u2190"));
		m_backButton->setAutoRaise(true);
		m_backButton->setToolTip(QStringLiteral("Back"));
		connect(m_backButton, &QToolButton::clicked, this, &Backstage::dismiss);

		m_nav = new QListWidget(this);
		m_nav->setObjectName(QStringLiteral("qfr.Backstage.nav"));
		m_nav->setFocusPolicy(Qt::StrongFocus);
		m_nav->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		m_stack = new QStackedWidget(this);
		m_stack->setObjectName(QStringLiteral("qfr.Backstage.stack"));

		connect(m_nav, &QListWidget::currentRowChanged, this,
				[this](int row)
				{
					if (m_stack && row >= 0 && row < m_stack->count())
					{
						m_stack->setCurrentIndex(row);
						emit currentChanged(row);
					}
				});
	}

	void Backstage::setThemeBridge(ThemeBridge* bridge)
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
			connect(m_bridge, &ThemeBridge::ribbonTokensChanged, this, &Backstage::polishFromStore);
		}
		polishFromStore();
	}

	int Backstage::addPage(const QString& title, QWidget* page)
	{
		if (!page)
		{
			return -1;
		}
		page->setParent(m_stack);
		m_pages.append(page);
		m_stack->addWidget(page);
		m_nav->addItem(title);
		if (m_pages.size() == 1)
		{
			setCurrentIndex(0);
		}
		rebuildChrome();
		return m_pages.size() - 1;
	}

	int Backstage::pageCount() const
	{
		return m_pages.size();
	}

	int Backstage::currentIndex() const
	{
		return m_nav ? m_nav->currentRow() : -1;
	}

	void Backstage::setCurrentIndex(int index)
	{
		if (!m_nav || index < 0 || index >= m_pages.size())
		{
			return;
		}
		m_nav->setCurrentRow(index);
		m_stack->setCurrentIndex(index);
	}

	void Backstage::open()
	{
		if (isVisible())
		{
			return;
		}
		show();
		raise();
		setFocus(Qt::OtherFocusReason);
		emit opened();
	}

	void Backstage::dismiss()
	{
		if (!isVisible())
		{
			return;
		}
		hide();
		emit closed();
	}

	bool Backstage::isOpen() const
	{
		return isVisible();
	}

	void Backstage::polishFromStore()
	{
		const QColor bg = qfluentribbon::tokens::color(
			QStringLiteral("ribbon"), QStringLiteral("backstage.bg"),
			qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), palette().base().color()));
		const QColor navBg = qfluentribbon::tokens::color(
			QStringLiteral("ribbon"), QStringLiteral("backstage.nav.bg"),
			qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color()));
		const QColor fg = qfluentribbon::tokens::color(
			QStringLiteral("ribbon"), QStringLiteral("backstage.fg"),
			qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("fg"), palette().windowText().color()));
		const QColor accent =
			qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("accent"), QColor(QStringLiteral("#0078D4")));

		QPalette hostPal = palette();
		hostPal.setColor(QPalette::Window, bg);
		hostPal.setColor(QPalette::Base, bg);
		hostPal.setColor(QPalette::WindowText, fg);
		setPalette(hostPal);
		setAutoFillBackground(true);

		if (m_nav)
		{
			QPalette navPal = m_nav->palette();
			navPal.setColor(QPalette::Base, navBg);
			navPal.setColor(QPalette::Window, navBg);
			navPal.setColor(QPalette::Text, fg);
			navPal.setColor(QPalette::WindowText, fg);
			navPal.setColor(QPalette::Highlight, accent);
			navPal.setColor(QPalette::HighlightedText, QColor(Qt::white));
			m_nav->setPalette(navPal);
			m_nav->setAutoFillBackground(true);
		}
		if (m_stack)
		{
			QPalette stackPal = m_stack->palette();
			stackPal.setColor(QPalette::Window, bg);
			stackPal.setColor(QPalette::Base, bg);
			m_stack->setPalette(stackPal);
			m_stack->setAutoFillBackground(true);
		}
		if (m_backButton)
		{
			QPalette btnPal = m_backButton->palette();
			btnPal.setColor(QPalette::ButtonText, fg);
			m_backButton->setPalette(btnPal);
		}

		rebuildChrome();
		update();
	}

	void Backstage::paintEvent(QPaintEvent* event)
	{
		Q_UNUSED(event);
		QPainter p(this);
		const QColor bg = qfluentribbon::tokens::color(
			QStringLiteral("ribbon"), QStringLiteral("backstage.bg"),
			qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), palette().base().color()));
		const QColor navBg = qfluentribbon::tokens::color(
			QStringLiteral("ribbon"), QStringLiteral("backstage.nav.bg"),
			qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("bg"), palette().window().color()));
		const QColor border = qfluentribbon::tokens::color(QStringLiteral("ribbon"), QStringLiteral("border"), palette().mid().color());
		const int navW = navWidth();
		const int borderW = qMax(1, qfluentribbon::tokens::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("border.width"), 1));

		p.fillRect(rect(), bg);
		p.fillRect(QRect(0, 0, navW, height()), navBg);
		p.fillRect(QRect(navW - borderW, 0, borderW, height()), border);
	}

	void Backstage::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		rebuildChrome();
	}

	void Backstage::keyPressEvent(QKeyEvent* event)
	{
		if (event->key() == Qt::Key_Escape)
		{
			dismiss();
			event->accept();
			return;
		}
		QWidget::keyPressEvent(event);
	}

	void Backstage::showEvent(QShowEvent* event)
	{
		QWidget::showEvent(event);
		rebuildChrome();
		setFocus(Qt::OtherFocusReason);
	}

	void Backstage::rebuildChrome()
	{
		const int navW = navWidth();
		const int topPad = 8;
		const int backH = 28;
		if (m_backButton)
		{
			m_backButton->setGeometry(8, topPad, 36, backH);
			m_backButton->raise();
		}
		if (m_nav)
		{
			m_nav->setGeometry(0, topPad + backH + 8, navW, qMax(1, height() - topPad - backH - 8));
		}
		if (m_stack)
		{
			m_stack->setGeometry(navW, 0, qMax(1, width() - navW), height());
		}
	}

	int Backstage::navWidth() const
	{
		return qfluentribbon::tokens::scaledMetric(QStringLiteral("ribbon"), QStringLiteral("backstage.nav.width"), 180);
	}
} // namespace qfluentribbon
