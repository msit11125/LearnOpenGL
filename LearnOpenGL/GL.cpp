#include "GL.h"

// ���ӷ|�q.obj�ɨ��o
float vertices[] = {
	 0.5f,  0.5f, 0.0f,  //  0 top right
	 0.5f, -0.5f, 0.0f,  //  1 bottom right
	-0.5f, -0.5f, 0.0f,  //  2 bottom left
	-0.5f,  0.5f, 0.0f   //  3 top left 
};


/*
	�ί��ި̧ǵe�X��ӤT���� = �����
	�ݨϥΨ�EBO
*/
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

/*
	location=0 �i��O��vao position(0~15)
	glVertexAttribPointer()
	glEnableVertexAttribArray()
	�]�n�令�ۦP��position��m
*/
const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n" // 1.��X�ܶq(�C��)
"void main()\n"
"{ 	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"  	vertexColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n"; // �¶q��m0, �V�q��m1


const char* fragmentShaderSource =
"#version 330 core\n"
"in vec4 vertexColor;\n" // 2.�����ܶq(�C��)
"uniform vec4 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{ 	FragColor = ourColor; }\n";


GL::GL() {
	
}

GL::~GL() {

}

int GL::Draw() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// �� mac�~�n�ŧi�U���o��
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	glViewport(0, 0, 800, 600);
	// �� ���簣 
	// https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/04%20Face%20culling/
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	// �� �u�e�ؽu
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//�� Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // ����1��VAO
	//unsigned int VAO[10];
	//glGenVertexArrays(10, VAO); // ����10��VAO
	glBindVertexArray(VAO);

	//�� Vertex Buffer Object ���I�w�Ī���
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�� Element Buffer Objects
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// �}�l��m, �X�ӳ��I, ���A, �O�_�ݭn�ഫ���쥿�t1����, ���I�Ʋռƶq*�j�p, �����q
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

#pragma region ����
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// �M���C��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// �[�Juniform�Ѽ�
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		glUseProgram(shaderProgram); // �}��shader
		// �����C��
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// �� �����eVAO
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}