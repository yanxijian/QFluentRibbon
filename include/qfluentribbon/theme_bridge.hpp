#ifndef __QFR_THEME_BRIDGE_H__
#define __QFR_THEME_BRIDGE_H__

#include <QColor>
#include <QObject>
#include <qfluentribbon/qfluentribbon_export.hpp>

namespace qfluentribbon
{
	/// Owns notification for ribbon chrome when tokens change.
	/// Does not depend on any external theme engine; apps (e.g. Gallery) seed
	/// qfluentribbon::tokens then call ensureRibbonTokens() / notifyTokensChanged().
	class QFR_EXPORT ThemeBridge final : public QObject
	{
		Q_OBJECT
	public:
		explicit ThemeBridge(QObject* parent = nullptr);

		/// Seed built-in defaults into the local token store (using optional palette colors) and emit.
		void ensureRibbonTokens(const QColor& window = QColor(QStringLiteral("#F3F3F3")),
								const QColor& surface = QColor(QStringLiteral("#FFFFFF")),
								const QColor& stroke = QColor(QStringLiteral("#D1D1D1")),
								const QColor& text = QColor(QStringLiteral("#1A1A1A")),
								const QColor& accent = QColor(QStringLiteral("#0078D4")),
								const QColor& tertiary = QColor(QStringLiteral("#8D8D8D")), const QColor& accentText = QColor(Qt::white));

		/// Emit ribbonTokensChanged after an external writer updated qfluentribbon::tokens.
		void notifyTokensChanged();

	signals:
		void ribbonTokensChanged();
	};
} // namespace qfluentribbon

#endif // __QFR_THEME_BRIDGE_H__
