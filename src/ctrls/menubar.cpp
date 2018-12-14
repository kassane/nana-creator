/*
 *		ctrls::menubar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/menubar.h"
#include "filemanager.h"
#include "tokenizer/Tokenizer.h"


extern filemanager		g_file_mgr;


namespace ctrls
{

	//menubar
	menubar::menubar(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		mnb.create(*parent->nanawdg);
		ctrl::init(&mnb, CTRL_MENUBAR, name);

		// common
		properties.append("menus").label("Menus").category(CAT_COMMON).type(pg_type::collection_menubar) = "";
		// appearance
		// ...
		// layout
		// ...
	}


	void menubar::update()
	{
		ctrl::update();

		// menus - START
		mnb.clear();
		create_menu_tree();

		// Create menus
		//--------------------------------
		auto pmnb = &mnb;
		menu_tree.for_each([&pmnb](tree_node<menu_data>* node) -> bool
		{
			if(node->level() == 0)
				return true; //this is the root node

			auto* pval = &node->value;

			if(node->level() == 1)
			{
				//this is a menubar item
				if(node->child)
					pval->submenu = &(pmnb->push_back(pval->text));
				else
					pmnb->push_back(pval->text);
			}
			else
			{
				//this is a menu item
				auto* pval_parent = &node->owner->value;
				if(pval->separator)
					pval_parent->submenu->append_splitter();
				else
				{
					pval_parent->submenu->append(pval->text);

					if(!pval->img.empty())
						pval_parent->submenu->image(pval_parent->submenu->size()-1, nana::paint::image(g_file_mgr.to_relative(pval->img)));
				}

				if(node->child)
					pval->submenu = pval_parent->submenu->create_sub_menu(pval_parent->submenu->size()-1);
			}

			return true;
		});
		// menus - END
	}


	void menubar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/menubar.hpp>");
		// declaration
		cd->decl.push_back("nana::menubar " + name + ";");
		// init

		// menus - START
		//create_menu_tree(); //already created on update()

		// Generate menus code
		//--------------------------------
		menu_tree.for_each([&cd, &name](tree_node<menu_data>* node) -> bool
		{
			if(node->level() == 0)
				return true; //this is the root node

			auto* pval = &node->value;

			if(node->level() == 1)
			{
				//this is a menubar item
				if(node->child)
				{
					//TODO: il nome del menu non dovrebbe essere presente tra i vari controlli
					pval->submenu_name = name + "_" + std::to_string(node->pos());
					cd->init.push_back("auto* " + pval->submenu_name + " = &" + name + ".push_back(\"" + pval->text + "\");");
				}
				else
					cd->init.push_back(name + ".push_back(\"" + pval->text + "\");");
			}
			else
			{
				//this is a menu item
				auto* pval_parent = &node->owner->value;
				if(pval->separator)
					cd->init.push_back(pval_parent->submenu_name + "->append_splitter();");
				else
				{
					cd->init.push_back(pval_parent->submenu_name + "->append(\"" + pval->text + "\");");

					if(!pval->img.empty())
						cd->init.push_back(pval_parent->submenu_name + "->image(" + std::to_string(node->pos()) + ", nana::paint::image(\"" + g_file_mgr.to_relative(pval->img) + "\"));");
				}

				if(node->child)
				{
					pval->submenu_name = pval_parent->submenu_name + "_" + std::to_string(node->pos());
					cd->init.push_back("auto* " + pval->submenu_name + " = " + pval_parent->submenu_name + "->create_sub_menu(" + std::to_string(node->pos()) + ");");
				}
			}

			return true;
		});
	}


	void menubar::create_menu_tree()
	{
		menu_tree.clear();

		menu_data root;
		root.key = CITEM_EMPTY;
		root.text = "root";
		menu_tree.append(0, root);

		// split columns into item (delimiter = CITEM_TKN)
		Tokenizer items_tkn(properties.property("menus").as_string());
		items_tkn.setDelimiter(CITEM_TKN);

		std::string item;
		while((item = items_tkn.next()) != "")
		{
			// split item into properties (delimiter = CITEM_INNER_TKN)
			Tokenizer item_tkn(item);
			item_tkn.setDelimiter(CITEM_INNER_TKN);

			menu_data md;
			md.key = item_tkn.next();
			auto owner = item_tkn.next();
			md.text = item_tkn.next();
			if(md.text == CITEM_SEPARATOR)
			{
				md.separator = true;
			}
			else
			{
				md.img = item_tkn.next();
			}

			menu_tree.for_each([&owner, &md](tree_node<menu_data>* node) -> bool
			{
				if(owner == node->value.key)
				{
					node->append(md);
					return false;
				}
				return true;
			});
		}
	}

}//end namespace ctrls
