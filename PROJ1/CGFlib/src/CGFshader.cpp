#include "CGFshader.h"

#include <string.h>
#include <iostream>
#include <stdlib.h>

#ifndef CGF_NO_SHADERS
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "CGFapplication.h"


using namespace std;

static char* textFileRead(const char *fileName) {
	char* text=NULL;
    
	if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
		if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}


static void validateShader(GLuint shader, const char* file = 0) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	if (length > 0) {
		cerr << "Shader " << shader << " (" << (file?file:"") << ") compile error: " << buffer << endl;
	}
}

static void validateProgram(GLuint program) {
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	memset(buffer, 0, BUFFER_SIZE);
	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
	if (length > 0)
		cerr << "Program " << program << " link error: " << buffer << endl;
    
	glValidateProgram(program);
	GLint status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
		cerr << "Error validating shader " << program << endl;
}


CGFshader::CGFshader(void)
{
}
CGFshader::CGFshader(const char *vsFile, const char *fsFile) 
{
	init(vsFile,fsFile);
}

void CGFshader::init(const char *vsFile, const char *fsFile) {
	shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
	const char* vsText = textFileRead(vsFile);
	const char* fsText = textFileRead(fsFile);	
    
    if (vsText == NULL)
	{
        cerr << "Problem reading vertex shader file: " << vsFile << endl;
        return;
    }
    if (fsText == NULL)
	{
        cerr << "Problem reading fragment shader file: " << fsFile << endl;
        return;
    }
    
	glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_fp, 1, &fsText, 0);
    
	glCompileShader(shader_vp);
	validateShader(shader_vp, vsFile);
	glCompileShader(shader_fp);
	validateShader(shader_fp, fsFile);

	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
	validateProgram(shader_id);

	timeloc = glGetUniformLocation(shader_id, "time");
}

CGFshader::~CGFshader() {
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);
    
	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

unsigned int CGFshader::id() {
	return shader_id;
}

void CGFshader::bind() {
	glUseProgram(shader_id);
}

void CGFshader::unbind() {
	glUseProgram(0);
}

void CGFshader::update() 
{
	update(CGFapplication::getTime());
}
void CGFshader::update(float time) 
{
	if (timeloc!=-1)
		glUniform1f(timeloc, time);
}

#else
#include "CGFapplication.h"


static void validateShader(GLuint shader, const char* file = 0) {}

static void validateProgram(GLuint program) {}


CGFshader::CGFshader(void)
{
	cerr << "CGF compiled without shaders. Shaders disabled." << endl;
}
CGFshader::CGFshader(const char *vsFile, const char *fsFile) 
{
	init(vsFile,fsFile);
}

void CGFshader::init(const char *vsFile, const char *fsFile) {
	cerr << "CGF compiled without shaders. Shaders disabled." << endl;
}

CGFshader::~CGFshader() {
}

unsigned int CGFshader::id() {
	return -1;
}

void CGFshader::bind() {
}

void CGFshader::unbind() {
}

void CGFshader::update() 
{
}
void CGFshader::update(float time) 
{
}



#endif
