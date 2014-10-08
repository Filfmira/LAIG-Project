

#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "DemoScene.h"
#include "XMLScene.h"
#include "XMLSceneMod.h"
#include "TPinterface.h"
#include "ProjectScene.h"

using std::cout;
using std::exception;


int main(int argc, char* argv[]) {
	//XMLScene temp = XMLScene("demograph.xml");
	/*CGFapplication app = CGFapplication();
	app.init(&argc, argv);
	XMLSceneMod temp = XMLSceneMod("anfFile.xml");
	cin.get();*/


	
	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		//app.setScene(new DemoScene());
		//app.setInterface(new CGFinterface());
		app.setScene(new ProjectScene());
		app.setInterface(new TPinterface());


		app.run();
	}
	catch(GLexception& ex) {
		cout << "Error: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Unexpected error: " << ex.what();
		return -1;
	}

	

	return 0;
}