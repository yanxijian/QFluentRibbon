#ifndef __QFR_SCREEN_TIP_H__
#define __QFR_SCREEN_TIP_H__

#include <QObject>
#include <QString>

class QAction;
class QWidget;

namespace qfluentribbon
{
	/// Lightweight ScreenTip: title + description popup (ThemeStore colors, no QSS).
	class ScreenTip final
	{
	public:
		static void set(QAction* action, const QString& title, const QString& description);
		[[nodiscard]] static QString title(const QAction* action);
		[[nodiscard]] static QString description(const QAction* action);

		/// Install a shared hover filter under host (typically RibbonBar).
		static void install(QWidget* host);
	};

	class ScreenTipFilter final : public QObject
	{
		Q_OBJECT
	public:
		explicit ScreenTipFilter(QWidget* host);
		~ScreenTipFilter() override;

	protected:
		bool eventFilter(QObject* watched, QEvent* event) override;

	private:
		void showFor(QWidget* widget, const QPoint& globalPos);
		void hideTip();

		QWidget* m_host = nullptr;
		QWidget* m_popup = nullptr;
	};
} // namespace qfluentribbon

#endif // __QFR_SCREEN_TIP_H__
