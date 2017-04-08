#include "Lab.h"


/********************** GLOBALS ******************************/

std::vector<LightSource*> lights;

const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
const GLint FRAME_INTERVAL = 1000 / FRAMES_PER_SECOND; // Interval in milliseconds between frames

// Frame buffer that contains both the color and depth buffers
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Objects in the scene

// Pyramid
std::vector<VertexData> redPyramidVertices, yellowPyramidVertices, pinkPyramidVertices, blue1, blue2, whitePy;

// Reference plane
std::vector<VertexData> refPlaneVertices;

// Global to hold the rotation angle of objects in the scene
float angle = glm::radians(45.0f);

/********************** END GLOBALS ******************************/

void renderObjects()
{
	static float angle = 0.0f;
	static float angle1 = 0.0f;
	angle -= 1.0f;
	angle1 += 6.0f;
	// Set Modeling transformation for the reference plane
	PerVertex::modelingTransformation = glm::translate(glm::vec3(0.0f, -3.0f, 0.0f));

	// Send the reference plane vertices down the pipeline
	PerVertex::processTriangleVertices(refPlaneVertices);

	// Set modeling transformation for the center pyramid
	PerVertex::modelingTransformation = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	// Send the pyramid vertices down the pipeline
	PerVertex::processTriangleVertices(redPyramidVertices);

	// Set modeling transformation for the center pyramid
	PerVertex::modelingTransformation = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

	// Send the pyramid vertices down the pipeline
	PerVertex::processTriangleVertices(yellowPyramidVertices);

	// Set modeling transformation for the center pyramid
	PerVertex::modelingTransformation = glm::translate(glm::vec3(-3.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

	// Send the pyramid vertices down the pipeline
	PerVertex::processTriangleVertices(pinkPyramidVertices);

	// Set modeling transformation for the center pyramid
	PerVertex::modelingTransformation = glm::translate(glm::vec3(-3.5f, -2.5f, 3.5f));
	PerVertex::processTriangleVertices(blue1);

	PerVertex::modelingTransformation = glm::translate(glm::vec3(3.5f, -2.5f, -3.5f));
	PerVertex::processTriangleVertices(blue2);

	PerVertex::modelingTransformation = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)) 
				* glm::translate(glm::vec3(10.0f, 3.0f, 0.0f)) * glm::rotate(glm::radians(angle1), glm::vec3(0.0f, 0.0f, 1.0f));
	PerVertex::processTriangleVertices(whitePy);

} // end renderObjects


/**
* Acts as the display function for the window.
*/
static void RenderSceneCB()
{
	// time in milliseconds of last frame render
	static GLint lastRenderTime = 0;

	int currentTime = glutGet(GLUT_ELAPSED_TIME); // Get current time
	int elapsedTime = currentTime - lastRenderTime; // Calc time since last frame

	// Check if enough time has elapsed since the last render.
	if (elapsedTime >= FRAME_INTERVAL) {

		// Save time for this frame render
		lastRenderTime = currentTime;

		// Clear the color buffer
		frameBuffer.clearColorAndDepthBuffers();

		angle += glm::radians(1.0f);

		// Determine the position of the viewpoint in world coordinates
		PerVertex::eyePositionInWorldCoords = glm::inverse(PerVertex::viewingTransformation)[3].xyz;

		// Draw the objects in the scene
		renderObjects();

		// Display the color buffer
		frameBuffer.showColorBuffer();
	}

} // end RenderSceneCB

// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize(width, height); 

	// Set rendering window parameters for viewport transfomation
	PerVertex::xViewportMin = 0;
	PerVertex::yViewportMin = 0;
	PerVertex::xViewportMax = (float)width;
	PerVertex::yViewportMax = (float)height;

	// Create a perspective projection matrix. glm::perspective vertical field of view is specifed in degrees.
	PerVertex::projectionTransformation = 
		glm::perspective(45.0f, ((float)PerVertex::xViewportMax - PerVertex::xViewportMin) / 
								((float)PerVertex::yViewportMax - PerVertex::yViewportMin), 1.0f, 50.0f);

	// Set viewport transformation
	PerVertex::viewportTransformation =
		glm::translate(glm::vec3(PerVertex::xViewportMin, PerVertex::yViewportMin, 0.0f)) *
		glm::scale(glm::vec3((float)(PerVertex::xViewportMax - PerVertex::xViewportMin) / 
			(PerVertex::xNdcMax - PerVertex::xNdcMin), 
			(float)(PerVertex::yViewportMax - PerVertex::yViewportMin) / 
			(PerVertex::yNdcMax - PerVertex::yNdcMin), 1.0f)) *
		glm::translate(glm::vec3(-PerVertex::xNdcMin, -PerVertex::yNdcMin, 0.0f));

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch (key) {

	case('f') : case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27) : // Escape key
		glutLeaveMainLoop();
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}


	glutPostRedisplay();

} // end KeyboardCB


// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	static const float rotateInc = glm::radians(20.0f);

	switch (key) {

	case(GLUT_KEY_RIGHT) :

		break;
	case(GLUT_KEY_LEFT) :

		break;
	case(GLUT_KEY_UP) :

		break;
	case(GLUT_KEY_DOWN) :

		break;

	default:
		std::cout << key << " key pressed." << std::endl;
	}


	glutPostRedisplay();

} // end SpecialKeysCB


// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()
{
	glutPostRedisplay();

} // end animate

std::vector<VertexData> makePyramidVertices(color pyramidColor, float width = 1.0f, float height = 1.0f)
{
	std::vector<VertexData> pyramidVertices;

	// Set vertex locations for a pyramid

	// Positive Z face
	pyramidVertices.push_back(VertexData(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));

	pyramidVertices.push_back(VertexData(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f), pyramidColor));

	pyramidVertices.push_back(VertexData(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f), pyramidColor));

	pyramidVertices.push_back(VertexData(glm::vec4(0.0f, height / 2.0f, 0.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));

	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f), pyramidColor));

	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(width / 2.0f, -height / 2.0f, width / 2.0f, 1.0f), pyramidColor));
	pyramidVertices.push_back(VertexData(glm::vec4(-width / 2.0f, -height / 2.0f, -width / 2.0f, 1.0f), pyramidColor));

	return pyramidVertices;

} // end makePyramidVertices


// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

	// initialize random seed
	srand((unsigned int)time(NULL));

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("CSE 287 Lab 8 Modeling Tranformations");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(animate);

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	color clearColor(0.784, 0.784, 1.0, 1.0);

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(clearColor);

	// ************* Object vertex coordinate initialization ***********************

	color red(1.0f, 0.0f, 0.0f, 1.0f);
	color green(0.0f, 1.0f, 0.0f, 1.0f);
	color yellow(1.0f, 1.0f, 0.0f, 1.0f);
	color pink(1.0f, 0.0f, 1.0f, 1.0f);
	color blue(0.0f, 0.0f, 1.0f, 1.0f);
	color white(1.0f, 1.0f, 1.0f, 1.0f);

	// Make a red pyramid
	redPyramidVertices = makePyramidVertices(red);
	// Make some other pyramid
	yellowPyramidVertices = makePyramidVertices(yellow);
	pinkPyramidVertices = makePyramidVertices(pink, 2.0f, 2.0f);
	blue1 = makePyramidVertices(blue);
	blue2 = makePyramidVertices(blue);
	whitePy = makePyramidVertices(white);

	// Set vertex locations for a plane that is composed o four triangles.
	float planeWidth = 8.0f;

	// Red Triangles
	refPlaneVertices.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), red));
	refPlaneVertices.push_back(VertexData(glm::vec4(-planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f), red));
	refPlaneVertices.push_back(VertexData(glm::vec4(-planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f), red));
	refPlaneVertices.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), red));
	refPlaneVertices.push_back(VertexData(glm::vec4(planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f), red));
	refPlaneVertices.push_back(VertexData(glm::vec4(planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f), red));
	// Green Triangles
	refPlaneVertices.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), green));
	refPlaneVertices.push_back(VertexData(glm::vec4(planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f), green));
	refPlaneVertices.push_back(VertexData(glm::vec4(-planeWidth / 2.0f, 0.0f, -planeWidth / 2.0f, 1.0f), green));
	refPlaneVertices.push_back(VertexData(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), green));
	refPlaneVertices.push_back(VertexData(glm::vec4(-planeWidth / 2.0f, 0.0f, planeWidth / 2.0, 1.0f), green));
	refPlaneVertices.push_back(VertexData(glm::vec4(planeWidth / 2.0f, 0.0f, planeWidth / 2.0f, 1.0f), green));

	// Set the initial viewing tranformation for the scene
	PerVertex::viewingTransformation = glm::translate(glm::vec3(0.0f, 0.0f, -12.0));

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	return 0;

} // end main


