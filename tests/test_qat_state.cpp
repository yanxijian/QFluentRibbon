#include "qfluentribbon/quick_access_bar.hpp"

#include <QAction>
#include <QHash>
#include <QtTest>

class TestQatState : public QObject
{
	Q_OBJECT
private slots:
	void actionIdPrefersProperty();
	void resolveIdsSkipsUnknownAndDupes();
	void roundTripIds();
};

void TestQatState::actionIdPrefersProperty()
{
	QAction action(QStringLiteral("Paste"));
	action.setObjectName(QStringLiteral("obj.paste"));
	action.setProperty("qfr.actionId", QStringLiteral("clipboard.paste"));
	QCOMPARE(qfluentribbon::QuickAccessBar::actionId(&action), QStringLiteral("clipboard.paste"));
}

void TestQatState::resolveIdsSkipsUnknownAndDupes()
{
	QAction a(QStringLiteral("A"));
	a.setObjectName(QStringLiteral("a"));
	QAction b(QStringLiteral("B"));
	b.setObjectName(QStringLiteral("b"));

	QHash<QString, QAction*> catalog{{QStringLiteral("a"), &a}, {QStringLiteral("b"), &b}};
	const auto resolved = qfluentribbon::QuickAccessBar::resolveIds(
		{QStringLiteral("a"), QStringLiteral("missing"), QStringLiteral("a"), QStringLiteral("b")}, catalog);
	QCOMPARE(resolved.size(), 2);
	QCOMPARE(resolved[0], &a);
	QCOMPARE(resolved[1], &b);
}

void TestQatState::roundTripIds()
{
	QAction paste(QStringLiteral("Paste"));
	paste.setObjectName(QStringLiteral("clipboard.paste"));
	QAction bold(QStringLiteral("Bold"));
	bold.setObjectName(QStringLiteral("font.bold"));

	const QList<QAction*> pinned{&paste, &bold};
	const QStringList ids = qfluentribbon::QuickAccessBar::actionIds(pinned);
	QCOMPARE(ids, (QStringList{QStringLiteral("clipboard.paste"), QStringLiteral("font.bold")}));

	QHash<QString, QAction*> catalog{{QStringLiteral("clipboard.paste"), &paste}, {QStringLiteral("font.bold"), &bold}};
	const auto restored = qfluentribbon::QuickAccessBar::resolveIds(ids, catalog);
	QCOMPARE(restored, pinned);
}

QTEST_MAIN(TestQatState)
#include "test_qat_state.moc"
