#include "qfluentribbon/ribbon_group.hpp"

#include <QAction>
#include <QActionGroup>
#include <QtTest>

class TestRibbonGroupActions : public QObject
{
	Q_OBJECT
private slots:
	void checkableSurvivesSimplifiedToggle();
	void exclusiveGroupSurvivesSimplifiedToggle();
};

void TestRibbonGroupActions::checkableSurvivesSimplifiedToggle()
{
	qfluentribbon::RibbonGroup group(QStringLiteral("Edit"));
	QAction* bold = group.addAction(QStringLiteral("Bold"));
	bold->setCheckable(true);
	bold->setChecked(true);

	group.setSimplified(true);
	QCOMPARE(bold->isChecked(), true);
	group.setSimplified(false);
	QCOMPARE(bold->isChecked(), true);
	QCOMPARE(group.actions().size(), 1);
}

void TestRibbonGroupActions::exclusiveGroupSurvivesSimplifiedToggle()
{
	qfluentribbon::RibbonGroup group(QStringLiteral("Align"));
	QAction* left = group.addAction(QStringLiteral("Left"));
	QAction* right = group.addAction(QStringLiteral("Right"));
	group.setExclusiveActions({left, right});
	left->setChecked(true);

	QVERIFY(group.exclusiveActionGroup());
	QCOMPARE(group.exclusiveActionGroup()->checkedAction(), left);

	group.setSimplified(true);
	QCOMPARE(left->isChecked(), true);
	QCOMPARE(right->isChecked(), false);
	QCOMPARE(group.exclusiveActionGroup()->checkedAction(), left);

	right->setChecked(true);
	group.setSimplified(false);
	QCOMPARE(right->isChecked(), true);
	QCOMPARE(left->isChecked(), false);
	QCOMPARE(group.exclusiveActionGroup()->checkedAction(), right);
}

QTEST_MAIN(TestRibbonGroupActions)
#include "test_ribbon_group_actions.moc"
