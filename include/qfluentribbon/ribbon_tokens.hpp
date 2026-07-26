#ifndef __QFR_RIBBON_TOKENS_H__
#define __QFR_RIBBON_TOKENS_H__

#include <QColor>
#include <QString>
#include <qfluentribbon/qfluentribbon_export.hpp>

namespace qfluentribbon::tokens
{
	/// Process-local ribbon token store (independent of any theme engine).
	[[nodiscard]] QFR_EXPORT QColor color(const QString& group, const QString& role, const QColor& fallback = {});
	[[nodiscard]] QFR_EXPORT int metric(const QString& group, const QString& role, int fallback = 0);
	/// Metric multiplied by the current DPI scale (default 1.0 until setDpiScale).
	[[nodiscard]] QFR_EXPORT int scaledMetric(const QString& group, const QString& role, int fallback = 0);

	QFR_EXPORT void setColor(const QString& group, const QString& role, const QColor& value);
	QFR_EXPORT void setMetric(const QString& group, const QString& role, int value);
	[[nodiscard]] QFR_EXPORT bool hasColor(const QString& group, const QString& role);
	[[nodiscard]] QFR_EXPORT bool hasMetric(const QString& group, const QString& role);

	QFR_EXPORT void setDpiScale(qreal scale);
	[[nodiscard]] QFR_EXPORT qreal dpiScale();

	/// Seed built-in Fluent ribbon metrics/colors when keys are missing; refresh chrome colors from palette roles.
	QFR_EXPORT void seedDefaults(const QColor& window = QColor(QStringLiteral("#F3F3F3")),
								 const QColor& surface = QColor(QStringLiteral("#FFFFFF")),
								 const QColor& stroke = QColor(QStringLiteral("#D1D1D1")),
								 const QColor& text = QColor(QStringLiteral("#1A1A1A")),
								 const QColor& accent = QColor(QStringLiteral("#0078D4")),
								 const QColor& tertiary = QColor(QStringLiteral("#8D8D8D")), const QColor& accentText = QColor(Qt::white));
} // namespace qfluentribbon::tokens

#endif // __QFR_RIBBON_TOKENS_H__
