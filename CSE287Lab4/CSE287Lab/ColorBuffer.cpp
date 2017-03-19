#include "ColorBuffer.h"

/**
* Constructor. Allocates memory for storing pixel values.
*/
ColorBuffer::ColorBuffer(int width, int height)
{
	setColorBufferSize(width, height);

} // end ColorBuffer constructor


/**
* Deallocates dynamically memory associated with the class.
*/
ColorBuffer::~ColorBuffer(void)
{
	// Free the memory associated with the color buffer
	delete [] colorBuffer;

} // end ColorBuffer destructor


/**
* Sizes the color buffer to match the window size. Deallocates any
* memory that was previsouly allocated.
*/
void ColorBuffer::setColorBufferSize(int width, int height) {

	// Save the dimensions of the window
	window.width = width;
    window.height = height;

	// Set pixel storage modes. 
	// (https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Free the memory previously associated with the color buffer
	delete [] colorBuffer;

	// Allocate the color buffer to match the size of the window
	colorBuffer = new GLubyte[width*BYTES_PER_PIXEL*height];

} // end setColorBufferSize


/**
* Sets the color to which the window will be cleared. Does NOT
* actually clear the window
*/
void ColorBuffer::setClearColor( const color & clear ) {

	color clampedColor = clamp(clear, 0.0f, 1.0f);

	clearColor[0] = (GLubyte)(clampedColor.r * 255.0f);
	clearColor[1] = (GLubyte)(clampedColor.g * 255.0f);
	clearColor[2] = (GLubyte)(clampedColor.b * 255.0f);
	clearColor[3] = (GLubyte)(clampedColor.a * 255.0f);

} // end setClearColor


/**
* Clears the window to the clear color.
*/
void ColorBuffer::clearColorBuffer( ) {

	for(int y = 0; y < window.height ; ++y) {
		for(int x = 0; x < window.width; ++x) {
			std::memcpy( colorBuffer + BYTES_PER_PIXEL * (x + y * window.width ), 
						 clearColor,  BYTES_PER_PIXEL );
		
		}
	}

} // end clearColorBuffer


/**
* Copies memory into frame buffer and updates the window
* using an OpenGL command.
*/
void ColorBuffer::showColorBuffer()
{
	// Insure raster position is lower left hand corner of the window.
	glRasterPos2d(-1, -1);

	// Copy color buffer to raster (OpenGL command)
	glDrawPixels( window.width, window.height, 
				  GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer );
	
	// Flush all drawing commands and swapbuffers
	glutSwapBuffers();

} // end showColorBuffer


/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void ColorBuffer::setPixel(float x, float y, const GLubyte rgba[]) {

	setPixel((int)(x + 0.5f), (int)(y + 0.5f), rgba);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void ColorBuffer::setPixel(int x, int y, const color & rgba) 
{
	color clampedColor = clamp(rgba, 0.0f, 1.0f);

	GLubyte c[] = { (GLubyte)(clampedColor.r * 255),
		(GLubyte)(clampedColor.g * 255),
		(GLubyte)(clampedColor.b * 255),
		(GLubyte)(clampedColor.a * 255) };

	setPixel(x, y, c);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void ColorBuffer::setPixel(float x, float y, const color & rgba) {
	
	color clampedColor = clamp(rgba, 0.0f, 1.0f);

	GLubyte c[] = { (GLubyte)(clampedColor.r * 255),
		(GLubyte)(clampedColor.g * 255),
		(GLubyte)(clampedColor.b * 255),
		(GLubyte)(clampedColor.a * 255) };

	setPixel(x, y, c);

} // end setPixel

/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void ColorBuffer::setPixel(int x, int y, const GLubyte rgba[]) {

	if (x >= 0 && x < window.width && y >= 0 && y < window.height) {
		std::memcpy( colorBuffer + BYTES_PER_PIXEL * (x + y * window.width), 
					 rgba, BYTES_PER_PIXEL);
	}

} // end setPixel