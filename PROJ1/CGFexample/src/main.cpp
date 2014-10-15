#include <iostream>
#include <exception>

//#include "CGFapplication.h"
#include "XMLSceneMod.h"
#include "TPinterface.h"
//#include "ProjectScene.h"

using std::cout;
using std::exception;


int main(int argc, char* argv[]) {

	
	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);
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