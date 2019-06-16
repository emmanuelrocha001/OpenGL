#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//Random RGB
//#----------------------------------------------------------------------
void RandomRGBValues(float RGB[])
{
	//Initialzie random seed
	srand(time(NULL));
	for (int i = 0; i < 3; i++)
	{
		RGB[i] = float((rand() % 10) * .1);
		RGB[i] = ((float)rand() / (RAND_MAX));
		std::cout << RGB[i] << std::endl;
	}
}
//#----------------------------------------------------------------------


//error checking for OpenGL
//#----------------------------------------------------------------------
#define ASSERT(x) if (!(x)) __debugbreak(); //specific to visual studio 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__, __LINE__))


static void GLClearError()
{
	//clears errors
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL_error] (" << error << ")" << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}
//#----------------------------------------------------------------------

//Shaders
//#----------------------------------------------------------------------

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

//c++ implementation, slower
static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			//set mode to vertex
		
			if (line.find("vertex") != std::string::npos)
			{
				//set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//set mode to fragment
				type = ShaderType::FRAGMENT;

			}
				
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}
	
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	//=&source[0];
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	//TODO: Error handling //iv is types
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//alloca() allows to allocate on stack dynamically 
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile "  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		//delete shader, since compilation failed
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//create program
	GLCall(unsigned int program = glCreateProgram());
	//create shadder objects
	//vertex shadder
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//fragment shadder
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//attach shaders to program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	//link to program
	GLCall(glLinkProgram(program));
	//validate program
	GLCall(glValidateProgram(program));

	//delete shaders, since they have been linked to program
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

//#----------------------------------------------------------------------


int main(void)
{
//Window Creation
//#----------------------------------------------------------------------
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	//fullscreen
	//GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	//glfwGetMonitorSize(primaryMonitor, NULL, NULL);

	window = glfwCreateWindow(640, 480, "Graphics Engine", NULL, NULL);//, primaryMonitor, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

//#----------------------------------------------------------------------

//GLEW
//#----------------------------------------------------------------------

	//GLEW
	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!\n";

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	float positions[] = {
		-.5f, -.5f,
		0.5f, -.5f,
		.5f, .5f,
		-.5f, .5f,
		
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};
	//default OpenGL projection
	//(left,right,bottom, top,-z,+z)
	//(-1,1,-1,1,-1,1)
	//Vertex Buffer
	
	//define vertex buffer and send to GPU 

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	//bind buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	//give data(in bytes)
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	//enable vertex attribute
	GLCall(glEnableVertexAttribArray(0));
	//specify layout of attribute
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	


	//create and send index buffer to GPU

	unsigned int ibo;
	//define vertex buffer
	GLCall(glGenBuffers(1, &ibo));
	//bind buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	//give data(in bytes)
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));



	//Shader creation 
	//compalibility, use latest versions if new features are needed 

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	//create shaders
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	//use shader to renders//binds shader to program
	GLCall(glUseProgram(shader));

//Uniforms
//#----------------------------------------------------------------------
	
	//retrieves location of variable u_Color from GPU
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	//sets data in shader
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

//#----------------------------------------------------------------------



	float RGB[3];
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

 
		RandomRGBValues(RGB);

		/* Render here */
		//clear screen
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		//draw current bound buffer
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//using index buffers
		GLCall(glUniform4f(location, RGB[0], RGB[1], RGB[2], 1.0f));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //can put nullptr since buffer is bounded
		
		//Legacy OpenGL
		/*
		glBegin(GL_TRIANGLES);
		glVertex2f(-.5f, -.5f);
		glVertex2f(0.0f, .5f);
		glVertex2f(0.5f, -.5f);
		glEnd();
		*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	//delete shader before program termination
	//UNCOMMENT 
	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}