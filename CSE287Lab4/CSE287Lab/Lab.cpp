#include "Lab.h"
#include <time.h> 

//******** Global Variables ***********

// Frame buffer for holding pixel color and depth values
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Ray tracer that generates the rendered image
RayTracer rayTrace(frameBuffer);

// Surfaces or object in the scene to be rendered
std::vector<std::shared_ptr<Surface>> surfaces;

// Light sources in the scene
std::vector<std::shared_ptr<LightSource>> lights;

//***********************************

/**
* Acts as the display function for the window. 
*/
static void RenderSceneCB()
{
	// Get time before rendering the scene
	int frameStartTime = glutGet( GLUT_ELAPSED_TIME ); 

	// Clear the color buffer
//	frameBuffer.clearColorAndDepthBuffers( ); // Not necessary for ray tracing

	// Ray trace the scene to determine the color of all the pixels in the scene
	rayTrace.raytraceScene( surfaces, lights);

	// Display the color buffer
	frameBuffer.showColorBuffer();

	int frameEndTime = glutGet( GLUT_ELAPSED_TIME ); // Get end time

	// Calculate and display time to render the scene
	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;
	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;

} // end RenderSceneCB


// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize( width, height );

	rayTrace.setCameraFrame(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

	rayTrace.calculatePerspectiveViewingParameters(45.0f);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch(key) {

	case('f'): case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case('d'): case('D'): // 'd' key to switch to day mode
		for (std::shared_ptr<LightSource> l : lights) {
			l->lightColor = color(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	case('n'): case('N'): // 'n' key to switch to night mode
		for (std::shared_ptr<LightSource> l : lights) {
			l->lightColor = color(0.3f, 0.3f, 0.3f, 1.0f);
		}
		break;
	case('a'): case('A'):
		rayTrace.setCameraFrame(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		break;
	case('b'): case('B'):
		rayTrace.setCameraFrame(glm::vec3(0, 0, -20), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		break;
	case(27): // Escape key
		glutLeaveMainLoop();
		break;
	case('0'):
		rayTrace.recursionDepth = 0;
		break;
	case('1'):
		rayTrace.recursionDepth = 1;
		break;
	case('2'):
		rayTrace.recursionDepth = 2;
		break;
	case('3'):
		rayTrace.recursionDepth = 3;
		break;
	case('4'):
		rayTrace.recursionDepth = 4;
		break;
	case('5'):
		rayTrace.recursionDepth = 5;
		break;
	case('6'):
		rayTrace.recursionDepth = 6;
		break;
	case('7'):
		rayTrace.recursionDepth = 7;
		break;
	case('8'):
		rayTrace.recursionDepth = 8;
		break;
	case('9'):
		rayTrace.recursionDepth = 9;
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end KeyboardCB


// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y)
{
	switch(key) {
	
	case(GLUT_KEY_RIGHT):
		
		break;
	case(GLUT_KEY_LEFT):
		
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end SpecialKeysCB





void buildScene()
{
	// Initialize random seed - used to create random colors
	srand((unsigned int)time(NULL));

	// Create objects in the scene
	std::shared_ptr<Sphere> redBall = std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -10.0f), 0.8f, color(0.8f, 0.3f, 0.3f, 1.0f));
	std::shared_ptr<Sphere> blueBall = std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -8.0f), 0.8f, color(0.1f, 0.1f, 0.8f, 1.0f));
	std::shared_ptr<Plane> plane1 = std::make_shared<Plane>(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), color(0.1f, 0.2f, 0.1f, 1.0f));
	std::shared_ptr<Plane> plane2 = std::make_shared<Plane>(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), color(0.5f, 0.5f, 0.5f, 1.0f));
	std::shared_ptr<CylinderAlongX> cyl = std::make_shared<CylinderAlongX>(glm::vec3(2.0f, 1.0f, -4.0f), 0.6f, 0.6f, color(0.2f, 0.2f, 0.6f, 1.0f));

	// Create a polygon
	std::vector<glm::vec3> polyCorners;
	polyCorners.push_back(glm::vec3(glm::vec3(-1, -0.8, -4)));
	polyCorners.push_back(glm::vec3(glm::vec3(-1, 0.2, -4)));
	polyCorners.push_back(glm::vec3(glm::vec3(-1, 0.2, -2)));
	polyCorners.push_back(glm::vec3(glm::vec3(-1, -0.8, -2)));
	std::shared_ptr<Polygon1> poly = std::make_shared<Polygon1>(polyCorners, color(0.4f,0.1f,0.4f,1.0f));

	// Add objects to vector of objects in the scene
	surfaces.push_back(redBall);
	surfaces.push_back(blueBall);
	surfaces.push_back(plane1);
	surfaces.push_back(plane2);
	surfaces.push_back(poly);
	surfaces.push_back(cyl);

	// Create light sources
	std::shared_ptr<LightSource> ambientLight = std::make_shared<LightSource>(color(0.15, 0.15, 0.15, 1.0));
	std::shared_ptr<PositionalLight> lightPos = std::make_shared<PositionalLight>(glm::vec3(5, 5, 4), glm::vec4(.75, .75, .75, 1));
	std::shared_ptr<DirectionalLight> lightDir = std::make_shared<DirectionalLight>(glm::vec3(0, 0, -1), glm::vec4(.75, .75, .75, 1));

	// Add light sources to vector of light sources
	lights.push_back(lightPos);
	lights.push_back(ambientLight);
	lights.push_back(lightDir);
}


// Register as the "idle" function to have the screen continously
// repainted. Due to software rendering, the frame rate will not
// be fast enough to support motion simulation
static void animate()  
{
	glutPostRedisplay();

} // end animate


int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************
	rayTrace.setDefaultColor(color(0.0f,0.0f,0.0f,1.0f));

    // Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
    glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA );

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Ray Trace");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(color(0.5f,0.5f,1.0f,1.0f));

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);		
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);

	buildScene();

	// Enter the GLUT main loop. Control will not return until the window is closed.
    glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	return 0;

} // end main