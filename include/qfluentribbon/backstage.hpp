#ifndef __QFR_BACKSTAGE_H__
#define __QFR_BACKSTAGE_H__

#include <QList>
#include <QString>
#include <QWidget>
#include <qfluentribbon/qfluentribbon_export.hpp>

class QKeyEvent;
class QListWidget;
class QPaintEvent;
class QResizeEvent;
class QShowEvent;
class QStackedWidget;
class QToolButton;

namespace qfluentribbon
{
	class ThemeBridge;

	/// Full-content overlay with left navigation + stacked pages (Office Backstage-lite).
	class QFR_EXPORT Backstage final : public QWidget
	{
		Q_OBJECT
	public:
		explicit Backstage(QWidget* parent = nullptr);

		void setThemeBridge(ThemeBridge* bridge);

		/// Takes ownership of page (reparents into the stack). Returns page index.
		int addPage(const QString& title, QWidget* page);
		[[nodiscard]] int pageCount() const;
		[[nodiscard]] int currentIndex() const;
		void setCurrentIndex(int index);

		void open();
		/// Hide the overlay (named dismiss to avoid clashing with QWidget::close).
		void dismiss();
		[[nodiscard]] bool isOpen() const;

	public slots:
		void polishFromStore();

	signals:
		void opened();
		void closed();
		void currentChanged(int index);

	protected:
		void paintEvent(QPaintEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;
		void keyPressEvent(QKeyEvent* event) override;
		void showEvent(QShowEvent* event) override;

	private:
		void rebuildChrome();
		[[nodiscard]] int navWidth() const;

		ThemeBridge* m_bridge = nullptr;
		QToolButton* m_backButton = nullptr;
		QListWidget* m_nav = nullptr;
		QStackedWidget* m_stack = nullptr;
		QList<QWidget*> m_pages;
	};
} // namespace qfluentribbon

#endif // __QFR_BACKSTAGE_H__
