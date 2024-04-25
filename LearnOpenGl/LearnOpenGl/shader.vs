#version 330 core //declare the version of opengl.

layout (location = 0) in vec3 Position;//declaring the attributes it expects to receive.

uniform float gScale;

//shader main function.
void main()//takes no input or output since we are using the in and out qualifiers.
{
	// we are outputting out a vec4 and not a vec3.
	gl_Position = vec4(gScale*Position.x, gScale*Position.y, Position.x, 1.0);

}



//single attribute that goes into the shader.
//make sure that the vertex shader is executed on a single input vertex
//and must output a single vertex no more no less.
//Position = Name of the attribute.
//vec3 = type of the input.(this is the native own type in GLSL).
//in = storage qualifier, telling the shader if this is input or output.
//(location = 0)  = we need to bind the GLSL attrib into a variable in a buffer in out c++ app.

// gl_Position is set in the vertex shader to define each vertex's position in clip space.
// This variable is crucial for multiple stages in the rendering pipeline:

// 1. Perspective Division: Converts clip space coordinates to normalized device coordinates (NDC) by dividing x, y, z by w.
//    This determines the vertex's location on screen within the canonical view volume (-1 to 1 for x, y, z).

// 2. Clipping: Vertices outside the view volume are clipped. This process relies on the w component to define visibility within the view frustum.

// 3. Rasterization: Post clipping, the rasterizer uses these coordinates to generate fragments, which are then mapped to pixels on the viewport.
//    Depth testing during this stage uses the z-value from gl_Position to resolve which fragments are visible based on depth.

// 4. Viewport Transformation: Finally, coordinates are scaled and translated based on the viewport settings to fit the specific screen dimensions.

// By defining gl_Position, the shader controls vertex positioning, visibility, and rendering order in the graphics pipeline.

//uniforms
//uniform float gScale;

//(tells the Shader compiler that it needs to allocate some space for this vriable of the type specified.)
//if we dont initalise the system will init to zero.(good for consistency.)
//uniform: third storage qualifier 