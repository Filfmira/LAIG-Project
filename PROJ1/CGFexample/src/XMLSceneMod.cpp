#include "XMLSceneMod.h"

using namespace std;
#include <iostream>
#include "glui.h"
#include "glut.h"
#include "GL/glui.h"
#include "GL/glut.h"
#include <stdlib.h>     /* strtol */
#include <cstdlib>
#include "CGFapplication.h"


//como vamos guardar os Appearances? talvez uma especia de map, tabela tipo <id,CGFappearance>
//é preciso por os nos com links entre eles?!?*****

XMLSceneMod::XMLSceneMod(char *filename, Graph* gr, Light** lig, vector<Texture* > &textures, vector<Appearance* > &appearances, vector<Camera >*cameras, Camera* &activeCamera, Global *globals): destinationGraph(gr), destinationLights(lig)
{

	// Read XML from file
	
	//Graph graph = Graph();
	
	
	//this->textures=textures;
	//this->appearances=appearances;
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
	cin.get();


	if (!readLights(dgxElement)){
		cin.get();
		exit(1);
	}
	cin.get();


	if(!readTextures(dgxElement, textures)){
		cin.get();
		exit(1);
	}
	cin.get();


	if (!readAppearances(dgxElement, appearances, textures)){
		cin.get();
		exit(1);
	}
	cin.get();
		

	if (!readGraph(dgxElement, appearances)){
		cin.get();
		exit(1);
	}
	cin.get();

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
	//camerasElement = dgxElement->FirstChildElement( "cameras" );
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

	/////////////globals/////////////////
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
			//if(background && sscanf(background,"%f %f %f %f",&red, &green, &blue, &alpha)== 4)
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

			if (strlen(face) == 0){ //aqui tem de verificar as opções ainda...
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
			cout<<"------------------2\n";
			if (strcmp(doublesidedS,"true") != 0)		doublesided=true;
			else if(strcmp(doublesidedS,"false") != 0)	doublesided=false;
			else
			{
				printf("Error parsing lighting: missing or wrong type doublesided variable\n");
				doublesidedS = "error";
			}
			cout<<"------------------3\n";

			if (strcmp(localS,"true") != 0)			local=true;
			else if(strcmp(localS,"false") != 0)	local=false;
			else{
				printf("Error parsing lighting: missing or wrong type local variable\n");
				localS = "error";
			}
			cout<<"------------------4\n";
			if (strcmp(enabledS,"true") != 0)			enabled=true;
			else if(strcmp(enabledS,"false") != 0)		enabled=false;
			else{
				printf("Error parsing lighting: missing or wrong type local variable\n");
				localS = "error";
			}
			cout<<"------------------5\n";

			printf("	doublesided: %s\n	local: %s\n	enabled: %s\n", doublesidedS, localS, enabledS);
		

			char *ambientS=NULL;
			float red, green, blue, alpha;
			cout<<"------------------6\n";
			ambientS=(char *) lightingElement->Attribute("ambient");
			if(ambient && readRGBcomponents(ambientS,ambient[0], ambient[1], ambient[2], ambient[3]))
			{
				cout<<"------------------7\n";
					printf("	ambient: R-%f, G-%f, B-%f, A-%f\n",  ambient[1], ambient[2], ambient[3]);
					cout<<"------------------8\n";
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
	//globals= new Global(mode,shading,&background[0],face,order,doublesided,local,enabled,&ambient[0]);
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
		//CGFcamera* cgfCamera= new CGFcamera();

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

	//verificar se a câmera indicada como inicial existe
	if (cameras->size() == 0){
		printf("No cameras read... Program will end...\n");
		return false;
	}

	for (unsigned int i = 0; i < cameras->size(); i++){
		if (cameras->at(i).getId() == (string) initial_T){
			initialCamera = &(cameras->at(i));
			//printf("set da initial camera\n");
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

	//while(lights){
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
			/*id = atoi(id_T);
			printf("id: %d \n",id);*/
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
			angle = 360; // ????
			exponent = 1; // ????
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
	
	if (destinationLights[0] != NULL ) //vê se fez load de pelo menos uma luz
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


bool XMLSceneMod::readGraph(TiXmlElement* dgxElement, vector<Appearance* > &appearances){
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
		
		/*if (node)
			rootNodeId = (char*) node->Attribute("id");
		*/

		vector<vector<char* > > descendentes;
		while (node)
		{
			
			printf("-Node id: %s\n",node->Attribute("id"));
			char* charString=(char *)node->Attribute("id");

			string s=string(charString);
			//cout<<s;
			Node* n = new Node(s);
			//cout<<"id do no"+n.getId();

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


			//por a matriz final em m e depois atribuir m à matriz transformação do nó
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
						float top = atof((char*) primitive->Attribute("top"));
						float height = atof((char*) primitive->Attribute("height"));
						int slices = atoi((char*) primitive->Attribute("slices"));
						int stacks = atoi((char*) primitive->Attribute("stacks"));
						printf("		cylinder: base-%.2f, top-%.2f, height-%.2f, slices-%d, stacks-%d\n",
							base, top, height, slices, stacks);
						if (base == 0 || top == 0 || height == 0 || slices == 0 || stacks == 0)
							printf("		cylinder: invalid values or wrong format. Program will try to run anyway without this Primitive...\n");
						else{
							Cylinder* cil = new Cylinder(base,top,height,slices,stacks);
							n->addPrimitiva(cil);
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
					else printf("		Invalid primitive detected.  Program will try to run anyway...\n");

					primitive = primitive->NextSiblingElement();
				}
				
			}

			TiXmlElement *descendants  = node->FirstChildElement("descendants");
			if (!descendants){
				printf("	No descendants block\n");
				//break;
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
			//std::cout << n->mostrarNo();
			node = node->NextSiblingElement();
		}

		//ao sair do while, é preciso verificar se existe pelo menos um nó (pelo menos o root node tem de existir)
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
			}
		}
	}



	//mostrar nos
	/*for(unsigned int i =0;i<destinationGraph->getNumberOfNodes();i++)
		cout<<destinationGraph->searchForNode(i)->mostrarNo();*/

	return true;

}






















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


