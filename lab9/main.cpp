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
	for (int loop = 0; loop < 10;  loop++)
	{
		cEnemy* cEnemyItem = new cEnemy();
		cEnemyItem->MarchNo(loop);
		cEnemyItem->setMdlDimensions(tardisMdl.getModelDimensions());
		enemyList.push_back(cEnemyItem);
	}

	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, -100), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(tardisMdl2.getModelDimensions());

	float shotDelay= 0.5f;
	float lastShot = 0.0f;
	float time = 0.0f;
	int score = 0;

	std::vector<cLaser*> PlayerLaserList;
	std::vector<cLaser*>::iterator index;
	
	std::vector<cLaser*>enemyLaserList;
	std::vector<cLaser*>::iterator enemyShotIndex;
	//cLaser laserList[30];
	//int numShots = 0;

	// Load font
	struct dtx_font *fntmain;

	fntmain = dtx_open_font("Fonts/doctor_who.ttf", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);             /* ASCII */
	
	dtx_use_font(fntmain, FONT_SZ);
	
	// Load Sound
	cSound themeMusic;
	themeMusic.createContext();
	themeMusic.loadWAVFile("Audio/NationalShiteDay.wav");
	themeMusic.playAudio(AL_LOOPING);

	// explosion
	cSound explosionFX;
	explosionFX.loadWAVFile("Audio/Blast.wav");

	//firing sound
	cSound firingFX;
	firingFX.loadWAVFile("Audio/pew.wav");

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		time += elapsedTime;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(0.8, 0.9, 1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (restart == true)
		{
			index = PlayerLaserList.begin();
			while (index != PlayerLaserList.end())
			{
				(index) = PlayerLaserList.erase(index);
			}
			for (EnemyIndex = enemyList.begin(); EnemyIndex != enemyList.end(); ++EnemyIndex)
			{
				(*EnemyIndex)->setIsActive(false);
			}
			enemyShotIndex = enemyLaserList.begin();
			while (enemyShotIndex != enemyLaserList.end())
			{
				(enemyShotIndex) = enemyLaserList.erase(enemyShotIndex);
			}
			for (int loop = 0; loop < 10; loop++)
			{
				cEnemy* cEnemyItem = new cEnemy();
				cEnemyItem->MarchNo(loop);
				cEnemyItem->setMdlDimensions(tardisMdl.getModelDimensions());
				enemyList.push_back(cEnemyItem);
			}
			alive = true;
			restart = false;
			thePlayer.health = 250;
			thePlayer.setIsActive(true);
		}

		if (camera)
		{
			gluLookAt(0, 300, 10, 0, 0, 0, 0, 1, 0);
		}
		else
		{
			glm::vec3 facingDir;
			facingDir.x = -(float)glm::sin(glm::radians(thePlayer.getRotation()));
			facingDir.y = 0.0f;
			facingDir.z = (float)glm::cos(glm::radians(thePlayer.getRotation()));
			facingDir *= -1;

			glm::vec3 pos;
			pos = thePlayer.getPosition();

			gluLookAt(pos.x, 3, -pos.z-2, pos.x + facingDir.x, 3, (pos.z + facingDir.z), 0, 1, 0);
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
				if (time > (*EnemyIndex)->getShotTime() + (rand() % 30)+3)
				{
					(*EnemyIndex)->setShotTime(time);
					cLaser* laser = new cLaser();
					glm::vec3 mdlLaserDirection;
					mdlLaserDirection.x = 0.0f;
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = -1.0f;

					laser->setDirection(mdlLaserDirection);   ///     glm::vec3(0, 0, 5)
					laser->setRotation(0.0f);
					laser->setScale(glm::vec3(3, 3, 3));
					laser->setSpeed(25);
					laser->setPosition((*EnemyIndex)->getPosition() + mdlLaserDirection);    //   glm::vec3(0, 0, 0)
					laser->setIsActive(true);
					laser->setMdlDimensions(theLaser.getModelDimensions());
					laser->update(elapsedTime);
					enemyLaserList.push_back(laser);
					if (playing == true){ firingFX.playAudio(AL_TRUE); }
				}
			}
		}
		translationX = 0;
		tardisMdl2.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		thePlayer.update(elapsedTime);
		////are we shooting?
		if (playing == false){ themeMusic.playAudio(AL_FALSE); }

		if (fire&&(time-lastShot)>shotDelay&&alive==true)
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
			PlayerLaserList.push_back(laser);
			//fire = false;
			if (playing == true){ firingFX.playAudio(AL_TRUE); }
			//numShots++;
			lastShot = time;
		}

		

		for (enemyShotIndex = enemyLaserList.begin(); enemyShotIndex != enemyLaserList.end(); ++enemyShotIndex)
		{
			//glScalef(3, 3, 3);
			theLaser.renderMdl((*enemyShotIndex)->getPosition(), (*enemyShotIndex)->getRotation(), (*enemyShotIndex)->getScale());
			(*enemyShotIndex)->update(elapsedTime);
			// check for collisions
			if ((*enemyShotIndex)->SphereSphereCollision(thePlayer.getPosition(), thePlayer.getMdlRadius()))
				{
					thePlayer.setHealth(50);
					if (thePlayer.getHealth() < 1)
					{
						thePlayer.setIsActive(false);
					}
					(*enemyShotIndex)->setIsActive(false);
					if (playing == true){ explosionFX.playAudio(AL_TRUE); }
					break; // No need to check for other bullets.
				}
			}
		for (index = PlayerLaserList.begin(); index != PlayerLaserList.end(); ++index)
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
						(*EnemyIndex)->setHealth(50);
						if ((*EnemyIndex)->getHealth() < 1)
						{
							(*EnemyIndex)->setIsActive(false);
							score++;
						}
						(*index)->setIsActive(false);
						explosionFX.playAudio(AL_TRUE);
						break; // No need to check for other bullets.
					}
				}
			}
		}
		if (thePlayer.isActive() == false)
		{
			thePlayer.~cPlayer();
		}
		index = PlayerLaserList.begin();
		while (index != PlayerLaserList.end())
		{
			if ((*index)->isActive() == false)
			{
				(index) = PlayerLaserList.erase(index);
			}
			else
			{
				index++;
			}
		}
		enemyShotIndex = enemyLaserList.begin();
		while (enemyShotIndex != enemyLaserList.end())
		{
			if ((*enemyShotIndex)->isActive() == false)
			{
				(enemyShotIndex) = enemyLaserList.erase(enemyShotIndex);
			}
			else
			{
				enemyShotIndex++;
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

		string temp = "                               Health- ";
		for (int i = 0; i < thePlayer.getHealth(); i += 50)
		{
			if (i==0)
			temp += "|///|";
			else temp += "///|";
		}
		if (alive == true&& enemyList.begin()!=enemyList.end()){ temp += "\nSpace to fire arrow keys to move"; }
		else if (alive == false && enemyList.begin() != enemyList.end()){ temp += "\nWell done you incompetent fool/ndo you want to restart? Y/N"; }
		else{ temp += "\nYou killed them all, restart?Y/N"; alive = false; }
		string orig("Doctor Who: " + std::to_string(score) + temp);
		cout << orig << "Doctor Who: " << endl;

		// Convert to a char*
		const size_t newsize = 300;
		char nstring[newsize];
		strcpy_s(nstring, orig.c_str());
		strcat_s(nstring, ""+score);
		cout << nstring << endl;

		dtx_string(nstring);

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
