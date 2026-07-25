#include "qfluentribbon/layout/collapse.hpp"

#include <QtGlobal>

namespace qfluentribbon::layout
{
	namespace
	{
		int widthOf(RibbonItemSize size, const GroupWidthHints& g)
		{
			switch (size)
			{
			case RibbonItemSize::Large:
				return g.large;
			case RibbonItemSize::Medium:
				return g.medium;
			case RibbonItemSize::Small:
				return g.small;
			}
			return g.small;
		}

		int sumFor(RibbonItemSize size, const QVector<GroupWidthHints>& groups)
		{
			int total = 0;
			for (const GroupWidthHints& g : groups)
			{
				total += widthOf(size, g);
			}
			return total;
		}

		QVector<RibbonItemSize> fill(RibbonItemSize size, int count)
		{
			return QVector<RibbonItemSize>(count, size);
		}
	} // namespace

	QVector<RibbonItemSize> chooseUniformSizes(int availableWidth, const QVector<GroupWidthHints>& groups, RibbonItemSize maxSize)
	{
		const int n = groups.size();
		if (n == 0)
		{
			return {};
		}
		const int budget = qMax(0, availableWidth);

		// Try largest allowed tier first, then step down (Large=0 … Small=2).
		const RibbonItemSize candidates[] = {RibbonItemSize::Large, RibbonItemSize::Medium, RibbonItemSize::Small};
		for (RibbonItemSize size : candidates)
		{
			if (static_cast<int>(size) < static_cast<int>(maxSize))
			{
				continue;
			}
			if (sumFor(size, groups) <= budget)
			{
				return fill(size, n);
			}
		}
		return fill(RibbonItemSize::Small, n);
	}
} // namespace qfluentribbon::layout
