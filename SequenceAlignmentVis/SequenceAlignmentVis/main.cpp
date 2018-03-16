#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include <Windows.h>
#include "IndexArray.h"
#include "DPTable.h"
#include "Aligner.h"
#include "Engine.h"
#include "Visualizer.h"
#include "Textbox.h"
#include "RadioButton.h"
#include "MessageBox.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


using namespace std;
using namespace glm;

int setupMenu(Engine& e,Aligner& aligner);
SettingsScene<Visualizer*>* setupSettings(Engine& e,Aligner& aligner, int menuInd);
bool isInteger(string str);

int main(int argc, char** argv) {
	vector<char> alphabet = { 'A','B','C' };
	Engine e(1024,768,"Visualization");
	Aligner aligner(alphabet, 5.0f, -3.0f, -2.0f);
	int menuInd = setupMenu(e,aligner);
	e.changeScene(menuInd);
	e.run();
}

SettingsScene<Visualizer*>* setupSettings(Engine& e, Aligner& aligner, int menuInd) {
	SettingsScene<Visualizer*>* settings = new SettingsScene<Visualizer*>(e.getDisplay(), e);
	TextBox t1(0, 7.5f, 15, 1.5f, e, settings);
	TextBox t2(0, 5.5f, 15, 1.5f, e, settings);
	TextBox t3(0, 3.5f, 15, 1.5f, e, settings);
	TextBox speed(-6.0f, 1.5f, 3, 1.5f, e, settings);
	speed.changeText("5");
	int t1Ind = settings->addObject(t1);
	int t2Ind = settings->addObject(t2);
	int t3Ind = settings->addObject(t3);
	int speedInd = settings->addObject(speed);
	RadioButton<AlignmentType> global(7.0f, 0.5f, "Global", e.getTextShader(), GLOBAL_ALIGNMENT);
	RadioButton<AlignmentType> freeEnds(7.0f, -1.0f, "Free ends", e.getTextShader(), FREE_ENDS);
	RadioButton<AlignmentType> local(7.0f, -2.5f, "Local", e.getTextShader(), LOCAL_ALIGNMENT);
	int ind1 = settings->addObject(global);
	int ind2 = settings->addObject(freeEnds);
	int ind3 = settings->addObject(local);
	shared_ptr<RadioArray<AlignmentType>> arr = make_shared<RadioArray<AlignmentType>>();
	arr->setPtr(arr);
	arr->addRadioButton(&(static_cast<RadioButton<AlignmentType>&>(settings->getObject(ind1))));
	arr->addRadioButton(&(static_cast<RadioButton<AlignmentType>&>(settings->getObject(ind2))));
	arr->addRadioButton(&(static_cast<RadioButton<AlignmentType>&>(settings->getObject(ind3))));
	settings->addButton(vec3(0, 0.5, 1), 1, -6, 5, 1, "Return", [menuInd, settings, t1Ind, t2Ind, t3Ind, speedInd, arr, &aligner](Engine& e) {
		TextBox& t1Ref = static_cast<TextBox&>(settings->getObject(t1Ind));
		TextBox& t2Ref = static_cast<TextBox&>(settings->getObject(t2Ind));
		TextBox& t3Ref = static_cast<TextBox&>(settings->getObject(t3Ind));
		TextBox& speedRef = static_cast<TextBox&>(settings->getObject(speedInd));
		aligner.resetStrings();
		if (t1Ref.getText() != "") {
			try {
				aligner.addString(t1Ref.getText());
			}
			catch (invalid_argument excep) {
				e.showAlert("Error", excep.what());
				return;
			}
		}
		if (t2Ref.getText() != "") {
			try {
				aligner.addString(t2Ref.getText());
			}
			catch (invalid_argument excep) {
				e.showAlert("Error", excep.what());
				return;
			}
		}
		if (t3Ref.getText() != "") {
			try {
				aligner.addString(t3Ref.getText());
			}
			catch (invalid_argument excep) {
				e.showAlert("Error", excep.what());
				return;
			}
		}
		if (!isInteger(speedRef.getText())) {
			e.showAlert("Error", "Speed should be an integer!");
			return;
		}
		int speed = stoi(speedRef.getText());
		if (aligner.getStringsNum() > 1) {
			Visualizer* vis = aligner.createVisualizer(e, speed);
			switch (arr->getSelected()) {
			case GLOBAL_ALIGNMENT:
				vis->globalAlignmentInit(); break;
			case LOCAL_ALIGNMENT:
				vis->localAlignmentInit(); break;
			case FREE_ENDS:
				vis->freeEndsInit(); break;
			default:
				break;
			}
			settings->setHeldObject(vis);
		}
		e.changeScene(menuInd);
	});
	return settings;
}

int setupMenu(Engine& e,Aligner& aligner) {
	Menu* menu = new Menu(e.getDisplay(), e);
	int menuInd = e.addScene(menu);
	Menu& menuRef = static_cast<Menu&>(e.getScene(menuInd));
	SettingsScene<Visualizer*>* settings = setupSettings(e, aligner, menuInd);
	int settingsInd = e.addScene(settings);
	auto start = [&aligner,settings, menuInd](Engine & e) {
		if (aligner.getStringsNum() <2) {
			e.showAlert("Error", "You should add at least two strings before running the visualizer.");
			return;
		}
		Visualizer* vis = settings->getHeldObject();
		if (vis == nullptr) {
			e.showAlert("Error", "Unexpected error");
			return;
		}
		vis->setPrevScene(menuInd);
		vis->addScene();
		e.addAdditionalFunction([vis]()->bool {
			vis->step();
			return !(vis->isFinished());
		});
		int visInd = vis->getSceneInd();
		e.changeScene(visInd);
	};
	menuRef.addButton(glm::vec3(0, 0.5, 1), 1, 5, 5, 1, "Start", start);
	menuRef.addButton(glm::vec3(0, 0.5, 1), 1, 2, 5, 1, "Settings", [settingsInd](Engine& e) {e.changeScene(settingsInd); });
	menuRef.addButton(glm::vec3(1, 0, 0), 1, -2, 3, 1, "Exit", [](Engine& e) {e.close(); });
	return menuInd;
}

bool isInteger(string str) {
	for (unsigned int i = 0; i < str.size(); ++i) {
		char curr = str[i];
		if (curr<'0' || curr>'9') { return false; }
	}
	return true;
}

