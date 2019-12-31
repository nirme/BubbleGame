#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Renderable.h"
#include "MaterialManager.h"


namespace core
{
	namespace _2d
	{

		class RenderQueue
		{
		public:
			typedef std::vector<const Renderable*> RenderableList;
			typedef std::unordered_map<MaterialId, RenderableList> RenderableListByMaterial;
			typedef std::unordered_map<Priority, RenderableListByMaterial> RenderableGroupMap;
			typedef std::unordered_set<Priority> RenderPrioritySet;

		protected:
			RenderableGroupMap renderableMap;
			RenderPrioritySet usedPrioritySet;

			bool isPriorityListSorted;

			std::list<Priority> sortedPriorityList;
			std::list<Priority>::iterator priorityIterator;


			void sortByPriority()
			{
				if (usedPrioritySet.size())
				{
					sortedPriorityList.assign(usedPrioritySet.begin(), usedPrioritySet.end());
					sortedPriorityList.sort();
				}
				else
					sortedPriorityList.clear();

				isPriorityListSorted = true;
				priorityIterator = sortedPriorityList.begin();
			};


		public:

			RenderQueue() :
				isPriorityListSorted(false)
			{
				priorityIterator = sortedPriorityList.end();
			};

			void addRenderable(const Renderable* _rend)
			{
				Priority pr = _rend->getPriority();
				renderableMap[pr][_rend->getMaterialId()].push_back(_rend);
				usedPrioritySet.insert(pr);
			};

			void clear()
			{
				isPriorityListSorted = false;
				priorityIterator = sortedPriorityList.end();

				//clear the queue without removing containers
				for (auto it = renderableMap.begin(); it != renderableMap.end(); ++it)
					for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
						(*it2).second.clear();
					
				usedPrioritySet.clear();
			};


			void resetPriorityIterator()
			{
				if (!isPriorityListSorted)
					sortByPriority();
				priorityIterator = sortedPriorityList.begin();
			};


			const RenderableListByMaterial* getNextPriorityRenderableList()
			{
				if (!isPriorityListSorted)
					sortByPriority();

				if (priorityIterator == sortedPriorityList.end())
					return nullptr;

				const RenderableListByMaterial* list = &(renderableMap[(*priorityIterator)]);
				++priorityIterator;

				return list;
			};





		};
	}
}
