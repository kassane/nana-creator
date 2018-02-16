/*
 *		ctrls::progress Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROGRESS_H
#define NANA_CREATOR_PROGRESS_H

#include <nana/gui/widgets/progress.hpp>
#include "ctrls/property.h"
#include "codegenerator.h"


namespace ctrls
{

	class progress
		: public nana::progress
	{
	public:
		progress(nana::window wd, properties_collection* properties, const std::string& name);

		void update(properties_collection* properties);

		void generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci);


	protected:
		void _initproperties(properties_collection* properties, const std::string& name);
	};

}//end namespace ctrls

#endif //NANA_CREATOR_PROGRESS_H
