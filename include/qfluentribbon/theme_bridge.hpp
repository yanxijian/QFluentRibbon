#ifndef __QFR_THEME_BRIDGE_H__
#define __QFR_THEME_BRIDGE_H__

#include <QObject>

namespace qtheme
{
	class Engine;
	class ThemeStore;
} // namespace qtheme

namespace qfluentribbon
{
	/// Seeds draft `ribbon.*` tokens into the active ThemeStore and keeps chrome in sync with QTE skin changes.
	/// Does not own the Engine; call bind() after Engine::apply.
	class ThemeBridge final : public QObject
	{
		Q_OBJECT
	public:
		explicit ThemeBridge(QObject* parent = nullptr);

		void bind(qtheme::Engine* engine);
		[[nodiscard]] qtheme::Engine* engine() const
		{
			return m_engine;
		}

		/// Ensure ribbon metrics/colors exist (derive from palette when absent). Safe to call after every pack reload.
		void ensureRibbonTokens();

	signals:
		void ribbonTokensChanged();

	private slots:
		void onSkinChanged(const QString& previous, const QString& current);
		void onAccentChanged(const QColor& accent);
		void onColorSchemeChanged();
		void onDpiScaleChanged(qreal scale);

	private:
		void seedDefaults(qtheme::ThemeStore* store);
		qtheme::Engine* m_engine = nullptr;
	};
} // namespace qfluentribbon

#endif // __QFR_THEME_BRIDGE_H__
