#include<app.h>
#include<inputs.h>
#include<extern_definitions.h>
#include<physics.h>
#include<set>
#include<fstream>

std::string App::projectDataFileName{ "AppObj" };

printHelperNS::PrintHelper App::ph{ "App" };



void App::switchPhysics() {}



void App::endGame()
{
	ph.mapNewString("ENDGAME", "GAME OVER");
	ph.mapNewString("ENDGAME",
		"PRESS Q TO QUIT OR P TO PLAY AGAIN");
	

}


void App::render(double currentTime)
{

  GLfloat black[] = {0.2,0.2,0.2,1.0};
  GLfloat one = 1.0f;
  glClearBufferfv(GL_COLOR,0,black);
  glClearBufferfv(GL_DEPTH,0,&one);

  glViewport(0,0,Application::window_width,Application::window_height);

  

  for (int i = 0; i < objectManager.ApplicationCollectorList.size(); i++)
	  objectManager.ApplicationCollectorList[i]->OCrender();

  /*Il testo va renderizzato necessariamente 
  dopo tutto il resto altrimenti vedi i frame dei caratteri*/
  textRenderer.render(fpcameraNS::CameraManager::getActiveCamera());
  
}


void App::update(){
	
	fpcameraNS::CameraManager::update();
	userInterface.update();

	if (!UserInterface::paused)
	{
		TimingData::update();
		globalNS::DURATION = 0.1;// TimingData::get().lastFrameDuration * 0.005f;
		if (globalNS::DURATION <= 0.0f) return;

		

		for (int i = 0; i < objectManager.ApplicationCollectorList.size(); i++)
			objectManager.ApplicationCollectorList[i]->OCupdate(globalNS::DURATION);

		myphysicsNS::CollisorePolimorfo::update(globalNS::DURATION);

		soundNS::soundMap::update();
		soundNS::soundMap::collisionSound(polyCollisor.contacts, polyCollisor.frameContactCounter);

		aiNS::AI::updateHealt(polyCollisor.contacts, polyCollisor.frameContactCounter);
	}
}


void App::generateContacts(){	

}





void App::startup()
{
	TimingData::init();
	fpcameraNS::CameraManager::load(App::projectDataFileName);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);



	soundEngine.startSounds();
	//controls.setup(this);
	userInterface.init();
	textRenderer.create();

}


void App::SaveProjectData(string filename) {

	std::string filepath{ logNS::Logger::STOREDDATADIR + filename };
	std::ofstream out(filepath);
	out.clear();
	out.close();	

	myobjectNS::ApplicationObjectManager::save(filename);
	fpcameraNS::CameraManager::save();
	

	
}

bool App::loadProjectData(string filename) {
	fpcameraNS::CameraManager::load(App::projectDataFileName);
	return myobjectNS::ApplicationObjectManager::loadData(filename);
}


// resetta l'intera applicazione in modo da poter caricare un nuovo file
void App::reset()
{
	
	//fpcameraNS::CameraManager::resetAll();
}



//resetta gli oggetti
void App::resetAll() {

	for (int i = 0; i < objectManager.ApplicationCollectorList.size(); i++)
		objectManager.ApplicationCollectorList[i]->OCreset();
	
	
}