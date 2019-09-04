#include "..\HeaderFiles\FormatterGUI.h"
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>

void FormateGUI::RunNana()
{
	nana::form fm;
	nana::label lab(fm, "Hello, World!" );
	nana::button btn(fm, "Quit");
	btn.events().click([&fm] {
		fm.close();
	});

	fm.div("vert<text><button>");
	fm["text"] << lab;
	fm["button"] << btn;
	fm.collocate();

	fm.show();
	nana::exec();
}
