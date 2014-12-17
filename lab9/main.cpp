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
#include "CXBOXController.h"
#include <vector>
#include <time.h>



#define FONT_SZ	24

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
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
	//Loads the model for the enemy
	cModelLoader tardisMdl;
	tardisMdl.loadModel("Models/gun.obj");
	//Loads the model for the player
	cModelLoader tardisMdl2;
	tardisMdl2.loadModel("Models/luke.obj");
	//Loads the model for the shot
	cModelLoader theLaser;
	theLaser.loadModel("Models/pokeball.obj");
	cModelLoader otherLaser;
	otherLaser.loadModel("Models/Sword2.obj");


	//Creates the list that will store the attacks sent out by the player until they are deleted and the index to access them
	std::vector<cLaser*> PlayerLaserList;
	std::vector<cLaser*>::iterator index;

	//Creates the list that will store the attacks sent out by the enemies until they are deleted and the index to access them
	std::vector<cLaser*>enemyLaserList;
	std::vector<cLaser*>::iterator enemyShotIndex;

	//Creates the enemys and instansiates them in lines of 5
	std::vector<cEnemy*> enemyList;
	std::vector<cEnemy*>::iterator EnemyIndex;
	for (int loop = 0; loop < 10;  loop++)
	{
		cEnemy* cEnemyItem = new cEnemy();
		cEnemyItem->MarchNo(loop);
		cEnemyItem->setMdlDimensions(tardisMdl.getModelDimensions());
		enemyList.push_back(cEnemyItem);
		cEnemyItem->setRotation(180);
		cEnemyItem->setScale(glm::vec3(3, 3, 3));
	}
	//Creates the player and instansiates it at the bottom  of the top dawn camera's view 
	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, -100), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(tardisMdl2.getModelDimensions());
	thePlayer.setScale(glm::vec3(3, 3, 3));

	// Load font
	struct dtx_font *fntmain;

	//Loads the font for use when writing to the screen
	fntmain = dtx_open_font("Fonts/doctor_who.ttf", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);             /* ASCII */
	dtx_use_font(fntmain, FONT_SZ);
	
	// Loads and plays sound for background
	cSound themeMusic;
	themeMusic.createContext();
	themeMusic.loadWAVFile("Audio/NationalShiteDay.wav");
	themeMusic.playAudio(AL_LOOPING);

	// Load sound for sucessful hit
	cSound explosionFX;
	explosionFX.loadWAVFile("Audio/Blast.wav");

	//Load sound for firing
	cSound firingFX;
	firingFX.loadWAVFile("Audio/pew.wav");

	//variables for the player
	//shotDelay is the time that the player has to wait before firing again
	//lastShot is the time the last shot was taken
	//time is roughly the current time that the program has been running
	//score is the count of how many enemies the player has killed
	float shotDelay = 0.5f;
	float lastShot = 0.0f;
	float time = 0.0f;
	int score = 0;

	CXBOXController* Player1;
	Player1 = new CXBOXController(1);
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

		//If the game has finished either by the player winning or losing and wishes to play again
		//This resets the player, enemies and shots
		if (restart == true)
		{
			//going down the list of shots from the player 
			index = PlayerLaserList.begin();
			while (index != PlayerLaserList.end())
			{
				//destroys the current shot
				(index) = PlayerLaserList.erase(index);
			}
			//going down the list of enemies 
			EnemyIndex = enemyList.begin();
			while (EnemyIndex != enemyList.end())
			{
				//destroys the current enemy
				(EnemyIndex) = enemyList.erase(EnemyIndex);
			}
			//going down the list of shots from the enemies 
			enemyShotIndex = enemyLaserList.begin();
			while (enemyShotIndex != enemyLaserList.end())
			{
				//destroys the current shot
				(enemyShotIndex) = enemyLaserList.erase(enemyShotIndex);
			}
			//Creates the 10 enemies again
			for (int loop = 0; loop < 10; loop++)
			{
				cEnemy* cEnemyItem = new cEnemy();
				cEnemyItem->MarchNo(loop);
				cEnemyItem->setMdlDimensions(tardisMdl.getModelDimensions());
				enemyList.push_back(cEnemyItem);
				cEnemyItem->setScale(glm::vec3(3, 3, 3));
			}
			//Resets the player to pregame status(not including position and rotation)
			score = 0;
			alive = true;
			restart = false;
			thePlayer.health = 250;
			thePlayer.setIsActive(true);
		}
		//Creates the camera and shows where it is to look(the bool camera can be used to toggle between views)
		if (camera)
		{
			//A top down camera view
			gluLookAt(0, 300, 10, 0, 0, 0, 0, 1, 0);
		}
		else
		{
			//A camera view that follows the player
			glm::vec3 lookDirectoin;
			lookDirectoin.x = (float)glm::sin(glm::radians(thePlayer.getRotation()));
			lookDirectoin.y = 0.0f;
			lookDirectoin.z = -(float)glm::cos(glm::radians(thePlayer.getRotation()));

			gluLookAt(thePlayer.getPosition().x, 3, -thePlayer.getPosition().z - 2, thePlayer.getPosition().x + lookDirectoin.x, 3, (thePlayer.getPosition().z + lookDirectoin.z), 0, 1, 0);
		}
		//Do any pre-rendering logic
		//Render the scene
		//Drawing the model

		//ac and vel are used to make all the enemies follow the first enemy
		bool ac=true;
		int vel=0;
		//goes through every enemy
		for (EnemyIndex = enemyList.begin(); EnemyIndex != enemyList.end(); ++EnemyIndex)
		{
			if ((*EnemyIndex)->isActive() == true)
			{
				if (ac==true)
				{
					//this gets the velocity of the first enemy
					vel = (*EnemyIndex)->GetSpeed();
					ac = false;
				}
				// this sets the velocity of the current enemy to the velocity of the first enemy
				(*EnemyIndex)->SetSpeed(vel);
				tardisMdl.renderMdl((*EnemyIndex)->getPosition(), (*EnemyIndex)->getRotation(), (*EnemyIndex)->getScale());
				(*EnemyIndex)->update(elapsedTime);

				//This section will fire a shot from the current enemy if it was a certain length of time since the last shot with some deviation
				if (time > (*EnemyIndex)->getShotTime() + (rand() % 30)+3)
				{
					//This creates the pellet, places it at the enemy that fired it and sets it going down the screen
					(*EnemyIndex)->setShotTime(time);
					cLaser* laser = new cLaser();
					glm::vec3 mdlLaserDirection;
					mdlLaserDirection.x = 0.0f;
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = -1.0f;

					laser->setDirection(mdlLaserDirection);   ///     glm::vec3(0, 0, 5)
					laser->setRotation(0.0f);
					laser->setScale(glm::vec3(9, 9, 9));
					laser->setSpeed(75);
					laser->setPosition((*EnemyIndex)->getPosition() + mdlLaserDirection);    //   glm::vec3(0, 0, 0)
					laser->setIsActive(true);
					laser->setMdlDimensions(otherLaser.getModelDimensions());
					laser->update(elapsedTime);
					enemyLaserList.push_back(laser);
					//If the mute toggle is not on the firing sound will play
					if (playing == true){ firingFX.playAudio(AL_TRUE); }
				}
			}
		}
		//This renders the player with the new positions
		tardisMdl2.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
		thePlayer.update(elapsedTime);
		//If the game is not muted the background music will play else it will not
		if (playing == false){ themeMusic.playAudio(AL_FALSE); }
		//If the player is holding the shoot button and it has been 0.5 seconds since the last pellet was fired the player shoots a pellet
		if ((fire || Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) && (time - lastShot)>shotDelay&&alive == true)
		{
			//This creates the pellet, places it at the player and sets it going up the screen
			cLaser* laser = new cLaser();
			glm::vec3 mdlLaserDirection;
			mdlLaserDirection.x = -(float)glm::sin(glm::radians(thePlayer.getRotation()));
			mdlLaserDirection.y = 0.0f;
			mdlLaserDirection.z = (float)glm::cos(glm::radians(thePlayer.getRotation()));

			laser->setDirection(mdlLaserDirection);
			laser->setRotation(0.0f);
			laser->setScale(glm::vec3(9, 9, 9));
			laser->setSpeed(75);
			laser->setPosition(thePlayer.getPosition() + mdlLaserDirection);
			laser->setIsActive(true);
			laser->setMdlDimensions(theLaser.getModelDimensions());
			laser->update(elapsedTime);
			PlayerLaserList.push_back(laser);
			//If the mute toggle is not on the firing sound will play
			if (playing == true){ firingFX.playAudio(AL_TRUE); }
			//The time since the last pellet was fored is et to the current time
			lastShot = time;
		}
		//This section of code checks each of the enemies shots to see if it has hit the player
		for (enemyShotIndex = enemyLaserList.begin(); enemyShotIndex != enemyLaserList.end(); ++enemyShotIndex)
		{
			//The pellet is rendered and it's position updated
			otherLaser.renderMdl((*enemyShotIndex)->getPosition(), (*enemyShotIndex)->getRotation(), (*enemyShotIndex)->getScale());
			(*enemyShotIndex)->update(elapsedTime);
			// Check for collision with an enemy
			if ((*enemyShotIndex)->SphereSphereCollision(thePlayer.getPosition(), thePlayer.getMdlRadius()))
			{
				//The pellet has hit and is removing 50 health from the player
				thePlayer.setHealth(50);
				if (thePlayer.getHealth() < 1)
				{
					//The players health is 0 or lower the game is over
					thePlayer.setIsActive(false);
				}
				//The pellet is marked for deletion
				(*enemyShotIndex)->setIsActive(false);
				//The sucessful hit sound is played if the game is unmuted
				if (playing == true){ explosionFX.playAudio(AL_TRUE); }
				break; // No need to check for other bullets.
			}
		}
		//This section of code checks each of the players shots to see if it has hit an enemy		
		for (index = PlayerLaserList.begin(); index != PlayerLaserList.end(); ++index)
		{
			if ((*index)->isActive())
			{
				//The pellet is rendered and it's position updated
				theLaser.renderMdl((*index)->getPosition(), (*index)->getRotation()+180, (*index)->getScale() );
				(*index)->update(elapsedTime);
				for (EnemyIndex = enemyList.begin(); EnemyIndex != enemyList.end(); ++EnemyIndex)
				{
					// check for collision with an enemy
					if ((*index)->SphereSphereCollision((*EnemyIndex)->getPosition(), (*EnemyIndex)->getMdlRadius()))
					{
						//The pellet has hit and is removing 50 health from the enemy it hit
						(*EnemyIndex)->setHealth(50);
						if ((*EnemyIndex)->getHealth() < 1)
						{
							//The enemies health and is marked for deletion
							(*EnemyIndex)->setIsActive(false);
							score++;
						}
						//The pellet is marked for deletion
						(*index)->setIsActive(false);
						//If unmuted the explosion sound is played
						if (playing == true){ explosionFX.playAudio(AL_TRUE); }
						break; // No need to check for other bullets.
					}
				}
			}
		}
		//is the player has 0 health or less the deconstructer for the player is called prompting the player to use the restart function
		if (thePlayer.isActive() == false)
		{
			thePlayer.~cPlayer();
		}
		//Each of the players shots that have been marked for deletion are deleted
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
		//Each of the enemies shots that have been marked for deletion are deleted
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
		//Each of the enemies that have been marked for deletion are deleted
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

		//The text is set for display
		string temp = "                               Health- ";
		for (int i = 0; i < thePlayer.getHealth(); i += 50)
		{
			//for each 50 health the player has a bar is added to the string
			if (i==0)
			temp += "|///|";
			else temp += "///|";
		}
		//Different text is displayed depending on the situation
		if (alive == true&& enemyList.begin()!=enemyList.end()){ temp += "\nSpace to fire arrow keys to move"; }
		else if (alive == false && enemyList.begin() != enemyList.end()){ temp += "\nWell done you incompetent fool\ndo you want to restart? Y/N"; }
		else{ temp += "\nYou killed them all, restart?Y/N"; alive = false; }
		string orig("Score: " + std::to_string(score) + temp);

		// Convert to a char*
		const size_t newsize = 300;
		char nstring[newsize];
		strcpy_s(nstring, orig.c_str());
		strcat_s(nstring, ""+score);

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
