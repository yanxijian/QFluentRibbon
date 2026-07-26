#ifndef __QFR_RIBBON_BAR_H__
#define __QFR_RIBBON_BAR_H__

#include <QList>
#include <QString>
#include <QWidget>
#include <qfluentribbon/qfluentribbon_export.hpp>

class QPaintEvent;
class QResizeEvent;
class QStackedWidget;
class QTabBar;

namespace qfluentribbon
{
	class QuickAccessBar;
	class RibbonTab;
	class ThemeBridge;

	/// Classic ribbon chrome: optional QAT + tab strip + stacked RibbonTab panels (ThemeStore colors, no QSS).
	class QFR_EXPORT RibbonBar final : public QWidget
	{
		Q_OBJECT
	public:
		explicit RibbonBar(QWidget* parent = nullptr);

		void setThemeBridge(ThemeBridge* bridge);
		[[nodiscard]] ThemeBridge* themeBridge() const
		{
			return m_bridge;
		}

		[[nodiscard]] QuickAccessBar* quickAccessBar() const
		{
			return m_qat;
		}

		[[nodiscard]] QTabBar* tabBar() const
		{
			return m_tabBar;
		}

		[[nodiscard]] RibbonTab* addTab(const QString& title);
		[[nodiscard]] RibbonTab* tabAt(int index) const;
		[[nodiscard]] int tabCount() const;
		[[nodiscard]] int currentIndex() const;
		void setCurrentIndex(int index);

		void setSimplified(bool simplified);
		[[nodiscard]] bool isSimplified() const
		{
			return m_simplified;
		}

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
		void simplifiedChanged(bool simplified);

	protected:
		void paintEvent(QPaintEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;

	private:
		void rebuildChrome();
		void syncBarHeightMetric();
		[[nodiscard]] int qatRowHeight() const;
		[[nodiscard]] int tabRowHeight() const;
		[[nodiscard]] int panelHeight() const;
		[[nodiscard]] int barHeight() const;

		ThemeBridge* m_bridge = nullptr;
		QuickAccessBar* m_qat = nullptr;
		QTabBar* m_tabBar = nullptr;
		QStackedWidget* m_stack = nullptr;
		QList<RibbonTab*> m_tabs;
		bool m_simplified = false;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_BAR_H__
