#ifndef __QFR_RIBBON_GROUP_H__
#define __QFR_RIBBON_GROUP_H__

#include "qfluentribbon/layout/collapse.hpp"
#include "qfluentribbon/ribbon_types.hpp"

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

	/// One labeled group of QAction-backed tool buttons (Large / Medium / Small).
	class RibbonGroup final : public QWidget
	{
		Q_OBJECT
	public:
		explicit RibbonGroup(const QString& title, QWidget* parent = nullptr);

		void setTitle(const QString& title);
		[[nodiscard]] QString title() const
		{
			return m_title;
		}

		void setThemeBridge(ThemeBridge* bridge);
		[[nodiscard]] QAction* addAction(QAction* action);
		[[nodiscard]] QAction* addAction(const QString& text);
		/// Optional custom content (e.g. RibbonGallery) laid out after command buttons.
		void addWidget(QWidget* widget);
		[[nodiscard]] QList<QAction*> actions() const
		{
			return m_actions;
		}

		void setItemSize(RibbonItemSize size);
		[[nodiscard]] RibbonItemSize itemSize() const
		{
			return m_itemSize;
		}

		void setSimplified(bool simplified);
		[[nodiscard]] bool isSimplified() const
		{
			return m_simplified;
		}

		/// Office-style dialog launcher in the group title band (bottom-right).
		void setDialogLauncher(QAction* action);
		[[nodiscard]] QAction* dialogLauncher() const
		{
			return m_launcherAction;
		}

		[[nodiscard]] layout::GroupWidthHints widthHints() const;
		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;

	public slots:
		void polishFromStore();

	signals:
		void dialogLauncherClicked();

	protected:
		void paintEvent(QPaintEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;

	private:
		void rebuildButtons();
		void ensureLauncherButton();
		void relayoutButtons();
		void applyButtonStyle(QToolButton* button) const;
		[[nodiscard]] bool actionHasIcon(const QToolButton* button) const;
		[[nodiscard]] int titleBandHeight() const;
		[[nodiscard]] int contentHeight() const;
		[[nodiscard]] int launcherReserve() const;

		ThemeBridge* m_bridge = nullptr;
		QString m_title;
		RibbonItemSize m_itemSize = RibbonItemSize::Large;
		bool m_simplified = false;
		QList<QAction*> m_actions;
		QList<QToolButton*> m_buttons;
		QList<QWidget*> m_extraWidgets;
		QAction* m_launcherAction = nullptr;
		QToolButton* m_launcherButton = nullptr;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_GROUP_H__
