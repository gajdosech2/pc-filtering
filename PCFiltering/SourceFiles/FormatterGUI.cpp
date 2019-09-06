#include "..\HeaderFiles\FormatterGUI.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/spinbox.hpp>

void FormateGUI::RunNana()
{
	nana::form fm;
	fm.caption("Formatter");

	nana::textbox file(fm);
	file.caption("file path");

	nana::checkbox trim(fm);
	trim.caption("trim");

	nana::spinbox pad(fm);
	pad.range(0, 2048, 64);
	nana::label plabel(fm);
	plabel.caption("  pad size");

	nana::checkbox image(fm);
	image.caption("generate images");

	nana::checkbox data(fm);
	data.caption("generate data files");

	nana::spinbox size(fm);
	size.range(3, 25, 2);
	nana::label slabel(fm);
	slabel.caption("  tile size");

	nana::textbox truth(fm);
	truth.caption("truth path");

	nana::button run(fm);
	run.caption("RUN!");

	run.events().click([&file, &size, &trim, &pad, &image, &data, &truth] {
		std::cout << file.caption() << std::endl;
		std::cout << size.value() << std::endl;
		std::cout << trim.checked() << std::endl;
		std::cout << pad.value() << std::endl;
		std::cout << image.checked() << std::endl;
		std::cout << data.checked() << std::endl;
		std::cout << truth.caption() << std::endl;
		NeuralNetworkFormatter formatter;
		formatter.Import(file.caption());
		if (trim.checked()) {
			formatter.Trim();
		}
		if (pad.to_int() > 0) {
			formatter.Pad(pad.to_int());
		}
		if (image.checked()) {
			formatter.GenerateImageFiles();
		}
		if (data.checked()) {
			formatter.GenerateDataFiles(size.to_int());
		}
		if (truth.caption().size() > 0 && truth.caption() != "truth path") {
			formatter.GenerateTruthFile(truth.caption(), size.to_int());
		}
		std::cout << "DONE!" << std::endl;
		});

	fm.div("vert<file><trim>hor<<pad><plabel>><image><data>hor<<size><slabel>><truth><run>");
	fm["file"] << file;
	fm["trim"] << trim;
	fm["pad"] << pad;
	fm["plabel"] << plabel;
	fm["image"] << image;
	fm["size"] << size;
	fm["data"] << data;
	fm["slabel"] << slabel;
	fm["truth"] << truth;
	fm["run"] << run;
	fm.collocate();
	fm.show();
	nana::exec();
}
