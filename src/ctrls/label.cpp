/*
 *		ctrls::label Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/label.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//label
	label::label(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::label(wd, CTRL_LABEL)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void label::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;
		
		// format and caption
		if(properties->property("format").as_bool())
		{
			try
			{
				caption(properties->property("caption").as_string());
				format(true);
			}
			catch(...)
			{
				nana::msgbox m(0, CREATOR_NAME, nana::msgbox::ok);
				m.icon(m.icon_warning);
				m << "Errore formato!"; //TODO
				m();

				format(false);
				caption(properties->property("caption").as_string());
			}
		}
		else
		{
			format(false);
			caption(properties->property("caption").as_string());
		}
		//
		enabled(properties->property("enabled").as_bool());
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
		transparent(properties->property("transparent").as_bool());
		text_align(static_cast<nana::align>(properties->property("halign").as_int()),
			static_cast<nana::align_v>(properties->property("valign").as_int()));
	}


	void label::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/label.hpp>");

		auto name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::label " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		cd->init.push_back(name + ".caption(\"" + properties->property("caption").as_string() + "\");");
		cd->init.push_back(name + ".format(" + properties->property("format").as_string() + ");");
		cd->init.push_back(name + ".enabled(" + properties->property("enabled").as_string() + ");");
		// color
		bool inherited;
		std::string col;
		// bg
		col = properties->property("bgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + ".bgcolor(nana::color(" + col + "));");
		// fg
		col = properties->property("fgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + ".fgcolor(nana::color(" + col + "));");
		//
		cd->init.push_back(name + ".transparent(" + properties->property("transparent").as_string() + ");");
		cd->init.push_back(name + ".text_align(static_cast<nana::align>(" + properties->property("halign").as_string() + "), static_cast<nana::align_v>(" + properties->property("valign").as_string() + "));");

		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void label::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_LABEL;
		properties->append("name") = name;
		// common
		properties->append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "Label";
		properties->append("format").label("Format").category(CAT_COMMON).type(pg_type::check) = "false";
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), true);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), true);
		properties->append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = transparent();
		properties->append("halign").label("Horizontal Alignment").category(CAT_APPEARANCE).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_LEFT, CITEM_CENTER, CITEM_RIGHT }) = static_cast<int>(nana::align::left);
		properties->append("valign").label("Vertical Alignment").category(CAT_APPEARANCE).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_TOP, CITEM_CENTER, CITEM_BOTTOM }) = static_cast<int>(nana::align_v::top);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
