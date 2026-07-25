#include "qfluentribbon/qfluentribbon.hpp"
#include "qtheme/engine.hpp"
#include "qtheme/types.hpp"

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
	QAction* makeAction(QWidget* parent, const QString& text, QStyle::StandardPixmap icon, const QString& tip)
	{
		auto* action = new QAction(text, parent);
		action->setIcon(parent->style()->standardIcon(icon));
		action->setToolTip(tip);
		return action;
	}

	void wireGroupActions(qfluentribbon::RibbonGroup* group, QLabel* status)
	{
		for (QAction* action : group->actions())
		{
			QObject::connect(action, &QAction::triggered, status,
							 [status, action]()
							 {
								 status->setText(QStringLiteral("Triggered: %1").arg(action->text()));
							 });
		}
	}
} // namespace

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName(QStringLiteral("yanxijian"));
	QCoreApplication::setApplicationName(QStringLiteral("QFluentRibbonGallery"));

	qtheme::Engine engine;
	engine.apply(&app);

	qfluentribbon::ThemeBridge bridge;
	bridge.bind(&engine);

	qfluentribbon::RibbonWindow window;
	window.setWindowTitle(QStringLiteral("QFluentRibbon — M1 Gallery"));
	window.resize(960, 640);
	window.setThemeBridge(&bridge);

	auto* ribbon = window.ribbonBar();
	auto* home = ribbon->addTab(QStringLiteral("Home"));
	auto* insert = ribbon->addTab(QStringLiteral("Insert"));
	auto* view = ribbon->addTab(QStringLiteral("View"));

	auto* clipboard = home->addGroup(QStringLiteral("Clipboard"));
	(void)clipboard->addAction(makeAction(&window, QStringLiteral("Paste"), QStyle::SP_DialogOpenButton, QStringLiteral("Paste")));
	(void)clipboard->addAction(makeAction(&window, QStringLiteral("Cut"), QStyle::SP_DialogResetButton, QStringLiteral("Cut")));
	(void)clipboard->addAction(makeAction(&window, QStringLiteral("Copy"), QStyle::SP_FileDialogDetailedView, QStringLiteral("Copy")));

	auto* font = home->addGroup(QStringLiteral("Font"));
	(void)font->addAction(makeAction(&window, QStringLiteral("Bold"), QStyle::SP_DialogApplyButton, QStringLiteral("Bold")));
	(void)font->addAction(makeAction(&window, QStringLiteral("Italic"), QStyle::SP_DialogYesButton, QStringLiteral("Italic")));
	(void)font->addAction(makeAction(&window, QStringLiteral("Underline"), QStyle::SP_ArrowDown, QStringLiteral("Underline")));

	auto* para = home->addGroup(QStringLiteral("Paragraph"));
	(void)para->addAction(makeAction(&window, QStringLiteral("Bullets"), QStyle::SP_BrowserReload, QStringLiteral("Bullets")));
	(void)para->addAction(makeAction(&window, QStringLiteral("Align"), QStyle::SP_ArrowLeft, QStringLiteral("Align")));

	auto* tables = insert->addGroup(QStringLiteral("Tables"));
	(void)tables->addAction(makeAction(&window, QStringLiteral("Table"), QStyle::SP_FileDialogListView, QStringLiteral("Insert table")));
	(void)tables->addAction(
		makeAction(&window, QStringLiteral("Chart"), QStyle::SP_FileDialogContentsView, QStringLiteral("Insert chart")));

	auto* show = view->addGroup(QStringLiteral("Show"));
	(void)show->addAction(makeAction(&window, QStringLiteral("Grid"), QStyle::SP_ComputerIcon, QStringLiteral("Toggle grid")));
	(void)show->addAction(makeAction(&window, QStringLiteral("Ruler"), QStyle::SP_DesktopIcon, QStringLiteral("Toggle ruler")));

	auto* status = new QLabel(QStringLiteral("Ready — click a ribbon command"), &window);
	for (qfluentribbon::RibbonTab* tab : {home, insert, view})
	{
		for (qfluentribbon::RibbonGroup* group : tab->groups())
		{
			wireGroupActions(group, status);
		}
	}

	auto* central = new QWidget(&window);
	auto* root = new QVBoxLayout(central);

	auto* row = new QHBoxLayout();
	row->addWidget(new QLabel(QStringLiteral("Skin:"), central));
	auto* skinBox = new QComboBox(central);
	skinBox->addItem(QStringLiteral("Fluent Light"), static_cast<int>(qtheme::ColorScheme::Light));
	skinBox->addItem(QStringLiteral("Fluent Dark"), static_cast<int>(qtheme::ColorScheme::Dark));
	skinBox->addItem(QStringLiteral("Fluent High Contrast"), static_cast<int>(qtheme::ColorScheme::HighContrast));
	row->addWidget(skinBox, 1);
	root->addLayout(row);

	auto* body =
		new QLabel(QStringLiteral("Content area\n\n"
								  "M1: switch tabs, click commands, then narrow the window to see Large → Medium → Small collapse.\n"
								  "Chrome colors still come from ThemeStore (no Ribbon QSS)."),
				   central);
	body->setAlignment(Qt::AlignCenter);
	body->setWordWrap(true);
	root->addWidget(body, 1);
	root->addWidget(status);
	window.setCentralWidget(central);

	const auto syncCombo = [&]()
	{
		const int scheme = static_cast<int>(engine.colorScheme());
		const int idx = skinBox->findData(scheme);
		if (idx >= 0)
		{
			skinBox->setCurrentIndex(idx);
		}
	};
	syncCombo();

	QObject::connect(skinBox, QOverload<int>::of(&QComboBox::activated), &window,
					 [&](int index)
					 {
						 const auto scheme = static_cast<qtheme::ColorScheme>(skinBox->itemData(index).toInt());
						 (void)engine.setColorScheme(scheme);
						 syncCombo();
					 });

	window.show();
	return app.exec();
}
