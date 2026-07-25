#include "qfluentribbon/qfluentribbon.hpp"
#include "qtheme/engine.hpp"
#include "qtheme/types.hpp"

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QHash>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
	QAction* makeAction(QWidget* parent, const QString& id, const QString& text, QStyle::StandardPixmap icon, const QString& tipBody)
	{
		auto* action = new QAction(text, parent);
		action->setObjectName(id);
		action->setIcon(parent->style()->standardIcon(icon));
		qfluentribbon::ScreenTip::set(action, text, tipBody);
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
	window.setWindowTitle(QStringLiteral("QFluentRibbon — M3 Gallery"));
	window.resize(960, 640);
	window.setThemeBridge(&bridge);

	auto* ribbon = window.ribbonBar();
	auto* qat = ribbon->quickAccessBar();
	auto* home = ribbon->addTab(QStringLiteral("Home"));
	auto* insert = ribbon->addTab(QStringLiteral("Insert"));
	auto* view = ribbon->addTab(QStringLiteral("View"));

	auto* paste = makeAction(&window, QStringLiteral("clipboard.paste"), QStringLiteral("Paste"), QStyle::SP_DialogOpenButton,
							 QStringLiteral("Paste clipboard contents."));
	auto* cut = makeAction(&window, QStringLiteral("clipboard.cut"), QStringLiteral("Cut"), QStyle::SP_DialogResetButton,
						   QStringLiteral("Cut the selection to the clipboard."));
	auto* copy = makeAction(&window, QStringLiteral("clipboard.copy"), QStringLiteral("Copy"), QStyle::SP_FileDialogDetailedView,
							QStringLiteral("Copy the selection."));
	auto* bold = makeAction(&window, QStringLiteral("font.bold"), QStringLiteral("Bold"), QStyle::SP_DialogApplyButton,
							QStringLiteral("Make text bold."));
	auto* italic = makeAction(&window, QStringLiteral("font.italic"), QStringLiteral("Italic"), QStyle::SP_DialogYesButton,
							  QStringLiteral("Italicize text."));
	auto* underline = makeAction(&window, QStringLiteral("font.underline"), QStringLiteral("Underline"), QStyle::SP_ArrowDown,
								 QStringLiteral("Underline the selection."));
	auto* bullets = makeAction(&window, QStringLiteral("para.bullets"), QStringLiteral("Bullets"), QStyle::SP_BrowserReload,
							   QStringLiteral("Start a bulleted list."));
	auto* align = makeAction(&window, QStringLiteral("para.align"), QStringLiteral("Align"), QStyle::SP_ArrowLeft,
							 QStringLiteral("Change paragraph alignment."));
	auto* table = makeAction(&window, QStringLiteral("insert.table"), QStringLiteral("Table"), QStyle::SP_FileDialogListView,
							 QStringLiteral("Insert a table."));
	auto* chart = makeAction(&window, QStringLiteral("insert.chart"), QStringLiteral("Chart"), QStyle::SP_FileDialogContentsView,
							 QStringLiteral("Insert a chart."));
	auto* grid = makeAction(&window, QStringLiteral("view.grid"), QStringLiteral("Grid"), QStyle::SP_ComputerIcon,
							QStringLiteral("Toggle the grid."));
	auto* ruler = makeAction(&window, QStringLiteral("view.ruler"), QStringLiteral("Ruler"), QStyle::SP_DesktopIcon,
							 QStringLiteral("Toggle the ruler."));

	auto* clipboard = home->addGroup(QStringLiteral("Clipboard"));
	(void)clipboard->addAction(paste);
	(void)clipboard->addAction(cut);
	(void)clipboard->addAction(copy);
	auto* clipboardDialog = new QAction(QStringLiteral("Clipboard Options"), &window);
	clipboardDialog->setObjectName(QStringLiteral("clipboard.options"));
	qfluentribbon::ScreenTip::set(clipboardDialog, QStringLiteral("Clipboard Options"),
								  QStringLiteral("Open clipboard settings (group launcher)."));
	clipboard->setDialogLauncher(clipboardDialog);

	auto* font = home->addGroup(QStringLiteral("Font"));
	(void)font->addAction(bold);
	(void)font->addAction(italic);
	(void)font->addAction(underline);
	auto* fontDialog = new QAction(QStringLiteral("Font Dialog"), &window);
	fontDialog->setObjectName(QStringLiteral("font.dialog"));
	qfluentribbon::ScreenTip::set(fontDialog, QStringLiteral("Font Dialog"), QStringLiteral("Open the font dialog (group launcher)."));
	font->setDialogLauncher(fontDialog);

	auto* para = home->addGroup(QStringLiteral("Paragraph"));
	(void)para->addAction(bullets);
	(void)para->addAction(align);

	auto* tables = insert->addGroup(QStringLiteral("Tables"));
	(void)tables->addAction(table);
	(void)tables->addAction(chart);

	auto* show = view->addGroup(QStringLiteral("Show"));
	(void)show->addAction(grid);
	(void)show->addAction(ruler);

	QHash<QString, QAction*> catalog;
	for (QAction* action : {paste, cut, copy, bold, italic, underline, bullets, align, table, chart, grid, ruler})
	{
		catalog.insert(action->objectName(), action);
	}

	auto* status = new QLabel(QStringLiteral("Ready — QAT above tabs; right-click QAT to unpin"), &window);
	for (qfluentribbon::RibbonTab* tab : {home, insert, view})
	{
		for (qfluentribbon::RibbonGroup* group : tab->groups())
		{
			wireGroupActions(group, status);
		}
	}

	QSettings settings;
	QObject::connect(qat, &qfluentribbon::QuickAccessBar::actionsChanged, &window,
					 [qat, status, &settings]()
					 {
						 qat->saveState(settings);
						 status->setText(QStringLiteral("QAT updated (%1 pinned) — saved to QSettings").arg(qat->actions().size()));
					 });

	const int restored = qat->restoreState(settings, catalog);
	if (restored == 0)
	{
		(void)qat->addAction(paste);
		(void)qat->addAction(bold);
	}

	QObject::connect(clipboardDialog, &QAction::triggered, &window,
					 [status]()
					 {
						 status->setText(QStringLiteral("Dialog launcher: Clipboard Options"));
					 });
	QObject::connect(fontDialog, &QAction::triggered, &window,
					 [&window, status]()
					 {
						 status->setText(QStringLiteral("Dialog launcher: Font Dialog"));
						 QMessageBox::information(&window, QStringLiteral("Font"), QStringLiteral("Font dialog placeholder."));
					 });

	auto* central = new QWidget(&window);
	auto* root = new QVBoxLayout(central);

	auto* row = new QHBoxLayout();
	row->addWidget(new QLabel(QStringLiteral("Skin:"), central));
	auto* skinBox = new QComboBox(central);
	skinBox->addItem(QStringLiteral("Fluent Light"), static_cast<int>(qtheme::ColorScheme::Light));
	skinBox->addItem(QStringLiteral("Fluent Dark"), static_cast<int>(qtheme::ColorScheme::Dark));
	skinBox->addItem(QStringLiteral("Fluent High Contrast"), static_cast<int>(qtheme::ColorScheme::HighContrast));
	row->addWidget(skinBox, 1);

	auto* simplified = new QCheckBox(QStringLiteral("Simplified ribbon"), central);
	row->addWidget(simplified);
	root->addLayout(row);

	auto* pinRow = new QHBoxLayout();
	pinRow->addWidget(new QLabel(QStringLiteral("Pin to QAT:"), central));
	auto* pinCopy = new QPushButton(QStringLiteral("Copy"), central);
	auto* pinGrid = new QPushButton(QStringLiteral("Grid"), central);
	auto* clearQat = new QPushButton(QStringLiteral("Clear QAT"), central);
	pinRow->addWidget(pinCopy);
	pinRow->addWidget(pinGrid);
	pinRow->addWidget(clearQat);
	pinRow->addStretch(1);
	root->addLayout(pinRow);

	QObject::connect(pinCopy, &QPushButton::clicked, &window,
					 [qat, copy, status]()
					 {
						 if (qat->addAction(copy))
						 {
							 status->setText(QStringLiteral("Pinned Copy to QAT"));
						 }
						 else
						 {
							 status->setText(QStringLiteral("Copy already on QAT"));
						 }
					 });
	QObject::connect(pinGrid, &QPushButton::clicked, &window,
					 [qat, grid, status]()
					 {
						 if (qat->addAction(grid))
						 {
							 status->setText(QStringLiteral("Pinned Grid to QAT"));
						 }
						 else
						 {
							 status->setText(QStringLiteral("Grid already on QAT"));
						 }
					 });
	QObject::connect(clearQat, &QPushButton::clicked, qat, &qfluentribbon::QuickAccessBar::clear);

	auto* body = new QLabel(QStringLiteral("Content area\n\n"
										   "M3: Quick Access Toolbar sits above the tabs.\n"
										   "Pin via buttons below; right-click a QAT icon to remove.\n"
										   "Pinned set is stored in QSettings and restored on next launch."),
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
	QObject::connect(simplified, &QCheckBox::toggled, ribbon, &qfluentribbon::RibbonBar::setSimplified);

	window.show();
	const int rc = app.exec();
	qat->saveState(settings);
	return rc;
}
