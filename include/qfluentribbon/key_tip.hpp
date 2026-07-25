#ifndef __QFR_KEY_TIP_H__
#define __QFR_KEY_TIP_H__

#include <QList>
#include <QObject>
#include <QSet>
#include <QString>

class QAction;
class QEvent;
class QWidget;

namespace qfluentribbon
{
	class RibbonWindow;

	/// Assign / read KeyTip sequences (stored on QObject properties).
	class KeyTip final
	{
	public:
		static void set(QAction* action, const QString& tip);
		static void set(QWidget* widget, const QString& tip);
		[[nodiscard]] static QString get(const QAction* action);
		[[nodiscard]] static QString get(const QWidget* widget);
	};

	/// Alt KeyTip mode: badges on tabs / QAT / visible commands; type to activate; Esc exits.
	class KeyTipService final : public QObject
	{
		Q_OBJECT
	public:
		explicit KeyTipService(RibbonWindow* window);

		[[nodiscard]] bool isActive() const
		{
			return m_active;
		}

	public slots:
		void activate();
		void deactivate();

	protected:
		bool eventFilter(QObject* watched, QEvent* event) override;

	private:
		struct Target
		{
			QWidget* widget = nullptr;
			QAction* action = nullptr;
			int tabIndex = -1;
			QString tip;
			QWidget* badge = nullptr;
		};

		void collectTargets();
		void clearBadges();
		void refreshBadges();
		[[nodiscard]] QString uniqueTip(const QString& preferred, const QString& fallback, QSet<QString>& used) const;
		[[nodiscard]] QString suggestTip(const QString& text, int disambiguate) const;
		bool handleTyped(const QString& ch);
		bool activateTarget(const Target& target);

		RibbonWindow* m_window = nullptr;
		bool m_active = false;
		bool m_altChord = false;
		QString m_buffer;
		QList<Target> m_targets;
	};
} // namespace qfluentribbon

#endif // __QFR_KEY_TIP_H__
