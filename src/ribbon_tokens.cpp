#include "qfluentribbon/ribbon_tokens.hpp"

#include <QHash>
#include <QMutex>
#include <QtGlobal>

namespace qfluentribbon::tokens
{
	namespace
	{
		struct Store
		{
			QMutex mutex;
			QHash<QString, QColor> colors;
			QHash<QString, int> metrics;
			qreal dpi = 1.0;
		};

		Store& store()
		{
			static Store s;
			return s;
		}

		QString key(const QString& group, const QString& role)
		{
			return group + QLatin1Char('/') + role;
		}
	} // namespace

	QColor color(const QString& group, const QString& role, const QColor& fallback)
	{
		QMutexLocker lock(&store().mutex);
		const auto it = store().colors.constFind(key(group, role));
		return it == store().colors.cend() ? fallback : *it;
	}

	int metric(const QString& group, const QString& role, int fallback)
	{
		QMutexLocker lock(&store().mutex);
		const auto it = store().metrics.constFind(key(group, role));
		return it == store().metrics.cend() ? fallback : *it;
	}

	int scaledMetric(const QString& group, const QString& role, int fallback)
	{
		const int logical = metric(group, role, fallback);
		QMutexLocker lock(&store().mutex);
		return qRound(logical * store().dpi);
	}

	void setColor(const QString& group, const QString& role, const QColor& value)
	{
		QMutexLocker lock(&store().mutex);
		store().colors.insert(key(group, role), value);
	}

	void setMetric(const QString& group, const QString& role, int value)
	{
		QMutexLocker lock(&store().mutex);
		store().metrics.insert(key(group, role), value);
	}

	bool hasColor(const QString& group, const QString& role)
	{
		QMutexLocker lock(&store().mutex);
		return store().colors.contains(key(group, role));
	}

	bool hasMetric(const QString& group, const QString& role)
	{
		QMutexLocker lock(&store().mutex);
		return store().metrics.contains(key(group, role));
	}

	void setDpiScale(qreal scale)
	{
		QMutexLocker lock(&store().mutex);
		store().dpi = scale > 0.0 ? scale : 1.0;
	}

	qreal dpiScale()
	{
		QMutexLocker lock(&store().mutex);
		return store().dpi;
	}

	void seedDefaults(const QColor& window, const QColor& surface, const QColor& stroke, const QColor& text, const QColor& accent,
					  const QColor& tertiary, const QColor& accentText)
	{
		auto setMetricIfAbsent = [](const QString& role, int value)
		{
			if (!hasMetric(QStringLiteral("ribbon"), role))
			{
				setMetric(QStringLiteral("ribbon"), role, value);
			}
		};

		setMetricIfAbsent(QStringLiteral("tab.height"), 32);
		setMetricIfAbsent(QStringLiteral("group.height"), 88);
		setMetricIfAbsent(QStringLiteral("group.padding"), 6);
		setMetricIfAbsent(QStringLiteral("group.titleHeight"), 18);
		setMetricIfAbsent(QStringLiteral("border.width"), 1);
		setMetricIfAbsent(QStringLiteral("accent.underline"), 3);
		setMetricIfAbsent(QStringLiteral("icon.large"), 32);
		setMetricIfAbsent(QStringLiteral("icon.medium"), 16);
		setMetricIfAbsent(QStringLiteral("icon.small"), 16);
		setMetricIfAbsent(QStringLiteral("group.height.simplified"), 40);
		setMetricIfAbsent(QStringLiteral("qat.height"), 26);
		setMetricIfAbsent(QStringLiteral("backstage.nav.width"), 180);
		setMetricIfAbsent(QStringLiteral("gallery.item.width"), 56);
		setMetricIfAbsent(QStringLiteral("gallery.item.height"), 64);

		setMetric(QStringLiteral("ribbon"), QStringLiteral("bar.height"),
				  metric(QStringLiteral("ribbon"), QStringLiteral("qat.height"), 26)
					  + metric(QStringLiteral("ribbon"), QStringLiteral("tab.height"), 32)
					  + metric(QStringLiteral("ribbon"), QStringLiteral("group.height"), 88));

		setColor(QStringLiteral("ribbon"), QStringLiteral("bg"), window);
		setColor(QStringLiteral("ribbon"), QStringLiteral("tab.row.bg"), window);
		setColor(QStringLiteral("ribbon"), QStringLiteral("panel.bg"), surface);
		setColor(QStringLiteral("ribbon"), QStringLiteral("border"), stroke);
		setColor(QStringLiteral("ribbon"), QStringLiteral("fg"), text);
		setColor(QStringLiteral("ribbon"), QStringLiteral("fg.secondary"), tertiary);
		setColor(QStringLiteral("ribbon"), QStringLiteral("accent"), accent);
		setColor(QStringLiteral("ribbon"), QStringLiteral("screentip.bg"), surface);
		setColor(QStringLiteral("ribbon"), QStringLiteral("screentip.fg"), text);
		setColor(QStringLiteral("ribbon"), QStringLiteral("screentip.fg.secondary"), tertiary);
		setColor(QStringLiteral("ribbon"), QStringLiteral("screentip.border"), stroke);
		setColor(QStringLiteral("ribbon"), QStringLiteral("qat.bg"), window);
		setColor(QStringLiteral("ribbon"), QStringLiteral("backstage.bg"), surface);
		setColor(QStringLiteral("ribbon"), QStringLiteral("backstage.nav.bg"), window);
		setColor(QStringLiteral("ribbon"), QStringLiteral("backstage.fg"), text);
		setColor(QStringLiteral("ribbon"), QStringLiteral("keytip.bg"), accent);
		setColor(QStringLiteral("ribbon"), QStringLiteral("keytip.fg"), accentText);
	}
} // namespace qfluentribbon::tokens
