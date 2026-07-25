#ifndef __QFR_RIBBON_BAR_H__
#define __QFR_RIBBON_BAR_H__

#include <QString>
#include <QWidget>

namespace qfluentribbon
{
	class ThemeBridge;

	/// M0 placeholder chrome: ThemeStore-backed strip (no QSS). Tabs/groups arrive in M1.
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

		void setStatusText(const QString& text);
		[[nodiscard]] QString statusText() const
		{
			return m_statusText;
		}

		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;

	public slots:
		void polishFromStore();

	protected:
		void paintEvent(QPaintEvent* event) override;

	private:
		[[nodiscard]] int barHeight() const;

		ThemeBridge* m_bridge = nullptr;
		QString m_statusText;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_BAR_H__
