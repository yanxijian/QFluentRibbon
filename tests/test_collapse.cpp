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
	void respectsMaxSizeMedium();
	void widensBackToLarge();
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

void TestCollapse::respectsMaxSizeMedium()
{
	using qfluentribbon::RibbonItemSize;
	using qfluentribbon::layout::GroupWidthHints;

	QVector<GroupWidthHints> groups{{100, 60, 40}, {100, 60, 40}};
	// Wide enough for Large, but capped at Medium (simplified mode).
	const auto sizes = qfluentribbon::layout::chooseUniformSizes(400, groups, RibbonItemSize::Medium);
	QCOMPARE(sizes.size(), 2);
	QCOMPARE(sizes[0], RibbonItemSize::Medium);
	QCOMPARE(sizes[1], RibbonItemSize::Medium);
}

void TestCollapse::widensBackToLarge()
{
	using qfluentribbon::RibbonItemSize;
	using qfluentribbon::layout::GroupWidthHints;

	QVector<GroupWidthHints> groups{{120, 70, 40}, {120, 70, 40}};
	const auto narrow = qfluentribbon::layout::chooseUniformSizes(100, groups);
	QCOMPARE(narrow[0], RibbonItemSize::Small);
	const auto wide = qfluentribbon::layout::chooseUniformSizes(300, groups);
	QCOMPARE(wide[0], RibbonItemSize::Large);
	QCOMPARE(wide[1], RibbonItemSize::Large);
}

QTEST_APPLESS_MAIN(TestCollapse)
#include "test_collapse.moc"
