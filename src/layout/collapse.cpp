#include "qfluentribbon/layout/collapse.hpp"

#include <QtGlobal>

namespace qfluentribbon::layout
{
	namespace
	{
		int sumFor(RibbonItemSize size, const QVector<GroupWidthHints>& groups)
		{
			int total = 0;
			for (const GroupWidthHints& g : groups)
			{
				switch (size)
				{
				case RibbonItemSize::Large:
					total += g.large;
					break;
				case RibbonItemSize::Medium:
					total += g.medium;
					break;
				case RibbonItemSize::Small:
					total += g.small;
					break;
				}
			}
			return total;
		}

		QVector<RibbonItemSize> fill(RibbonItemSize size, int count)
		{
			return QVector<RibbonItemSize>(count, size);
		}
	} // namespace

	QVector<RibbonItemSize> chooseUniformSizes(int availableWidth, const QVector<GroupWidthHints>& groups)
	{
		const int n = groups.size();
		if (n == 0)
		{
			return {};
		}
		const int budget = qMax(0, availableWidth);
		if (sumFor(RibbonItemSize::Large, groups) <= budget)
		{
			return fill(RibbonItemSize::Large, n);
		}
		if (sumFor(RibbonItemSize::Medium, groups) <= budget)
		{
			return fill(RibbonItemSize::Medium, n);
		}
		return fill(RibbonItemSize::Small, n);
	}
} // namespace qfluentribbon::layout
