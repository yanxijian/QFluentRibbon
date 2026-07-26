#ifndef __QFR_RIBBON_WINDOW_H__
#define __QFR_RIBBON_WINDOW_H__

#include <QMainWindow>
#include <qfluentribbon/qfluentribbon_export.hpp>

class QResizeEvent;
class QShowEvent;

namespace qfluentribbon
{
	class Backstage;
	class KeyTipService;
	class RibbonBar;
	class ThemeBridge;

	/// QMainWindow with a north RibbonBar, Backstage overlay, and Alt KeyTips.
	class QFR_EXPORT RibbonWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit RibbonWindow(QWidget* parent = nullptr);

		[[nodiscard]] RibbonBar* ribbonBar() const
		{
			return m_ribbon;
		}

		[[nodiscard]] Backstage* backstage() const
		{
			return m_backstage;
		}

		[[nodiscard]] KeyTipService* keyTipService() const
		{
			return m_keyTips;
		}

		/// Bind QTE engine bridge; seeds ribbon.* and repolishes chrome.
		void setThemeBridge(ThemeBridge* bridge);

	protected:
		void resizeEvent(QResizeEvent* event) override;
		void showEvent(QShowEvent* event) override;

	private:
		void syncBackstageGeometry();

		RibbonBar* m_ribbon = nullptr;
		Backstage* m_backstage = nullptr;
		KeyTipService* m_keyTips = nullptr;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_WINDOW_H__
