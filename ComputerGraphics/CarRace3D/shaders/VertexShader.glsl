#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 object_color;
uniform vec3 car_position;

// Output value to fragment shader
out vec3 color;

void main()
{
    color = object_color;

    // salvez pozitiile vertexilor in coordonatele lumii
    gl_Position = Model * vec4(v_position, 1.0);
    
    // calculez distantele dintre puncte si modific componenta y a tuturor vertexilor 
    // pentru a obtine efectul de curbura (factor 0.0065)   
    float len = length(vec4(car_position, 1.f)  - gl_Position);
    gl_Position.y -= len * len * 0.0065f;
    
    // Compute gl_Position
    gl_Position = Projection * View * gl_Position;
}