#ifndef __QFR_RIBBON_TAB_H__
#define __QFR_RIBBON_TAB_H__

#include <QList>
#include <QString>
#include <QWidget>
#include <qfluentribbon/qfluentribbon_export.hpp>

class QResizeEvent;
class QShowEvent;

namespace qfluentribbon
{
	class RibbonGroup;
	class ThemeBridge;

	/// One ribbon page: horizontal row of RibbonGroup widgets.
	class QFR_EXPORT RibbonTab final : public QWidget
	{
		Q_OBJECT
	public:
		explicit RibbonTab(const QString& title, QWidget* parent = nullptr);

		void setTitle(const QString& title);
		[[nodiscard]] QString title() const
		{
			return m_title;
		}

		void setThemeBridge(ThemeBridge* bridge);
		[[nodiscard]] RibbonGroup* addGroup(const QString& title);
		[[nodiscard]] QList<RibbonGroup*> groups() const
		{
			return m_groups;
		}

		void setSimplified(bool simplified);
		[[nodiscard]] bool isSimplified() const
		{
			return m_simplified;
		}

		void applyCollapse(int availableWidth);

	public slots:
		void polishFromStore();

	protected:
		void resizeEvent(QResizeEvent* event) override;
		void showEvent(QShowEvent* event) override;

	private:
		void relayout();

		ThemeBridge* m_bridge = nullptr;
		QString m_title;
		QList<RibbonGroup*> m_groups;
		bool m_simplified = false;
	};
} // namespace qfluentribbon

#endif // __QFR_RIBBON_TAB_H__
