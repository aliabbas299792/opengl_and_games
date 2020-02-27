var canv1 = document.getElementById("canv1");

var fragmentShader = "";
var vertexShader = "";

//load fragment shader, then the vertex shader, then start the program
readTextFile("shaders/fragment.glsl").then(function(returnText){
    fragmentShader = returnText
    readTextFile("shaders/vertex.glsl").then(function(returnText){
        vertexShader = returnText;
        main();
    });
});

function main(){
    var gl = canv1.getContext("webgl");

    if(gl === null){ //strict equality test, if it wasn't initialised
        alert("WebGL unsupported.");
        return;
    }

    const shaderProgram = initShader(gl, vertexShader, fragmentShader);

    const programInfo = { //effectively convenient way to store references to where to set uniforms/attribs
        program: shadferProgram,
        attribLocations: {
            vertexPosition: gl.setAttribLocation(shaderProgram, 'aVertexPosition'), //index of the vertex attrib to be modified
        },
        uniformLocations: {
            projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'), //used in passing uniforms to these variables in glsl
            modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'), //used in passing uniforms to these variables in glsl
        },
    };

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
}

window.addEventListener('load', function(){setWindowSize(window, canv1);});
window.addEventListener('resize', function(){setWindowSize(window, canv1);});