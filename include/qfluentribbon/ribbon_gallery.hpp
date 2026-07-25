#ifndef __QFR_RIBBON_GALLERY_H__
#define __QFR_RIBBON_GALLERY_H__

#include <QIcon>
#include <QList>
#include <QString>
#include <QWidget>

class QAction;
class QPaintEvent;
class QResizeEvent;
class QToolButton;

namespace qfluentribbon
{
	class ThemeBridge;

	/// Compact in-ribbon gallery subset: horizontal icon+label items that trigger QActions.
	class RibbonGallery final : public QWidget
	{
		Q_OBJECT
	public:
		explicit RibbonGallery(QWidget* parent = nullptr);

		void setThemeBridge(ThemeBridge* bridge);

		/// Adds an item; action must outlive the gallery (or be parented elsewhere).
		int addItem(const QString& text, const QIcon& icon, QAction* action = nullptr);
		[[nodiscard]] int itemCount() const
		{
			return m_items.size();
		}

		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;

	public slots:
		void polishFromStore();

	signals:
		void itemClicked(int index);

	protected:
		void paintEvent(QPaintEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;

	private:
		struct Item
		{
			QString text;
			QIcon icon;
			QAction* action = nullptr;
			QToolButton* button = nullptr;
		};

		void rebuild();
		void relayout();
		[[nodiscard]] int itemWidth() const;
		[[nodiscard]] int itemHeight() const;

		ThemeBridge* m_bridge = nullptr;
		QList<Item> m_items;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_GALLERY_H__
