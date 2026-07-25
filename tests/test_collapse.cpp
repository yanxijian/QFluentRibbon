#include "qfluentribbon/layout/collapse.hpp"

#include <QtTest>

class TestCollapse : public QObject
{
	Q_OBJECT
private slots:
	void emptyGroups();
	void staysLargeWhenWide();
	void dropsToMedium();
	void dropsToSmall();
};

void TestCollapse::emptyGroups()
{
	const auto sizes = qfluentribbon::layout::chooseUniformSizes(400, {});
	QCOMPARE(sizes.size(), 0);
}

void TestCollapse::staysLargeWhenWide()
{
	using qfluentribbon::RibbonItemSize;
	using qfluentribbon::layout::GroupWidthHints;

	QVector<GroupWidthHints> groups{{100, 60, 40}, {100, 60, 40}};
	const auto sizes = qfluentribbon::layout::chooseUniformSizes(250, groups);
	QCOMPARE(sizes.size(), 2);
	QCOMPARE(sizes[0], RibbonItemSize::Large);
	QCOMPARE(sizes[1], RibbonItemSize::Large);
}

void TestCollapse::dropsToMedium()
{
	using qfluentribbon::RibbonItemSize;
	using qfluentribbon::layout::GroupWidthHints;

	QVector<GroupWidthHints> groups{{120, 70, 40}, {120, 70, 40}};
	// Large=240 needs >200; Medium=140 fits.
	const auto sizes = qfluentribbon::layout::chooseUniformSizes(200, groups);
	QCOMPARE(sizes.size(), 2);
	QCOMPARE(sizes[0], RibbonItemSize::Medium);
	QCOMPARE(sizes[1], RibbonItemSize::Medium);
}

void TestCollapse::dropsToSmall()
{
	using qfluentribbon::RibbonItemSize;
	using qfluentribbon::layout::GroupWidthHints;

	QVector<GroupWidthHints> groups{{120, 80, 40}, {120, 80, 40}};
	// Large=240, Medium=160, Small=80 — only Small fits in 100.
	const auto sizes = qfluentribbon::layout::chooseUniformSizes(100, groups);
	QCOMPARE(sizes.size(), 2);
	QCOMPARE(sizes[0], RibbonItemSize::Small);
	QCOMPARE(sizes[1], RibbonItemSize::Small);
}

QTEST_APPLESS_MAIN(TestCollapse)
#include "test_collapse.moc"
