#ifndef __QFR_RIBBON_WINDOW_H__
#define __QFR_RIBBON_WINDOW_H__

#include <QMainWindow>

namespace qfluentribbon
{
	class RibbonBar;
	class ThemeBridge;

	/// QMainWindow with a north RibbonBar (via setMenuWidget). Skinning goes through ThemeBridge + QTE.
	class RibbonWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit RibbonWindow(QWidget* parent = nullptr);

		[[nodiscard]] RibbonBar* ribbonBar() const
		{
			return m_ribbon;
		}

		/// Bind QTE engine bridge; seeds ribbon.* and repolishes the bar.
		void setThemeBridge(ThemeBridge* bridge);

	private:
		RibbonBar* m_ribbon = nullptr;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_WINDOW_H__
