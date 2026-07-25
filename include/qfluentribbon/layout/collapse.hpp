#ifndef __QFR_LAYOUT_COLLAPSE_H__
#define __QFR_LAYOUT_COLLAPSE_H__

#include "qfluentribbon/ribbon_types.hpp"

#include <QVector>

namespace qfluentribbon::layout
{
	struct GroupWidthHints
	{
		int large = 0;
		int medium = 0;
		int small = 0;
	};

	/// Pick a uniform size tier so sum(widths) fits in availableWidth (padding excluded by caller).
	/// Tiers tried in order: Large → Medium → Small. Always returns one size per group.
	[[nodiscard]] QVector<RibbonItemSize> chooseUniformSizes(int availableWidth, const QVector<GroupWidthHints>& groups);
} // namespace qfluentribbon::layout

#endif // __QFR_LAYOUT_COLLAPSE_H__
