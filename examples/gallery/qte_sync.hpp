#ifndef __QFR_GALLERY_QTE_SYNC_H__
#define __QFR_GALLERY_QTE_SYNC_H__

#include <QObject>

namespace qtheme
{
	class Engine;
}

namespace qfluentribbon
{
	class ThemeBridge;
}

/// Demo-only: push QThemeEngine palette/DPI into qfluentribbon::tokens and notify ThemeBridge.
class GalleryQteSync final : public QObject
{
	Q_OBJECT
public:
	GalleryQteSync(qtheme::Engine* engine, qfluentribbon::ThemeBridge* bridge, QObject* parent = nullptr);

public slots:
	void syncFromEngine();

private:
	qtheme::Engine* m_engine = nullptr;
	qfluentribbon::ThemeBridge* m_bridge = nullptr;
};

#endif
