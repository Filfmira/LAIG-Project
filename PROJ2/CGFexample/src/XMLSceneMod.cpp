#include "XMLSceneMod.h"


#include <iostream>
#include "GL/glui.h"
#include "GL/glut.h"
#include <stdlib.h>     /* strtof */
#include <cstdlib>
#include "CGFapplication.h"


XMLSceneMod::XMLSceneMod(char *filename, Graph* gr, Light** lig, vector<Texture* > &textures, 
	vector<Appearance* > &appearances,vector<Animation* > &animations, vector<Camera >*cameras, Camera* &activeCamera, 
	Global *globals, vector<FlagShader*> &flagShaders): destinationGraph(gr), destinationLights(lig)
{

	this->cameras=cameras;
	this->initialCamera = NULL;
	this->globals=globals;
	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* dgxElement= doc->FirstChildElement( "anf" );

	if (dgxElement == NULL)
	{
		printf("Main anf block element not found! Exiting!\n");
		cin.get();
		exit(1);
	}



	readGlobals(dgxElement);

	if (!readCameras(dgxElement)){
		cin.get();
		exit(1);
	}
	activeCamera = initialCamera;
//	cin.get();


	if (!readLights(dgxElement)){
		cin.get();
		exit(1);
	}
//	cin.get();


	if(!readTextures(dgxElement, textures)){
		cin.get();
		exit(1);
	}
//	cin.get();


	if (!readAppearances(dgxElement, appearances, textures)){
		cin.get();
		exit(1);
	}
//	cin.get();
	
	if (!readAnimations(dgxElement,animations))
	{
		cin.get();
		exit(1);
	}

	if (!readGraph(dgxElement, appearances,animations,flagShaders)){
		cin.get();
		exit(1);
	}
//	cin.get();


}

bool XMLSceneMod::readRGBcomponents (char* rawString, float &R, float &G, float &B, float &A){
	if (sscanf(rawString,"%f %f %f %f",&R, &G, &B, &A) != 4)
		return false;
	if ( R>1 || G>1 || B>1 || A>1 )
		return false;
	if ( R<0 || G<0 || B<0 || A<0 )
		return false;
	return true;
}

bool XMLSceneMod::readXYcoord (char* rawString, float &x, float &y){
	return (sscanf(rawString,"%f %f",&x, &y) == 2);
}

bool XMLSceneMod::readXYZcoord (char* rawString, float &x, float &y, float &z){
	return (sscanf(rawString,"%f %f %f",&x, &y, &z) == 3);
}

bool XMLSceneMod::readFloatArray(char* rawString, float (&a)[4])
{
	return(sscanf(rawString,"%f %f %f %f",&a[0],&a[1],&a[2],&a[3]));
}

XMLSceneMod::~XMLSceneMod()
{
	delete(doc);
}



bool XMLSceneMod::readGlobals(TiXmlElement* dgxElement){


	globalsElement = dgxElement->FirstChildElement( "globals" );
	//drawing
	char* mode;
	char* shading;
    float background[4];
	//culling
	char* face;
	char* order;

	//lighting
	bool doublesided;
	bool local;
	bool enabled;
	float ambient[4];


	if (globalsElement == NULL){
		printf("globals block not found!\n");
		return false;
	}
	else
	{
		printf("Processing globals:\n");

		//////////drawing//////////////
		TiXmlElement* drawingElement = globalsElement->FirstChildElement("drawing");
		if (drawingElement)
		{
			printf("- Drawing:\n");
			

			mode = (char *) drawingElement->Attribute("mode");
			shading = (char *) drawingElement->Attribute("shading");

			if (strcmp(mode, "fill") != 0 && strcmp(mode, "line") != 0 && strcmp(mode, "point") != 0){
				printf("Error parsing drawing: wrong/missing mode variable\n");
				mode = "error";
			}
			
			if (strcmp(shading, "gouraud") != 0 && strcmp(shading, "flat") != 0){
				printf("Error parsing drawing: missing shading variable\n");
				shading = "error";
			}
			
			printf("	mode: %s\n	shading: %s\n", mode, shading);

			char *backgroundS=NULL;

			backgroundS=(char *) drawingElement->Attribute("background");
			if(backgroundS && readRGBcomponents(backgroundS,background[0], background[1], background[2], background[3]))
			{
					printf("	background: R-%f, G-%f, B-%f, A-%f\n", background[0], background[1], background[2], background[3]);
			}
			else{
				printf("Error parsing background\n");
			}
		}
		else
			printf("drawing not found\n");

		/////////////////end drawing////////////

		///////culling//////////
		TiXmlElement* cullingElement = globalsElement->FirstChildElement("culling");
		if (cullingElement)
		{
			printf("- Culling:\n");
			

			face = (char *) cullingElement->Attribute("face");
			order = (char *) cullingElement->Attribute("order");

			if (strlen(face) == 0){ 
				printf("Error parsing culling: missing face variable\n");
				face = "error";
			}
			if (strcmp(order,"cw") != 0 && strcmp(order,"ccw") != 0){ 
				printf("Error parsing culling: missing/wrong order variable\n");
				order = "error";
			}
			
			printf("	face: %s\n	order: %s\n", face, order);
		}
		else
			printf("culling not found\n");
		////////////////end culling////////////////


		////////////lighting/////////////
		TiXmlElement* lightingElement = globalsElement->FirstChildElement("lighting");
		
		if (lightingElement)
		{
			printf("- Lighting:\n");

			char* doublesidedS = (char *) lightingElement->Attribute("doublesided");
			char* localS = (char *) lightingElement->Attribute("local");
			char* enabledS = (char *) lightingElement->Attribute("enabled");
			if (strcmp(doublesidedS,"true") != 0)		doublesided=true;
			else if(strcmp(doublesidedS,"false") != 0)	doublesided=false;
			else
			{
				printf("Error parsing lighting: missing or wrong type doublesided variable\n");
				doublesidedS = "error";
			}

			if (strcmp(localS,"true") != 0)			local=true;
			else if(strcmp(localS,"false") != 0)	local=false;
			else{
				printf("Error parsing lighting: missing or wrong type local variable\n");
				localS = "error";
			}
			if (strcmp(enabledS,"true") != 0)			enabled=true;
			else if(strcmp(enabledS,"false") != 0)		enabled=false;
			else{
				printf("Error parsing lighting: missing or wrong type local variable\n");
				localS = "error";
			}

			printf("	doublesided: %s\n	local: %s\n	enabled: %s\n", doublesidedS, localS, enabledS);
		

			char *ambientS=NULL;
			
			ambientS=(char *) lightingElement->Attribute("ambient");
			if(ambient && readRGBcomponents(ambientS,ambient[0], ambient[1], ambient[2], ambient[3]))
			{
					printf("	ambient: R-%f, G-%f, B-%f, A-%f\n",  ambient[1], ambient[2], ambient[3]);
			}
			else{
				printf("Error parsing ambient\n");
			}
		}
		else
			printf("lighting not found\n");

		/////////////////end lighting ////////////////////////
	}

	globals->setMode(mode);
	globals->setShading(shading);
	globals->setBackground(&background[0]);
	globals->setFace(face);
	globals->setOrder(order);
	globals->setDoublesided(doublesided);
	globals->setLocal(local);
	globals->setEnabled(enabled);
	globals->setAmbient(&ambient[0]);
	return true;
	////////////////////// END GLOBALS /////////////////////
}


bool XMLSceneMod::readCameras(TiXmlElement* dgxElement){
	cout<<"\nParsing cameras:\n";

	camerasElement=dgxElement->FirstChildElement("cameras");
	if (!camerasElement){
		printf("Cameras block not found! Program will end...\n");
		return false;
	}

	int initial;
	char* initial_T = (char*) camerasElement->Attribute("initial");
	if (initial_T){
		initial = atoi(initial_T); 
		printf("initial: %d \n",initial);
	}


	printf("-Perspective\n");

	
	//prespective cameras
	TiXmlElement * perspectives = camerasElement->FirstChildElement("perspective");
	while(perspectives)
	{

		bool valid = true;

		float near;
		float far;
		float angle;
		float pos[3];
		float target[3];

		//id
		
		
		char* id = (char*) perspectives->Attribute("id"); 
		if (!id)
			valid = false;
		else printf("        id: %s \n",id);

		//near
		char* near_T=(char*) perspectives->Attribute("near");
		if (!near_T || !sscanf(near_T,"%f",&near))
			valid = false;
		else printf("        near: %f\n",near);
		

		//far
		char* far_T=(char*) perspectives->Attribute("far");
		if (!far_T || !sscanf(far_T,"%f",&far))
			valid = false;
		else printf("        far: %f\n",far);

		//angle
		char* angle_T=(char*) perspectives->Attribute("angle");
		if (!angle_T || !sscanf(angle_T,"%f",&angle))
			valid = false;
		else printf("        angle: %f\n",angle);
		

		//pos
		char* pos_T=(char*) perspectives->Attribute("pos");
		if (!pos_T || !readXYZcoord(pos_T,pos[0],pos[1],pos[2]))
			valid = false;
		else printf("        pos: %s\n",pos_T);

		//target
		char* target_T=(char*) perspectives->Attribute("target");
		if (!target_T || !readXYZcoord(target_T,target[0],target[1],target[2]))
			valid = false;
		else printf("        target: %s\n",target_T);

		if (valid)
			cameras->push_back(Camera(new Perspective(near,far,angle,&pos[0],&target[0]),(string) id, "Perspective"));
		else printf ("ERROR reading one of the Perspective cameras. Program will try to run anyway.\n");
		printf("-------------------------------\n");
		perspectives=perspectives->NextSiblingElement("perspective");
	}

	printf("-Ortho\n");
	//ortho cameras
	TiXmlElement * orthos = camerasElement->FirstChildElement("ortho");
	while(orthos)
	{
		bool valid = true;

		char* direction;
		float near;
		float far;
		float left;
		float right;
		float top;
		float bottom;

		
		//id
		char* id= (char*) orthos->Attribute("id");
		if (!id)
			valid = false;
		else printf("        id: %s \n",id);

		//direction
		direction=(char*) orthos->Attribute("direction");
		if(direction!="")
		{
			if(strcmp(direction,"xx")||strcmp(direction,"yy")||strcmp(direction,"xx"))
			{
				printf("        direction: %s\n",direction);
			}
			else printf("Error parsing camera: bad direction input \n");
			
		}
		else {
			valid = false;
			printf("Error parsing camera: missing direction\n");
		}


		//near
		char* near_T=(char*) orthos->Attribute("near");
		if (!near_T || !sscanf(near_T,"%f",&near))
			valid = false;
		else printf("        near: %f\n",near);

		//far
		char* far_T=(char*) orthos->Attribute("far");
		if (!far_T || !sscanf(far_T,"%f",&far))
			valid = false;
		else printf("        far: %f\n",far);

		//left
		char* left_T=(char*) orthos->Attribute("left");
		if (!left_T || !sscanf(left_T,"%f",&left))
			valid = false;
		else printf("        left: %f\n",left);

		//right
		char* right_T=(char*) orthos->Attribute("right");
		if (!right_T || !sscanf(right_T,"%f",&right))
			valid = false;
		else printf("        right: %f\n",right);
		
		//top
		char* top_T=(char*) orthos->Attribute("top");
		if (!top_T || !sscanf(top_T,"%f",&top))
			valid = false;
		else printf("        top: %f\n",top);
		
		//bottom
		char* bottom_T=(char*) orthos->Attribute("bottom");
		if (!bottom_T || !sscanf(far_T,"%f",&bottom))
			valid = false;
		else printf("        bottom: %f\n",bottom);

		if (valid)
			cameras->push_back(Camera(new Ortho(direction,near,far,left,right,top,bottom),id, "Ortho"));
		else printf ("ERROR reading one of the Ortho cameras. Program will try to run anyway.\n");
		
		printf("-------------------------------\n");

		orthos=orthos->NextSiblingElement("ortho");
	}

	//verificar se a c�mera indicada como inicial existe
	if (cameras->size() == 0){
		printf("No cameras read... Program will end...\n");
		return false;
	}

	for (unsigned int i = 0; i < cameras->size(); i++){
		if (cameras->at(i).getId() == (string) initial_T){
			initialCamera = &(cameras->at(i));
			return true;
		}
	}
	printf("Initial camera wasn't loaded... Program will end...\n");
	return false;
}

bool XMLSceneMod::readLights(TiXmlElement* dgxElement){
	cout<<"\nParsing lights:\n";
	lightsElement=dgxElement->FirstChildElement("lights");
	if(!lightsElement){
		printf("Lights block not found! Program will end...\n");
		return false;
	}

	TiXmlElement * lights = lightsElement->FirstChildElement("light");

	for (unsigned int i = 0; lights && i < 8; i++){

		bool validLight = true;

		char* id = "gg";
		char* type;
		bool enabled;
		bool marker;
		float pos[4];
		float target[3];
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float angle;
		float exponent;

		//id
		id = (char*) lights->Attribute("id");
		if (id){
			printf("id: %s\n", id);
		}
		else validLight = false;

		//type
		type = (char*) lights->Attribute("type");
		if (type){
			printf("	type: %s \n",type);
		}
		else validLight = false;

		//enabled
		char* enabledChr = (char*) lights->Attribute("enabled");
		printf("	enabled: %s \n",enabledChr);
		if (strcmp(enabledChr,"true") == 0)
			enabled=true;
		else  if (strcmp(enabledChr,"false") == 0)
			enabled=false;
		else{
			 printf("	Unexpected problem with light's enabled value\n");
			 validLight = false;
		}

		//marker
		char* markerChr = (char*) lights->Attribute("marker");
		printf("	marker: %s \n",markerChr);
		if (strcmp(markerChr,"true") == 0)
			marker=true;
		else  if (strcmp(markerChr,"false") == 0)
			marker=false;
		else{
			 printf("	Unexpected problem with light's marker value\n");
			 validLight = false;
		}


		//pos array
		char* pos_T = (char*) lights->Attribute("pos");
		if (pos_T){
			printf("	pos: %s \n",pos_T);
			float x,y,z;
			if(!readXYZcoord(pos_T,x,y,z)){
				printf("	Unexpected problem with light's position values \n");
				validLight = false;
			}
			else{ pos[0] = x; pos[1] = y; pos[2] = z;}
		}
		else{
			validLight = false;
			printf("	Light position missing\n");
		}

		

		//angle and exponent - only for spot lights
		if ( strcmp(type, "omni") == 0 ){
			angle = 360; 
			exponent = 1; 
			target[0] = NULL; target[1] = NULL; target[2] = NULL;
		}
		else{
			char* ang = (char*) lights->Attribute("angle");
			char* exp = (char*) lights->Attribute("exponent");
			char* trg = (char*) lights->Attribute("target");

			if (ang && exp && trg){
				angle = atof(ang);
				exponent = atof(exp);
				printf("	Angle: %f, Exponent: %f\n", angle, exponent);
				//target array - only for spot lights
				float x,y,z;
				if(!readXYZcoord(trg,x,y,z)){
					printf("	Unexpected problem with light's target values \n");
					validLight = false;
				}
				else{ target[0] = x; target[1] = y; target[2] = z;}
			}
			else{ validLight = false; printf ("	Unexpected problem with angle, target or exponent light values\n");}
		}
	

		TiXmlElement * components = lights->FirstChildElement("component");
		
		bool amb, dif, spec;
		amb = dif = spec = false;
		while(components)
		{
			char* typeComponent = (char*) components->Attribute("type");
			if (!typeComponent)
				validLight = false;
			else printf("	type: %s   ",typeComponent);

			char* valueComponent=(char*) components->Attribute("value");
			if (!valueComponent)
				validLight = false;
			else printf("	values: %s \n",valueComponent);


			if (typeComponent && valueComponent){ //se estes valores existem
				
				if (strcmp(typeComponent,"ambient") == 0){
					if(!readFloatArray(valueComponent, ambient)){
						printf("	Unexpected problem with ambient component's values of lights\n");
						validLight = false;
					}
					amb = true;
				}
				else if (strcmp(typeComponent,"diffuse") == 0){
					if(!readFloatArray(valueComponent, diffuse)){
						printf("	Unexpected problem with diffuse component's values of lights\n");
						validLight = false;
					}
					dif = true;
				}
				else if (strcmp(typeComponent,"specular") == 0){
					if(!readFloatArray(valueComponent, specular)){
						printf("	Unexpected problem with specular component's values of lights\n");
						validLight = false;
					}
					spec = true;
				}
				else{
					printf("	Unexpected light component... Program will run anyway\n");
				}

			}
			else {
				printf("	missing type and/or value of light component... Program will run anyway\n");
			}
			

			components=components->NextSiblingElement();
		}

		if (validLight && amb && dif && spec){
			unsigned int i = 0;
			for (i = 0; i < 8; i++)
				if (destinationLights[i] == NULL){
					Light* newLight = new Light((string) type, (string) id, enabled, marker, pos, target, angle, exponent, ambient, diffuse, specular, i);
					destinationLights[i] = newLight;
					break;
				}
			if (i == 8) printf("	Limit number of lights reached (8). This, and any further lights read, will be discarded...\n");
		}
		else printf ("Problem reading light... Program will try to run anyway...\n");
		printf("-------------------------------\n");
		lights=lights->NextSiblingElement();

	}
	
	if (destinationLights[0] != NULL ) //v� se fez load de pelo menos uma luz
		return true;

	else printf("No lights loaded... Program will end...\n");

	return false;
}

bool XMLSceneMod::readTextures(TiXmlElement* dgxElement, vector<Texture*> &text){

	cout<<"\nParsing textures:\n";


	texturesElement=dgxElement->FirstChildElement("textures");
	if (!texturesElement){
		printf("Missing textures block. Program will end...\n");
		return false;
	}
	
	TiXmlElement * texture = texturesElement->FirstChildElement("texture");
	while(texture)
	{
		bool validTexture = true;

		char* id;
		char* file;
		float texlength_s;
		float texlength_t;

		 id = (char*) texture->Attribute("id");
		 if (!id){
			 printf("	Problem with texture id\n");
			 validTexture = false;
		 }
		 else printf("	-Id: %s\n",id);

		 file=(char*) texture->Attribute("file");
		 if (!file){
			 validTexture = false;
			 printf("	Problem with texture file name\n");
		 }
		 else printf("	File: %s\n",file);


		 char* texlength_s_T=(char*) texture->Attribute("texlength_s");
		 if (!texlength_s_T || !sscanf(texlength_s_T,"%f",&texlength_s)){
			 validTexture = false;
			 printf("	Problem with texture texlength_s value...\n");
		 }
		 else printf("        Textlength_s: %s\n",texlength_s_T);

		 char* texlength_t_T=(char*) texture->Attribute("texlength_t");
		 if (!texlength_t_T || !sscanf(texlength_t_T,"%f",&texlength_t)){
			 validTexture = false;
			 printf("	Problem with texture texlength_t value...\n");
		 }
		 else printf("        Texlength_t: %s\n",texlength_t_T);

		 if (validTexture)
			 text.push_back(new Texture(id,file,texlength_s,texlength_t));
		 else printf ("Problem loading texture. Program will try to run anyway...\n");
		 printf("-------------------------------\n");
		 texture=texture->NextSiblingElement("texture");
	}
	cout<<endl;
	
	return true;
}


bool XMLSceneMod::readAppearances(TiXmlElement* dgxElement, vector<Appearance*> &appearances, vector<Texture*> &text){
	cout<<"\nParsing appearances:\n\n";

	appearancesElement=dgxElement->FirstChildElement("appearances");
	if (!appearancesElement){
		printf("Missing appearances block. Program will end...\n");
		return false;
	}

	TiXmlElement * appearance = appearancesElement->FirstChildElement("appearance");
	
	char* id;
	char* textureref;
	float shininess;

	while(appearance){

		bool validAppearance = true;

		//read the appearance's id
		id = (char*)appearance->Attribute("id");
		if(!id){
			validAppearance = false;
			printf("	Problem with appearance id...\n");
		}
		else printf("	id: %s\n",id);

		Appearance* appearanceObject= new Appearance(id);

		//read the appearance's shininess
		char* ss = (char*)appearance->Attribute("shininess");
		if (!ss || !sscanf(ss,"%f",&shininess)){
			validAppearance = false;
			printf("	Problem with appearance shininess value...\n");
		}
		else{
			printf("	shininess: %f\n", shininess);
			appearanceObject->setShininess(shininess);
		}


		//read the appearance's textured reference
		textureref = (char*)appearance->Attribute("textureref");
		if(textureref){
			bool exists=false;
			for(unsigned int i=0;i<text.size();i++)
				if(strcmp(text[i]->getId(),textureref)==0){
					exists=true;
					appearanceObject->setTexture(text[i]);
				}

			if(exists) cout<<"        Textureref: "<<textureref<<endl;
			else {cout<<"			Error parsing appearance: texture does not exist\n"; validAppearance = false;}
		}
		


		//read the appearence's elements(ambient,diffuse,specular)
		TiXmlElement *component = appearance->FirstChildElement("component");  
		string type;
		float floatVector[4];
		bool a,d,s;
		a=d=s=false;
		while(component)
		{
			

			type = string(component->Attribute("type"));
			cout<<"        Type:"<<type;
			cout<<"-> "<<(char*)component->Attribute("value")<<endl;
			if(!readFloatArray((char*)component->Attribute("value"),floatVector))
				cout<<"        Error reading values"<<endl;
			if(type=="ambient")
			{
				a=true;
				

				appearanceObject->setAmbient(&floatVector[0]);
			}
			else if(type=="diffuse")
			{
				d=true;
				appearanceObject->setDiffuse((&floatVector)[0]);
			}
			else if(type=="specular")
			{
				s=true;
				appearanceObject->setSpecular((&floatVector)[0]);
			}

			component=component->NextSiblingElement();
		}

		//If all the elements(ambient, diffuse, specular) are present
		if(!(a&&d&&s))
		{
			cout<<"        One or more components couldn't be found\n";
			validAppearance = false;			
		}
		
		if (validAppearance){
			appearances.push_back(appearanceObject);
		}
		else printf("This appearance wasn't loaded... Problems were detected... Program will try to run anyway...\n");
		
		printf("-------------------------------\n");
		appearance = appearance->NextSiblingElement("appearance");
	}	


	if (appearances.size() == 0){
		printf("No appearances have been loaded... Program will end...\n");
		return false;
	}

	return true;
}

bool XMLSceneMod::readAnimations(TiXmlElement* dgxElement, vector<Animation*> &animationsVector)
{
	cout<<"\nParsing animations:\n";

	animationElement=dgxElement->FirstChildElement("animations");
	if (!animationElement){
		printf("There are no animations in this file...\n");
		return true;
	}
	printf("-------------------------------\n");
	TiXmlElement * animation = animationElement->FirstChildElement("animation");
	while(animation)
	{
		printf("Uma animacao\n");
		bool validAnimation = true;

		char* id;
		float span;
		char* type;

		 id = (char*) animation->Attribute("id");
		 if (!id){
			 printf("	Problem with animation id...\n");
			 validAnimation = false;
		 }
		 else printf("	Id: %s\n",id);
		 

		 char* span_s=(char*) animation->Attribute("span");
		 if (!span_s || !sscanf(span_s,"%f",&span)){
			 validAnimation = false;
			 printf("	Problem with animation span value...\n");
		 }
		 else printf("        Span: %s\n",span_s);


		  type = (char*) animation->Attribute("type");
		 if (!id){
			 printf("	Problem with animation type...\n");
			 validAnimation = false;
		 }
		 else printf("        Type: %s\n",type);

		 if(strcmp(type,"circular")==0)
		 {
			 float x,y,z,radius,startang,rotang;

			 char* center = (char*) animation->Attribute("center");
				if (!center || !readXYZcoord(center,x,y,z)){
					validAnimation=false;
					printf("	Problem with circular animation center coordinates...\n");}
				else printf("        -Center: %s\n",center);


			 char* radius_s = (char*) animation->Attribute("radius");
			 if (!radius_s || !sscanf(radius_s,"%f",&radius)){
				 validAnimation = false;
				 printf("	Problem with circular animation radius value...\n");
			 }
			 else printf("        -Radius: %s\n",radius_s);

			 char* startang_s = (char*) animation->Attribute("startang");
			 if (!startang_s || !sscanf(startang_s,"%f",&startang)){
				 validAnimation = false;
				 printf("	Problem with animation startang value...\n");
			 }
			 else printf("        -Startang: %s\n",startang_s);

			  char* rotang_s = (char*) animation->Attribute("rotang");
			 if (!rotang_s || !sscanf(rotang_s,"%f",&rotang)){
				 validAnimation = false;
				 printf("	Problem with animation rotang value...\n");
			 }
			 else printf("        -Rotang: %s\n",rotang_s);

			 Animation* ani= new CircularAnimation(id,span,radius,startang,rotang);
				ani->show();
				animationsVector.push_back(ani);
		 }

		  if(strcmp(type,"linear")==0)
		 {
				vector< vector<float>> pontos;
				TiXmlElement * controlpoint = animation->FirstChildElement("controlpoint");
				while(controlpoint)
				{
					vector<float> ponto;
					float x,y,z;

					 char* x_s = (char*) controlpoint->Attribute("xx");
					 if (!x_s || !sscanf(x_s,"%f",&x)){
						 validAnimation = false;
						 printf("	Problem with linear animation controlopoint on value x \n");
					 }

					  char* y_s = (char*) controlpoint->Attribute("yy");
					 if (!y_s || !sscanf(y_s,"%f",&y)){
						 validAnimation = false;
						 printf("	Problem with linear animation controlopoint on value y \n");
					 }

					  char* z_s = (char*) controlpoint->Attribute("zz");
					 if (!z_s || !sscanf(z_s,"%f",&z)){
						 validAnimation = false;
						 printf("	Problem with linear animation controlopoint on value z \n");
					 }
					ponto.push_back(x);
					ponto.push_back(y);
					ponto.push_back(z);

					pontos.push_back(ponto);


					controlpoint=controlpoint->NextSiblingElement("controlpoint");
				}
				Animation* ani= new LinearAnimation(string(id),span,pontos);
				ani->show();
				animationsVector.push_back(ani);
		  }

		printf("-------------------------------\n");

		

		 animation=animation->NextSiblingElement("animation");
	}
	cout<<endl;
	
	return true;
}


bool XMLSceneMod::readGraph(TiXmlElement* dgxElement, std::vector<Appearance* > &appearances, std::vector<Animation*> &animations,vector<FlagShader*> &flagShaders){
	graphElement = dgxElement->FirstChildElement( "graph" );
	char* rootNodeId = "";
	if (graphElement == NULL){
		printf("Graph block not found!\n");
		return false;
	}
	else
	{

		printf("\nProcessing Graph:\n");

		rootNodeId = (char*) graphElement->Attribute("rootid");
		
		char *prefix="  -";
		
		TiXmlElement *node = graphElement->FirstChildElement("node");
		

		vector<vector<char* > > descendentes;
		while (node)
		{
			
			printf("-Node id: %s\n",node->Attribute("id"));
			char* charString=(char *)node->Attribute("id");

			string s=string(charString);
			Node* n = new Node(s);

			char* displayList_C = (char*) node->Attribute("displaylist");
			if (displayList_C){
				if (strcmp(displayList_C, "true") == 0)
					n->setDisplayList(true);
				else if (strcmp(displayList_C, "false") != 0)
					printf("	Invalid display list value. Program will assume \"false\" value...\n");
			}else{
				printf("	Missing display list value. Program will assume \"false\" value...\n");
			}
			printf("	DisplayList: %s\n", n->getDisplayList()? "true" : "false");

			TiXmlElement *transforms  = node->FirstChildElement();
			if (!transforms){
				printf("	Error: Transforms block not found!\n");
				return false;
			}
			printf("	Transforms:\n");
			TiXmlElement *transform = transforms->FirstChildElement("transform");  
			
			//inicia a matriz com a matriz identidade
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			while (transform)
			{

				char* tipo = (char*) transform->Attribute("type");
							
				if (strcmp(tipo,"translate") == 0){
					char* to = (char*) transform->Attribute("to");
					printf("	-Type: %s -> %s\n", tipo, to);

					float x,y,z;
					if (!to || !readXYZcoord(to,x,y,z))
						printf("	Unexpected problem with translation... Program will try to run anyway...\n");
					else glTranslatef(x,y,z);
				} 
				else if (strcmp(tipo,"rotate") == 0){					
					char* axis = (char*) transform->Attribute("axis");
					char* angle = (char*) transform->Attribute("angle");
					printf("	-Type: %s -> axis: %s, angle: %s\n", tipo, axis, angle);


					float angulo;
					if (sscanf(angle,"%f",&angulo) != 1)
						printf("	Problem with angle value in rotate transform... Program will try to run anyway...\n");
					else{
						if(strcmp(axis,"x") == 0)
							glRotatef(angulo, 1,0,0);
						else if(strcmp(axis,"y") == 0)
							glRotatef(angulo, 0,1,0);
						else if(strcmp(axis,"z") == 0)
							glRotatef(angulo, 0,0,1);
						else printf("	Unexpected problem with rotation... Program will try to run anyway...\n");
					}
				} 

				else if (strcmp(tipo,"scale") == 0){
					char* factor = (char*) transform->Attribute("factor");
					printf("	-Type: %s -> %s\n", tipo, factor);

					float x,y,z;
					if (!factor || !readXYZcoord(factor,x,y,z))
						printf("	Invalid scale transform... Program will try to run anyway...\n");
					else glScalef(x,y,z);

				} 
				else printf("	Missing/invalid transform.. Program will try to run anyway...\n");
				
				transform = transform->NextSiblingElement();
			}


			//por a matriz final em m e depois atribuir m � matriz transforma��o do n�
			float m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX,m);
			n->setMatrix(m);
			


			TiXmlElement *appearanceref  = node->FirstChildElement("appearanceref");
			if (!appearanceref){
				printf("	Error: Appearances block not found!\n");
				return false;
			}
			
			char* appearance = (char*) appearanceref->Attribute("id"); 
			bool exists=false;
			if(strcmp(appearance,"inherit")==0){
				n->setAparencia(NULL);
				exists=true;
			}
			else{

				for(unsigned int i=0;i<appearances.size();i++)
					if(strcmp(appearances[i]->getId(),appearance)==0){
						exists=true;
						n->setAparencia(appearances[i]);
						break;
					}
			}

			if(exists) printf("	Appearance: %s\n", appearance);
			else {cout<<"	Error parsing node: appearance does not exist. Program will end...\n"; return false;}
			
			
			TiXmlElement *animationref  = node->FirstChildElement("animationref");
			
			bool exists2=false;
			vector<Animation *> animationsVector;
			//percorre todas as animationref
			while (animationref)
			{
			string animation_S = string(animationref->Attribute("id")); 
			cout<<"        Animationref:"<<animation_S<<endl;
			
				//tenta encontrar no vector de animations
				for(unsigned int i=0;i<animations.size();i++)
				{
					cout<<animations[i]->getId()<<endl;
					//se encontrar adiciona ao vector de animacoes do no
					if(animations[i]->getId()==animation_S){
						exists2=true;
						animationsVector.push_back(animations[i]);
						cout<<"animation:"<<animations[i]->getId()<<endl;
						break;
					}
				}
				//se nao, nao adiciona
				if(!exists2)
				{
					cout<<"        Nao foi encontrada a Animacao"<<endl;
					
				}
				animationref=animationref->NextSiblingElement("animationref");
			}

			n->setAnimation(animationsVector);
			
			TiXmlElement *primitives  = node->FirstChildElement("primitives");
			if (primitives){
				printf("	Primitives:\n");
				TiXmlElement *primitive = primitives->FirstChildElement(); 
				while (primitive)
				{

					//////////////rectangle//////////////
					if (strcmp("rectangle", primitive->Value()) == 0){
						char* xy1 = (char*) primitive->Attribute("xy1");
						char* xy2 = (char*) primitive->Attribute("xy2");
						float x1, y1, x2, y2;
						if (readXYcoord(xy1, x1, y1) && readXYcoord(xy2, x2, y2)){
							printf("		rectangle: xy1-%.2f %.2f, xy2-%.2f %.2f\n", x1, y1, x2, y2);
							Rectangle* rect = new Rectangle(x1,y1,x2,y2);
							n->addPrimitiva(rect);
						}
						else printf("		rectangle: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						
					}


					////////////triangle///////////
					else if (strcmp("triangle", primitive->Value()) == 0){
						char* xyz1 = (char*) primitive->Attribute("xyz1");
						char* xyz2 = (char*) primitive->Attribute("xyz2");
						char* xyz3 = (char*) primitive->Attribute("xyz3");
						float x1,x2,x3,y1,y2,y3,z1,z2,z3;
						if (readXYZcoord(xyz1, x1, y1, z1) && readXYZcoord(xyz2, x2, y2, z2)
							&& readXYZcoord(xyz3, x3, y3, z3))
						{
							printf("		triangle:\n");
							printf("			xyz1-%.2f %.2f %.2f,\n", x1, y1, z1);
							printf("			xyz2-%.2f %.2f %.2f,\n", x2, y2, z2);
							printf("			xyz3-%.2f %.2f %.2f\n", x3, y3, z3);
							Triangle* tri = new Triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
							n->addPrimitiva(tri);
						}
						else printf("		triangle: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						
					}


					////////////cylinder//////////////
					else if (strcmp("cylinder", primitive->Value()) == 0){
						float base = atof((char*) primitive->Attribute("base"));
						//float top = atof((char*) primitive->Attribute("top"));
						float height = atof((char*) primitive->Attribute("height"));
						
						//topo � algumas vezes igual a zero, pelo que atof n�o pode ser utilizado
						char* endptr;
						char* top_S = (char*) primitive->Attribute("top");
						errno = 0;    /* To distinguish success/failure after call */
						double top = strtod(top_S, &endptr);
						
						/* Check for various possible errors */
						if ((errno == ERANGE && (top == HUGE_VAL || top == -HUGE_VAL))) {
								printf("		cylinder: error with top value. Program will try to run anyway without this primitive\n");
						}

						else{
							int slices = atoi((char*) primitive->Attribute("slices"));
							int stacks = atoi((char*) primitive->Attribute("stacks"));
							printf("		cylinder: base-%.2f, top-%.2f, height-%.2f, slices-%d, stacks-%d\n",
								base, top, height, slices, stacks);
							if (base == 0 || height == 0 || slices == 0 || stacks == 0)
								printf("		cylinder: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
							else{
								Cylinder* cil = new Cylinder(base,top,height,slices,stacks);
								n->addPrimitiva(cil);
							}
						}
					}


					/////////////sphere////////////////
					else if (strcmp("sphere", primitive->Value()) == 0){
						float radius = atof((char*) primitive->Attribute("radius"));
						int slices = atoi((char*) primitive->Attribute("slices"));
						int stacks = atoi((char*) primitive->Attribute("stacks"));
						printf("		sphere: radius-%.2f, slices-%d, stacks-%d\n", radius, slices, stacks);
						if (radius == 0 || slices == 0 || stacks == 0)
							printf("		sphere: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						else{
							Sphere* esf = new Sphere(radius,slices,stacks);
							n->addPrimitiva(esf);
						}
					}


					///////////////torus///////////////
					else if (strcmp("torus", primitive->Value()) == 0){
						float inner = atof ((char*) primitive->Attribute("inner"));
						float outer = atof((char*) primitive->Attribute("outer"));
						int slices = atoi((char*) primitive->Attribute("slices"));
						int loops = atoi((char*) primitive->Attribute("loops"));
						printf("		torus: inner-%.2f, outer-%.2f, slices-%d, loops-%d\n",
							inner, outer, slices, loops);
						if (inner == 0 || outer == 0 || slices == 0 || loops == 0)
							printf("		torus: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						else{
							Torus* tor = new Torus(inner, outer , slices, loops);
							n->addPrimitiva(tor);
						}
					}
					///////////////plane///////////////
					else if (strcmp("plane", primitive->Value()) == 0){
						int parts = atoi((char*) primitive->Attribute("parts"));
						printf("		plane: parts-%d\n");
						if (parts == 0)
							printf("		plane: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						else{
							Plane* pl = new Plane(parts);
							n->addPrimitiva(pl);
						}
					}
					///////////////flag///////////////
					else if (strcmp("flag", primitive->Value()) == 0){
						char* tex = (char*) primitive->Attribute("texture");
						printf("		plane: texture-%s\n", tex);
						if (!tex)
							printf("		flag: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						else{
							FlagShader* fl = new FlagShader(tex);
							flagShaders.push_back(fl);
							n->addPrimitiva(fl);
						}
					}

					//////////////patch//////////////
					else if(strcmp("patch", primitive->Value()) == 0){
						bool patchBool=true;
						cout<<"Patch:"<<endl;
						int order;
						int partsU;
						int partsV;
						string compute;

						char* order_s = (char*) primitive->Attribute("order");
						if (sscanf(order_s,"%i",&order) != 1)
						{
							cout<<"Error reading order value"<<endl;
							patchBool=false;
						}
						else cout<<"		order:"<<order<<endl;

						char* partsU_s = (char*) primitive->Attribute("partsU");
						if (sscanf(partsU_s,"%i",&partsU) != 1)
						{
							cout<<"Error reading partsU value"<<endl;
							patchBool=false;
						}
						else cout<<"		partsU:"<<partsU_s<<endl;

						char* partsV_s = (char*) primitive->Attribute("partsV");
						if (sscanf(partsV_s,"%i",&partsV) != 1)
						{
							cout<<"Error reading partsV value"<<endl;
							patchBool=false;
						}
						else cout<<"		partsV:"<<partsV_s<<endl;

						compute = string(primitive->Attribute("compute"));

						if(compute=="fill" || compute=="line" || compute=="point")
						{
							//cout<<"Error reading compute value"<<endl;
							cout<<"		compute:"<<compute<<endl;
						}
						else{
							patchBool=false;
							cout<<"Error reading partsV value"<<compute<<endl;

						}

						

						//ler os pontos

						TiXmlElement *controlpoint  = primitive->FirstChildElement("controlpoint");

						int counter=0;
						vector< vector<float>> cp;
						while(controlpoint)
						{
							
							cout<<"		controlPoint: ";
							counter++;
							float x,y,z;
							x=y=z=0;
							char* x_s = (char*) controlpoint->Attribute("x");
							if (sscanf(x_s,"%f",&x) != 1)
							{
								cout<<"Error reading x value"<<endl;
									patchBool=false;
							}
							else cout<<"  x:"<<x;

							char* y_s = (char*) controlpoint->Attribute("y");
							if (sscanf(y_s,"%f",&y) != 1)
							{
								cout<<endl<<"Error reading y value"<<endl;
									patchBool=false;
							}
							else cout<<"  y:"<<y;


							char* z_s = (char*) controlpoint->Attribute("z");
							if (sscanf(z_s,"%f",&z) != 1)
							{
								cout<<endl<<"Error reading z value"<<endl;
									patchBool=false;
							}
							else cout<<"  z:"<<z<<endl;
							vector<float> point;
							point.push_back(x);
							point.push_back(y);
							point.push_back(z);
							cp.push_back(point);
							counter++;
							controlpoint=controlpoint->NextSiblingElement();
						}

						
						/*for(int i =0;i<cp.size();i++)
							cout<<"x:"<<cp[i][0]<<" y:"<<cp[i][1]<<" z:"<<cp[i][2]<<endl;
						cin.get();*/


						if(patchBool)
						{
							Patch * p=new Patch(order,partsU,partsV,compute,cp);
							n->addPrimitiva(p);
						n->addPrimitiva(p);
						}
						else
							cout<<"Ocorreu um erro a ler o Patch"<<endl;
						cin.get();


					}

					else if (strcmp("vehicle", primitive->Value()) == 0){
						Vehicle* v = new Vehicle();
						n->addPrimitiva(v);
					}

					else printf("		Invalid primitive detected.  Program will try to run anyway...\n");



					primitive = primitive->NextSiblingElement();
				}
				
			}

			TiXmlElement *descendants  = node->FirstChildElement("descendants");
			if (!descendants){
				printf("	No descendants block\n");
				vector<char*> descendentesNo;
				descendentes.push_back(descendentesNo);
			}
			else {
				TiXmlElement *noderef = descendants->FirstChildElement("noderef");  
				vector<char*> descendentesNo;
				string idDesc;
				cout<<"	Descendants: ";
				while(noderef)
					{
						descendentesNo.push_back((char*) noderef->Attribute("id"));
						noderef=noderef->NextSiblingElement();
					}
				descendentes.push_back(descendentesNo);
			}
			cout<<"\n------------------------------\n";
			destinationGraph->addNode(n);
			node = node->NextSiblingElement();
			//cin.get();
		}

		//ao sair do while, � preciso verificar se existe pelo menos um n� (pelo menos o root node tem de existir)
		Node* rootTemp = destinationGraph->searchForNode((string)rootNodeId);
		if (rootTemp == NULL)
			return false;
		destinationGraph->setRoot(rootTemp);

		for (unsigned int i = 0; i < descendentes.size(); i++){
			Node* tempNode = destinationGraph->searchForNode(i);
			for (unsigned int j = 0; j < descendentes[i].size(); j++){
				Node* descendenteTemp = destinationGraph->searchForNode( (string) descendentes[i][j] );
				if (descendenteTemp != NULL){
					tempNode->addDescendente(descendenteTemp);
					descendenteTemp->addAscendente(tempNode);
				}
				else printf("Node: %s -> descendant: %s, doesn't exist. Program will try to run anyway...\n", tempNode->getId(), descendentes[i][j]);
			}
		}

	//	XMLSceneMod::processDisplayLists(destinationGraph->getRoot());
	}

	return true;

}
/*
void XMLSceneMod::processDisplayLists(Node* n){
	int numFilhos = n->getDescendentes().size();
	for (int i = 0; i < numFilhos; i++)
		XMLSceneMod::processDisplayLists(n->getDescendenteIndex(i));

	if (n->getDisplayList()){
		int id = glGenLists(1);
		n->setDisplayListID(id);
		glNewList(id, GL_COMPILE);
			XMLSceneMod::processDisplayListsAux(n);
		glEndList();
	}
}


void XMLSceneMod::processDisplayListsAux(Node* n){
	glPushMatrix();
		glMultMatrixf(n->getMatrix());
		
		if (n->getAppearance() != NULL){
			XMLSceneMod::appearancesStack.push(n->getAppearance());			
		}
		
		if (!XMLSceneMod::appearancesStack.empty()){
			Appearance* appearanceTemp = XMLSceneMod::appearancesStack.top();
			appearanceTemp->apply();		
			if (appearanceTemp->getTexture() != NULL){
					Appearance::texlength_s = appearanceTemp->getTexture()->getTexlengths();
					Appearance::texlength_t = appearanceTemp->getTexture()->getTexlengtht();
			}
		}
		for (unsigned int j = 0; j < n->getNumeroDePrimitivas(); j++){
			n->getPrimitiva(j)->draw();
		}


		for (unsigned int i = 0; i < n->getDescendentes().size(); i++)
			processDisplayListsAux(n->getDescendenteIndex(i));

		if (n->getAppearance() != NULL)
			XMLSceneMod::appearancesStack.pop();


	glPopMatrix();
}

*/

//-------------------------------------------------------

TiXmlElement *XMLSceneMod::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}


