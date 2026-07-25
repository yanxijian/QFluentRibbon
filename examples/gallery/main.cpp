#include "qfluentribbon/qfluentribbon.hpp"

#include "qtheme/engine.hpp"
#include "qtheme/types.hpp"

#include <QApplication>
#include <QComboBox>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

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
	window.setWindowTitle(QStringLiteral("QFluentRibbon — M0 Gallery"));
	window.resize(960, 640);
	window.setThemeBridge(&bridge);
	window.ribbonBar()->setStatusText(QStringLiteral("RibbonBar (M0) — change skin below to verify ThemeStore-driven chrome"));

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

	auto* body = new QLabel(
		QStringLiteral("Content area\n\nM0 acceptance: Ribbon strip follows QTE light / dark / hc.\n"
					   "No Ribbon-private QSS. Tabs / groups arrive in M1."),
		central);
	body->setAlignment(Qt::AlignCenter);
	body->setWordWrap(true);
	root->addWidget(body, 1);
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
