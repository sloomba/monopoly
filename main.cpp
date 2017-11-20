#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <IL/il.h>
#include "glm.h"
#include "drawtext.h"
/** \mainpage
A game by Sahil Loomba - 2012CS10114
\section intro_sec Introduction
Monopoly is a property-dealing family board game, for 2 to 8 players. The objective of the game is to become the wealthiest player in the game through strategised selling and buying of properties. This video game emulates the original board game, along with provisions for multiplayer, cheating, and chatting between multiplayers over the internet.
This is the documentation page of this game.
\section view_doc Viewing the Documentation
You can navigate to the Files tab to view the main file, as well as the parser files.
*/

using namespace std;
namespace {
	GLuint loadImage(const char* filename) {
		GLuint texId;
		ilInit();
		ILuint imageId;
		ILboolean success;
		ilGenImages(1, &imageId);
		ilBindImage(imageId);
		success = ilLoadImage((const ILstring)filename);
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		ilDeleteImages(1, &imageId); 
		return texId;
	}
}

const unsigned int TIME_STEP = 25; /**< This is a constant unisigned integer, which represents the update time (or the refresh rate) in milliseconds. */
bool start_screen = true; /**< Boolean variable to indicate start screen, with Monopoly Logo, is on. */
bool main_menu = false; /**< Boolean variable to indicate main menu, with options to start a new game, load an old game and quit are located, is on. */
bool game_begins = false; /**< Boolean variable to indicate that the game is on. */
GLuint logo, monopoly_text, blackwood, parchment, button_icon, pay_icon, done_icon, undo_icon, mortgage_icon, view_icon, report_icon, d1, d5, d10, d20, d50, d100, d500;
float angle = 0.0, mono_angle = 0.0, scale = 1.6, go_up = 0.0, fade_in = 0.0, menubar = 6.0, sidebar = 38.0, butscale = 0.0;
bool clkwise = true;
bool zoomout = true;
bool newgame = true, loadgame = false, quitgame = false;
bool interrupt = false, sideinterrupt = false, rules = false, status = false, properties = false, bid = false, trade = false, quit = false, my_properties = false, all_players = false, pay = false, done = false, undo = false, mortgage = false, view = false, report = false;
struct dtx_font* main_font = dtx_open_font("fonts/Quicksand_Bold.otf", 200);
struct dtx_font* dash_font = dtx_open_font("fonts/Quicksand_Dash.otf", 200);
struct dtx_font* mono_font = dtx_open_font("fonts/kabel.ttf", 200);
GLMmodel *earth; /**< Model variable for loading earth model. */
GLMmodel *road; /**< Model variable for loading road model. */
GLMmodel *dice1;
GLMmodel *dice2;
bool dice_visible = true, dice_shuffle = false;
float dice1_ang1 = 0.0, dice1_ang2 = 0.0, dice2_ang1 = 0.0, dice2_ang2 = 0.0, dice1_ang11 = 0.0, dice1_ang21 = 0.0, dice2_ang11 = 0.0, dice2_ang21 = 0.0;
int dice_trans = 0, dice_value = 8;
GLMmodel *house; /**< Model variable for loading house model. */
GLMmodel *hotel; /**< Model variable for loading hotel model. */
float slow_rot = 0.0;
//player attributes

int account_value = 5000;
int to_submit = 0;
/** This function is for handling mouse clicks, depending on which screen is currently on, and the pixel location of the mouse. Since the game is mainly mouse-driven, it is a very important function.*/
void handleMouseclick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN){
		cout<<"("<<x<<", "<<y<<")"<<endl;
		if (start_screen == true) {
			start_screen = false;
			main_menu = true;
		}
		if (main_menu == true and zoomout == false) {
			if (x >= 580 and x <= 730 and y >= 560 and y <= 630){
				main_menu = false;
				exit(0);
			}
			else if (x >= 420 and x <= 930 and y >= 260 and y <= 340){
				main_menu = false;
				game_begins = true;
			}
		}
		if (game_begins == true) {
			if (interrupt == true) {
				interrupt = false;
				rules = false;
				status = false;
				properties = false;
				bid = false;
				trade = false;
			}
			if (x >= 0 and x <= 117 and y >= 0 and y <= 40){
				rules = true;
				interrupt = true;
			}
			if (x >= 164 and x <= 285 and y >= 0 and y <= 40){
				status = true;
				interrupt = true;
			}
			if (x >= 340 and x <= 523 and y >= 0 and y <= 40){
				properties = true;
				interrupt = true;
			}
			if (x >= 572 and x <= 638 and y >= 0 and y <= 40){
				bid = true;
				interrupt = true;
			}
			if (x >= 683 and x <= 781 and y >= 0 and y <= 40){
				trade = true;
				interrupt = true;
			}
			if (x >= 822 and x <= 901 and y >= 0 and y <= 40){
				quit = true;
				exit(0);
			}
			if (interrupt == false) {
				if (x >= 890 and x <= 980 and y >= 85 and y <= 175){
					pay = true;
				}
				if (x >= 890 and x <= 980 and y >= 190 and y <= 280){
					done = true;
				}
				if (x >= 890 and x <= 980 and y >= 295 and y <= 385){
					undo = true; 
				}
				if (x >= 890 and x <= 980 and y >= 400 and y <= 490){
					mortgage = true;
				}
				if (x >= 890 and x <= 980 and y >= 505 and y <= 595){
					view = true; 
				}
				if (x >= 890 and x <= 980 and y >= 610 and y <= 700){
					report = true;
				}
				if (dice_visible == true and x >= 15 and x <= 200 and y >= 650 and y <= 730){
					dice_shuffle = true;
					dice_visible = false;
					dice_trans = 40;
				}
				if (sideinterrupt == false) {
					if (x >= 1020 and x <= 1365 and y >= 0 and y <= 55){
						my_properties = true;
						sideinterrupt = true;
					}
					if (x >= 1020 and x <= 1365 and y > 55 and y <= 110 and my_properties == false){
						all_players = true;
						sideinterrupt = true;
					}
					if (x >= 1050 and x <= 1160 and y >= 253 and y <= 385){
						if (account_value >= 1) {
							account_value = account_value - 1;
							to_submit = to_submit + 1;
						}
					}
					if (x > 1160 and x <= 1365 and y >= 253 and y <= 385){
						if (account_value >= 20) {
							account_value = account_value - 20;
							to_submit = to_submit + 20;
						}
					}
					if (x >= 1050 and x <= 1160 and y >= 403 and y <= 535){
						if (account_value >= 5) {
							account_value = account_value - 5;
							to_submit = to_submit + 5;
						}
					}
					if (x > 1160 and x <= 1365 and y >= 403 and y <= 535){
						if (account_value >= 50) {
							account_value = account_value - 50;
							to_submit = to_submit + 50;
						}
					}
					if (x >= 1050 and x <= 1160 and y >= 553 and y <= 685){
						if (account_value >= 10) {
							account_value = account_value - 10;
							to_submit = to_submit + 10;
						}
					}
					if (x > 1160 and x <= 1365 and y >= 553 and y <= 685){
						if (account_value >= 100) {
							account_value = account_value - 100;
							to_submit = to_submit + 100;
						}
					}
				}
				else {
					if (x >= 1020 and x <= 1365 and y >= 0 and y <= 55){
						if (my_properties == true) {
							my_properties = false;
							sideinterrupt = false;
						}
						else {
							my_properties = true;
							sideinterrupt = true;
						}
					}
					if (all_players == true and x >= 1020 and x <= 1365 and y > 55 and y <= 110){
						all_players = false;
						sideinterrupt = false;
					}
				}
			}
		}
	}
}
/** This function is for handling key presses from the keyboard, especially useful when navigating the main menu, or to skip through screens or exit the game. */
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case '\r': //Enter key
			if (start_screen == true) {
				start_screen = false;
				main_menu = true;
			}
			else if (main_menu == true) {
				main_menu = false;
				if (quitgame == true) {exit(0);}
				else if (newgame == true) {game_begins = true;}
			}
			break;
		case 32: //Space bar 
			break;
		case 27: //Escape key
			exit(0);
	}
}
/** This function is for handling special key presses, like the up and down keys, useful for menu traverals.*/
void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			if (main_menu == true) {
				if (newgame == true) {
					newgame = false;
					quitgame = true;
				}
				else if (loadgame == true) {
					loadgame = false;
					newgame = true;
				}
				else {
					quitgame = false;
					loadgame = true;
				}
			}
			break;
		case GLUT_KEY_DOWN:
			if (main_menu == true) {
				if (newgame == true) {
					newgame = false;
					loadgame = true;
				}
				else if (loadgame == true) {
					loadgame = false;
					quitgame = true;
				}
				else {
					quitgame = false;
					newgame = true;
				}
			}
			break;
	}
}
/** This function is for setting up the initial rendering conditions of the game, which includes initialising GL modes, loading various images into GLUint variables, read and load models from .obj and .mtl files, etc.*/
void initRendering() {
	glEnable(GL_DEPTH_TEST); //enables 3D stuff
	glEnable(GL_COLOR_MATERIAL); //enable color
	glEnable(GL_NORMALIZE); //automatically normalize normals of surfaces
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	logo = loadImage("images/logo.png");
	monopoly_text = loadImage("images/monopoly.jpg");
	blackwood = loadImage("images/blackwood.jpg");
	parchment = loadImage("images/parchment.png");
	button_icon = loadImage("images/button.png");
	pay_icon = loadImage("images/pay.png");
	done_icon = loadImage("images/done.png");
	undo_icon = loadImage("images/undo.png");
	mortgage_icon = loadImage("images/mortgage.png");
	view_icon = loadImage("images/view.png");
	report_icon = loadImage("images/report.png");
	d1 = loadImage("images/1.jpeg");
	d5 = loadImage("images/5.jpg");
	d10 = loadImage("images/10.jpg");
	d20 = loadImage("images/20.jpg");
	d50 = loadImage("images/50.jpg");
	d100 = loadImage("images/100.jpg");
	d500 = loadImage("images/500.jpg");
	earth = glmReadOBJ("objects/earth.obj");
	glmUnitize(earth);
	glmFacetNormals(earth);
	glmVertexNormals(earth,180.0,false);
	road = glmReadOBJ("objects/road.obj");
	glmUnitize(road);
	glmFacetNormals(road);
	glmVertexNormals(road,180.0,false);
	dice1 = glmReadOBJ("objects/dice.obj");
	glmVertexNormals(dice1,180.0,false);
	dice2 = glmReadOBJ("objects/dice.obj");
	glmVertexNormals(dice2,180.0,false);
	house = glmReadOBJ("objects/house.obj");
	glmVertexNormals(house,180.0,false);
	hotel = glmReadOBJ("objects/hotel.obj");
	glmVertexNormals(hotel,180.0,false);
}
/** This function is for handling resizing the game, although the game opens in full-screen mode by default.*/
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w/(double)h, 1.0, 200.0);
}
/** This function is for handling interrupts to the game. This could include clicks on action buttons, menu buttons and bars, handling user text input, etc.*/
void check_interrupts(void) {
	if (interrupt == true) {
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, parchment);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-74.0f, 42.0f, -98.989f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(74.0f, 42.0f, -98.989f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(74.0f, -42.0f, -98.989f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-74.0f, -42.0f, -98.989f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.0f, 0.0f, 0.0f);
		if (rules == true) {
			glPushMatrix();
			glTranslatef(-70.0f, 37.0f+menubar, -98.98f);
			glScalef(0.015f, 0.015f, 0.015f);
			dtx_string("Rules");
			glPopMatrix();
		}
		else if (status == true) {
			glPushMatrix();
			glTranslatef(-54.0f, 37.0f+menubar, -98.98f);
			glScalef(0.015f, 0.015f, 0.015f);
			dtx_string("Status");
			glPopMatrix();
		}
		else if (properties == true) {
			glPushMatrix();
			glTranslatef(-36.0f, 37.0f+menubar, -98.98f);
			glScalef(0.015f, 0.015f, 0.015f);
			dtx_string("Properties");
			glPopMatrix();
		}
		else if (bid == true) {
			glPushMatrix();
			glTranslatef(-11.0f, 37.0f+menubar, -98.98f);
			glScalef(0.015f, 0.015f, 0.015f);
			dtx_string("Bid");
			glPopMatrix();
		}
		else if (trade == true) { 
			glPushMatrix();
			glTranslatef(0.0f, 37.0f+menubar, -98.98f);
			glScalef(0.015f, 0.015f, 0.015f);
			dtx_string("Trade");
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glTranslatef(15.0f, 37.0f+menubar, -98.98f);
			glScalef(0.015f, 0.015f, 0.015f);
			dtx_string("Quit");
			glPopMatrix();
		}
	}
	if (sideinterrupt == true and my_properties == true) {
		glColor4f(0.8f, 0.0f, 0.0f, 0.8f);
		glPushMatrix();
		glBegin(GL_QUADS);
		glVertex3f(72.0f+sidebar, 35.0f, -98.989f);
		glVertex3f(35.0f+sidebar, 35.0f, -98.989f);
		glVertex3f(35.0f+sidebar, -40.0f, -98.989f);
		glVertex3f(72.0f+sidebar, -40.0f, -98.989f);
		glEnd();
		glPopMatrix();
	}
	if (sideinterrupt == true and all_players == true) {
		glColor4f(0.0f, 0.4f, 0.0f, 0.8f);
		glPushMatrix();
		glBegin(GL_QUADS);
		glVertex3f(72.0f+sidebar, 29.0f, -98.989f);
		glVertex3f(35.0f+sidebar, 29.0f, -98.989f);
		glVertex3f(35.0f+sidebar, -40.0f, -98.989f);
		glVertex3f(72.0f+sidebar, -40.0f, -98.989f);
		glEnd();
		glPopMatrix();
	}
	if (pay == true) {
		// deposit money
		to_submit = 0;
		pay = false;
	}
	if (undo == true) {
		account_value = account_value + to_submit;
		to_submit = 0;
		// undo selection of players etc.
		undo = false;
	}
}
/** This function is for running dice shuffles when the current player is in turn.*/
void shuffleDice() {
	int dc1 = rand() % 6 + 1;
	int dc2 = rand() % 6 + 1;
	switch (dc1) {
		case 1: 
			dice1_ang11 = 270.0;
			dice1_ang21 = 0.0;
			break;
		case 2: 
			dice1_ang11 = 0.0;
			dice1_ang21 = 90.0;
			break;
		case 3: 
			dice1_ang11 = 180.0;
			dice1_ang21 = 0.0;
			break;
		case 4: 
			dice1_ang11 = 0.0;
			dice1_ang21 = 0.0;
			break;
		case 5: 
			dice1_ang11 = 0.0;
			dice1_ang21 = 270.0;
			break;
		case 6: 
			dice1_ang11 = 90.0;
			dice1_ang21 = 0.0;
	}
	switch (dc2) {
		case 1: 
			dice2_ang11 = 270.0;
			dice2_ang21 = 0.0;
			break;
		case 2: 
			dice2_ang11 = 0.0;
			dice2_ang21 = 90.0;
			break;
		case 3: 
			dice2_ang11 = 180.0;
			dice2_ang21 = 0.0;
			break;
		case 4: 
			dice2_ang11 = 0.0;
			dice2_ang21 = 0.0;
			break;
		case 5: 
			dice2_ang11 = 0.0;
			dice2_ang21 = 270.0;
			break;
		case 6: 
			dice2_ang11 = 90.0;
			dice2_ang21 = 0.0;
	}
	dice_value = dc1 + dc2;
	cout<<"Dice: "<<dice_value<<endl;
}
/** This function is for drawing the main board of the game.*/
void drawBoard() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(-2.0f,0.0f,-10.0f);
	glRotatef(360.0f-angle, 0.2f, 1.0f, 0.2f);
	glScalef(5.0f, 5.0f, 5.0f);
	glmDraw(earth,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.0f,0.0f,-10.0f);
	glRotatef(angle, 0.2f, 1.0f, 0.2f);
	glScalef(4.0f, 4.0f, 4.0f);
	glmDraw(road,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glPopMatrix();
	if (dice_shuffle == true) {
		shuffleDice();
		dice_shuffle = false;
	}
	glPushMatrix();
	glTranslatef(-2.0f,-1.0f,-3.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	glRotatef(dice1_ang1, 1.0f, 0.0f, 0.0f);
	glRotatef(dice1_ang2, 0.0f, 1.0f, 0.0f);
	glmDraw(dice1,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.7f,-1.0f,-3.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	glRotatef(dice2_ang1, 1.0f, 0.0f, 0.0f);
	glRotatef(dice2_ang2, 0.0f, 1.0f, 0.0f);
	glmDraw(dice2,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.2f,-1.8f,-5.0f);
	glScalef(0.012f, 0.012f, 0.012f);
	glRotatef(slow_rot, 0.0f, 1.0f, 0.0f);
	glmDraw(house,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.6f,-1.8f,-5.0f);
	glScalef(0.4f, 0.4f, 0.4f);
	glRotatef(slow_rot, 0.0f, 1.0f, 0.0f);
	glmDraw(hotel,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glPopMatrix();
}
/** This function is for drawing the scene, and is the input to glut's display function.*/
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear information from last draw
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	if (start_screen == true) {
		glClearColor(0.4f, 1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, logo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else if (main_menu == true) {		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		
		glTranslatef(0.0f, 0.0f, -4.0f);
		glPushMatrix();		
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, monopoly_text);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glRotatef(mono_angle, 0.0f, 0.0f, 1.0f);
		glScalef(scale, scale, scale);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-3.0f, 0.5f + go_up, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.0f, 0.5f + go_up, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.0f, -1.5f + go_up, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-3.0f, -1.5f + go_up, 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);	
		glPopMatrix();			
		glPushMatrix();	
		glTranslatef(-2.8f, 0.5f, -6.0f);
		glColor4f(0.0f, 0.0f, 0.0f, fade_in);
		glScalef(0.005f, 0.005f, 0.005f);
		if (newgame == true) {dtx_use_font(main_font, 200);}	
		else {dtx_use_font(dash_font, 200);}
		dtx_string("New Game");
		glPopMatrix();					
		glPushMatrix();	
		glTranslatef(-2.9f, -1.0f, -6.0f);
		glColor4f(0.0f, 0.0f, 0.0f, fade_in);
		glScalef(0.005f, 0.005f, 0.005f);		
		if (loadgame == true) {dtx_use_font(main_font, 200);}	
		else {dtx_use_font(dash_font, 200);}	
		dtx_string("Load Game");
		glPopMatrix();					
		glPushMatrix();	
		glTranslatef(-1.1f, -2.5f, -6.0f);
		glColor4f(0.0f, 0.0f, 0.0f, fade_in);
		glScalef(0.004f, 0.004f, 0.004f);		
		if (quitgame == true) {dtx_use_font(main_font, 200);}	
		else {dtx_use_font(dash_font, 200);}
		dtx_string("Quit");
		glPopMatrix();
	}
	else if (game_begins == true) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, blackwood);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-100.0f, 50.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(100.0f, 50.0f, -100.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(100.0f, -50.0f, -100.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-100.0f, -50.0f, -100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		dtx_use_font(mono_font, 200);
		glPushMatrix();
		glTranslatef(-70.0f, 37.0f+menubar, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("Rules");
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-54.0f, 37.0f+menubar, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("Status");
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-36.0f, 37.0f+menubar, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("Properties");
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-11.0f, 37.0f+menubar, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("Bid");
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0f, 37.0f+menubar, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("Trade");
		glPopMatrix();
		glPushMatrix();
		glTranslatef(15.0f, 37.0f+menubar, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("Quit");
		glPopMatrix();
		glColor3f(0.8f, 0.0f, 0.0f);
		glPushMatrix();
		glBegin(GL_QUADS);
		glVertex3f(72.0f+sidebar, 41.0f, -99.0f);
		glVertex3f(35.0f+sidebar, 41.0f, -99.0f);
		glVertex3f(35.0f+sidebar, 35.0f, -99.0f);
		glVertex3f(72.0f+sidebar, 35.0f, -99.0f);
		glEnd();
		glPopMatrix();
		glColor3f(1.0f, 0.843f, 0.0f);
		glPushMatrix();
		glTranslatef(39.0f+sidebar, 36.0f, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("MY PROPERTIES");
		glPopMatrix();
		glColor3f(0.0f, 0.4f, 0.0f);
		glPushMatrix();
		glBegin(GL_QUADS);
		glVertex3f(72.0f+sidebar, 35.0f, -99.0f);
		glVertex3f(35.0f+sidebar, 35.0f, -99.0f);
		glVertex3f(35.0f+sidebar, 29.0f, -99.0f);
		glVertex3f(72.0f+sidebar, 29.0f, -99.0f);
		glEnd();
		glPopMatrix();
		glColor3f(1.0f, 0.843f, 0.0f);
		glPushMatrix();
		glTranslatef(41.0f+sidebar, 30.0f, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("ALL PLAYERS");
		glPopMatrix();
		glColor3f(0.75f, 0.75f, 0.75f);
		glPushMatrix();
		glTranslatef(41.0f+sidebar, 24.0f, -98.99f);
		glScalef(0.015f, 0.015f, 0.015f);
		dtx_string("MY ACCOUNT");
		glPopMatrix();
		glPushMatrix();
		glTranslatef(41.0f+sidebar, 17.0f, -98.99f);
		glScalef(0.025f, 0.025f, 0.025f);
		stringstream ss;
		ss << account_value;
		string val = ss.str();
		string money = '$' + val;
		dtx_string(money.c_str());
		glPopMatrix();
		glPushMatrix();
		glTranslatef(41.0f+sidebar, -38.0f, -98.99f);
		glScalef(0.025f, 0.025f, 0.025f);
		stringstream ss1;
		ss1 << to_submit;
		string val1 = ss1.str();
		string money1 = '$' + val1;
		dtx_string(money1.c_str());
		glPopMatrix();
		//currency
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, d1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(38.0f+(sidebar/2), 14.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(68.0f+(sidebar/2), 14.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(68.0f+(sidebar/2), 0.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(38.0f+(sidebar/2), 0.0f, -99.0f);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, d5);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(38.0f+(sidebar/2), -2.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(68.0f+(sidebar/2), -2.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(68.0f+(sidebar/2), -16.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(38.0f+(sidebar/2), -16.0f, -99.0f);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, d10);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(38.0f+(sidebar/2), -18.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(68.0f+(sidebar/2), -18.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(68.0f+(sidebar/2), -32.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(38.0f+(sidebar/2), -32.0f, -99.0f);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, d20);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(48.0f+sidebar, 14.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(78.0f+sidebar, 14.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(78.0f+sidebar, 0.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(48.0f+sidebar, 0.0f, -98.99f);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, d50);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(48.0f+sidebar, -2.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(78.0f+sidebar, -2.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(78.0f+sidebar, -16.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(48.0f+sidebar, -16.0f, -98.99f);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, d100);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(48.0f+sidebar, -18.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(78.0f+sidebar, -18.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(78.0f+sidebar, -32.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(48.0f+sidebar, -32.0f, -98.99f);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		//icons
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glScalef(butscale, butscale, 1.0f);
		glBindTexture(GL_TEXTURE_2D, button_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(21.0f, 31.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(31.0f, 31.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(31.0f, 21.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(21.0f, 21.0f, -99.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, pay_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(22.0f, 30.5f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(29.0f, 30.5f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(29.0f, 23.5f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(22.0f, 23.5f, -98.99f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, button_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(21.0f, 20.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(31.0f, 20.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(31.0f, 10.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(21.0f, 10.0f, -99.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, done_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(23.0f, 19.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(30.0f, 19.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(30.0f, 11.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(23.0f, 11.0f, -98.99f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, button_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(21.0f, 9.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(31.0f, 9.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(31.0f, -1.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(21.0f, -1.0f, -99.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, undo_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(22.5f, 8.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(29.5f, 8.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(29.5f, 0.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(22.5f, 0.0f, -98.99f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, button_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(21.0f, -2.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(31.0f, -2.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(31.0f, -12.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(21.0f, -12.0f, -99.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, mortgage_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(22.5f, -3.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(29.5f, -3.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(29.5f, -11.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(22.5f, -11.0f, -98.99f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, button_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(21.0f, -13.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(31.0f, -13.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(31.0f, -23.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(21.0f, -23.0f, -99.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, view_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(22.5f, -14.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(29.5f, -14.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(29.5f, -22.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(22.5f, -22.0f, -98.99f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, button_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(21.0f, -24.0f, -99.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(31.0f, -24.0f, -99.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(31.0f, -34.0f, -99.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(21.0f, -34.0f, -99.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, report_icon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(22.5f, -25.0f, -98.99f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(29.5f, -25.0f, -98.99f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(29.5f, -33.0f, -98.99f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(22.5f, -33.0f, -98.99f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		check_interrupts(); //treat clicks as interrupts
		drawBoard();//call game-board drawing function
	}
	glutSwapBuffers(); //Send the 3D scene to the screen
}
/** This function is for handling updates in the game. This would include all animation movements, game state transitions, and updating appropriate boolean variables.*/
void update(int value) {
	angle += 4.0;
	if (angle > 360.0) {angle = angle - 360.0;}
	if (main_menu == true) {
		if (zoomout == true) {scale -= 0.01; mono_angle -= 6; go_up += 0.027; if (scale <= 0.4) {zoomout = false; mono_angle = 0;}}
		else {	
			if (clkwise == true) {mono_angle += scale; if (mono_angle > 4.0) {clkwise = false;}}
			if (clkwise == false) {mono_angle -= scale; if (mono_angle < -4.0) {clkwise = true;}}
			if (fade_in < 1.0) {fade_in += 0.01;}
		}
	}
	if (game_begins == true) {
		if(menubar > 0) {menubar = menubar - 0.1; sidebar = sidebar - 0.6; butscale = butscale + 0.017;}
		if(slow_rot < 360) {slow_rot += 0.5;}
		else {slow_rot = 0;}
		if(dice_trans > 0) {
			dice_trans--;
			dice1_ang1 = rand() % 360;
			dice1_ang2 = rand() % 360;
			dice2_ang1 = rand() % 360;
			dice2_ang2 = rand() % 360;
		}
		else {
			dice_visible = true;
			dice1_ang1 = dice1_ang11;
			dice1_ang2 = dice1_ang21;
			dice2_ang1 = dice2_ang11;
			dice2_ang2 = dice2_ang21;
		}
			//call game advancing function
	}
	glutPostRedisplay();
	glutTimerFunc(TIME_STEP, update, 0);
}
/** This is the main function, which passes various inputs to glut functions, and sets glut's Main Loop running.*/
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Monopoly - The Fast-Dealing Property Trading Game");
	glutFullScreen();
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);
	glutMouseFunc(handleMouseclick);
	glutReshapeFunc(handleResize);
	glutTimerFunc(TIME_STEP, update, 0);
	glutMainLoop(); 
	return 0; 
}
