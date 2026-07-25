#ifndef __QFR_RIBBON_BAR_H__
#define __QFR_RIBBON_BAR_H__

#include <QList>
#include <QString>
#include <QWidget>

class QPaintEvent;
class QResizeEvent;
class QStackedWidget;
class QTabBar;

namespace qfluentribbon
{
	class RibbonTab;
	class ThemeBridge;

	/// Classic ribbon chrome: tab strip + stacked RibbonTab panels (ThemeStore colors, no QSS).
	class RibbonBar final : public QWidget
	{
		Q_OBJECT
	public:
		explicit RibbonBar(QWidget* parent = nullptr);

		void setThemeBridge(ThemeBridge* bridge);
		[[nodiscard]] ThemeBridge* themeBridge() const
		{
			return m_bridge;
		}

		[[nodiscard]] RibbonTab* addTab(const QString& title);
		[[nodiscard]] RibbonTab* tabAt(int index) const;
		[[nodiscard]] int tabCount() const;
		[[nodiscard]] int currentIndex() const;
		void setCurrentIndex(int index);

		[[nodiscard]] QList<RibbonTab*> tabs() const
		{
			return m_tabs;
		}

		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;

	public slots:
		void polishFromStore();

	signals:
		void currentChanged(int index);

	protected:
		void paintEvent(QPaintEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;

	private:
		void rebuildChrome();
		[[nodiscard]] int tabRowHeight() const;
		[[nodiscard]] int panelHeight() const;
		[[nodiscard]] int barHeight() const;

		ThemeBridge* m_bridge = nullptr;
		QTabBar* m_tabBar = nullptr;
		QStackedWidget* m_stack = nullptr;
		QList<RibbonTab*> m_tabs;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_BAR_H__
