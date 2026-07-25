#ifndef __QFR_QUICK_ACCESS_BAR_H__
#define __QFR_QUICK_ACCESS_BAR_H__

#include <QHash>
#include <QList>
#include <QString>
#include <QStringList>
#include <QWidget>

class QAction;
class QContextMenuEvent;
class QPaintEvent;
class QResizeEvent;
class QSettings;
class QToolButton;

namespace qfluentribbon
{
	class ThemeBridge;

	/// Compact icon strip of pinned QActions (Office Quick Access Toolbar).
	/// Position for M3: above the tab row inside RibbonBar.
	class QuickAccessBar final : public QWidget
	{
		Q_OBJECT
	public:
		explicit QuickAccessBar(QWidget* parent = nullptr);

		void setThemeBridge(ThemeBridge* bridge);

		[[nodiscard]] bool addAction(QAction* action);
		bool removeAction(QAction* action);
		[[nodiscard]] bool contains(QAction* action) const;
		[[nodiscard]] QList<QAction*> actions() const
		{
			return m_actions;
		}
		void clear();

		/// Persist pinned action ids (`objectName()`, falling back to `qfr.actionId` property).
		void saveState(QSettings& settings, const QString& key = QStringLiteral("quickAccess")) const;
		/// Restore from settings using a catalog keyed by the same ids.
		int restoreState(QSettings& settings, const QHash<QString, QAction*>& catalog, const QString& key = QStringLiteral("quickAccess"));

		[[nodiscard]] static QString actionId(const QAction* action);
		[[nodiscard]] static QStringList actionIds(const QList<QAction*>& actions);
		[[nodiscard]] static QList<QAction*> resolveIds(const QStringList& ids, const QHash<QString, QAction*>& catalog);

		[[nodiscard]] QSize sizeHint() const override;
		[[nodiscard]] QSize minimumSizeHint() const override;

	public slots:
		void polishFromStore();

	signals:
		void actionsChanged();

	protected:
		void paintEvent(QPaintEvent* event) override;
		void resizeEvent(QResizeEvent* event) override;
		void contextMenuEvent(QContextMenuEvent* event) override;

	private:
		void rebuildButtons();
		void relayoutButtons();
		[[nodiscard]] int barHeight() const;

		ThemeBridge* m_bridge = nullptr;
		QList<QAction*> m_actions;
		QList<QToolButton*> m_buttons;
	};
} // namespace qfluentribbon

#endif // __QFR_QUICK_ACCESS_BAR_H__
