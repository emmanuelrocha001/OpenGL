#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	unsigned int id = glCreateShader(type);
	//=&source[0];
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO: Error handling //iv is types
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//alloca() allows to allocate on stack dynamically 
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile "  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		//delete shader, since compilation failed
		glDeleteShader(id);
		return 0;
	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//create program
	unsigned int program = glCreateProgram();
	//create shadder objects
	//vertex shadder
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//fragment shadder
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//attach shaders to program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//link to program
	glLinkProgram(program);
	//validate program
	glValidateProgram(program);

	//delete shaders, since they have been linked to program
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	//fullscreen
	//GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	//glfwGetMonitorSize(primaryMonitor, NULL, NULL);

	window = glfwCreateWindow(640, 480, "Flappy Bird", NULL, NULL);//, primaryMonitor, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//GLEW
	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!\n";

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	float positions[6] = {
		-.5f, -.5f,
		0.0f, .5f,
		0.5f, -.5f };

	//default OpenGL projection
	//(left,right,bottom, top,-z,+z)
	//(-1,1,-1,1,-1,1)
	//Vertex Buffer
	unsigned int buffer;
	//define vertex buffer
	glGenBuffers(1, &buffer);
	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//give data(in bytes)
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
	//enable vertex attribute
	glEnableVertexAttribArray(0);
	//specify layout of attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//Shader creation 
	//compalibility, use latest versions if new features are needed 

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	std::cout << "VERTEX" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "FRAGMENT" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	//create shaders
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	//use shader to renders
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT);

		//draw current bound buffer
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

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
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}