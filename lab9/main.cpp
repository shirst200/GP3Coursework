#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include "GameConstants.h"
#include <windows.h>
#include "windowOGL.h"
#include "cWNDManager.h"
#include "libdrawtext-0.2.1\drawtext.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cEnemy.h"
#include "cPlayer.h"
#include "cLaser.h"
#include "cSound.h"
//#include "cCamera.h"
#include <vector>
#include <time.h>

#define FONT_SZ	24

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

    //The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL()) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	srand(time(NULL));

	cModelLoader tardisMdl;
	tardisMdl.loadModel("Models/tardis.obj");
	

	cModelLoader tardisMdl2;
	tardisMdl2.loadModel("Models/tardis.obj"); // Player

	cModelLoader theLaser;
	theLaser.loadModel("Models/laser.obj");

	std::vector<cEnemy*> enemyList;
	std::vector<cEnemy*>::iterator EnemyIndex;
	for (int loop = 0; loop < 10; loop++)
	{
		cEnemy* cEnemyItem = new cEnemy();
		cEnemyItem->MarchNo(loop);
		cEnemyItem->setMdlDimensions(tardisMdl.getModelDimensions());
		enemyList.push_back(cEnemyItem);
	}

	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, -100), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(tardisMdl2.getModelDimensions());

	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;
	
	//cLaser laserList[30];
	//int numShots = 0;

	// Load font
	struct dtx_font *fntmain;

	fntmain = dtx_open_font("Fonts/doctor_who.ttf", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);             /* ASCII */
	
	dtx_use_font(fntmain, FONT_SZ);
	
	// Load Sound
	//cSound themeMusic;
	//themeMusic.createContext();
	//themeMusic.loadWAVFile("Audio/who10Edit.wav");
	//themeMusic.playAudio(AL_LOOPING);

	// explosion
	//cSound explosionFX;
	//explosionFX.createContext();
	//explosionFX.loadWAVFile("Audio/explosion2.wav");

	//firing sound
	cSound firingFX;
	firingFX.createContext();
	firingFX.loadWAVFile("Audio/shot007.wav");

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(0.8, 0.9, 1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (camera)
		{
			gluLookAt(0, 300, 10, 0, 0, 0, 0, 1, 0);
		}
		else
		{
			gluLookAt(0, 0, 150, 0, 0, 0, 0, 1, 0);
		}




		//Do any pre-rendering logic
		//Render the scene
		//Drawing the model
		bool ac=true;
		int vel=0;
		for (EnemyIndex = enemyList.begin(); EnemyIndex != enemyList.end(); ++EnemyIndex)
		{
			if ((*EnemyIndex)->isActive() == true)
			{
				if (ac==true)
				{
					vel = (*EnemyIndex)->GetSpeed();
					ac = false;
				}
				(*EnemyIndex)->SetSpeed(vel);
				tardisMdl.renderMdl((*EnemyIndex)->getPosition(), (*EnemyIndex)->getRotation(), (*EnemyIndex)->getScale());
				(*EnemyIndex)->update(elapsedTime);
			}
		}

		tardisMdl2.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		thePlayer.update(elapsedTime);
		////are we shooting?
		if (fire)
		{
			cLaser* laser = new cLaser();
			glm::vec3 mdlLaserDirection;
			mdlLaserDirection.x = -(float)glm::sin(glm::radians(thePlayer.getRotation()));
			mdlLaserDirection.y = 0.0f;
			mdlLaserDirection.z = (float)glm::cos(glm::radians(thePlayer.getRotation()));

			laser->setDirection(mdlLaserDirection);   ///     glm::vec3(0, 0, 5)
			laser->setRotation(0.0f);
			laser->setScale(glm::vec3(3, 3, 3));
			laser->setSpeed(25);
			laser->setPosition(thePlayer.getPosition() + mdlLaserDirection);    //   glm::vec3(0, 0, 0)
			laser->setIsActive(true);
			laser->setMdlDimensions(theLaser.getModelDimensions());
			laser->update(elapsedTime);
          	laserList.push_back(laser);
			//fire = false;
			firingFX.playAudio(AL_FALSE);
			//numShots++;
		}

		//for (int loop = 0; loop < numShots; loop++)
		//{
		//	theLaser.renderMdl(laserList[loop].getPosition(), laserList[loop].getRotation());
		//	laserList[loop].update(elapsedTime);
		//}


		for (index = laserList.begin(); index != laserList.end(); ++index)
		{
			if ((*index)->isActive())
			{
				//glScalef(3, 3, 3);
				theLaser.renderMdl((*index)->getPosition(), (*index)->getRotation(), (*index)->getScale() );
				(*index)->update(elapsedTime);
				// check for collisions
				for (EnemyIndex = enemyList.begin(); EnemyIndex != enemyList.end(); ++EnemyIndex)
				{
					//enemyList[loop].update(elapsedTime);
					if ((*index)->SphereSphereCollision((*EnemyIndex)->getPosition(), (*EnemyIndex)->getMdlRadius()))
					{
						(*EnemyIndex)->setIsActive(false);
						(*index)->setIsActive(false);
     						//explosionFX.playAudio(AL_FALSE);
						break; // No need to check for other bullets.
					}
				}
			}
		}
		index = laserList.begin();
		while (index != laserList.end())
		{
			if ((*index)->isActive() == false)
			{
				(index) = laserList.erase(index);
			}
			else
			{
				index++;
			}
		}
		EnemyIndex = enemyList.begin();
		while (EnemyIndex != enemyList.end())
		{
			if ((*EnemyIndex)->isActive() == false)
			{
				(EnemyIndex) = enemyList.erase(EnemyIndex);
			}
			else
			{
				EnemyIndex++;
			}
		}
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, windowWidth, 0, windowHeight);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glPushAttrib(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);

		glTranslatef(0, 730, 0);
		dtx_string("Doctor Who: Tardis Test Drive");

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		pgmWNDMgr->swapBuffers();
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
