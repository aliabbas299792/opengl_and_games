attribute vec4 aVertPos; //vertex position attribute
attribute vec4 aVertColour; //vertex aVertColour

uniform mat4 uMVM; //model view matrix
uniform mat4 uProjMat; //projection matrix

varying lowp vec4 vertColour;

void main() {
    gl_Position = uProjMat * uMVM * aVertPos;
    vertColour = aVertColour; //passes on colour to fragment shader
}