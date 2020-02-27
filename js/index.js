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

function basic_initBuffers(gl){
    const positionBuffer = gl.createBuffer(); //buffer for shape positions
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer); //buffer operations will be on positionBuffer from here on out

    const positions = [ //square vertices
        -1.0, 1.0,
        1.0, 1.0,
        -1.0, -1.0,
        1.0, -1.0,
    ];

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW); //pass positions to webgl to make shape from js array, into the current buffer from above

    return {
        position: positionBuffer,
    };
}

function drawScene(gl, programInfo, buffers){ //basic function to draw square
    gl.clearColor(0.0, 0.0, 0.0, 1.0); //clear to opaque black
    gl.clearDepth(1.0); //clears everything
    gl.enable(gl.DEPTH_TEST); //enables depth testing
    gl.depthFunc(gl.LEQUAL); //near things are in front of further things
    
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT); //clear the canvas

    //create perspective matrix
    const fov = Math.PI / 4; //45 degrees fov
    const aspectRatio = gl.canvas.clientWidth / gl.canvas.clientHeight;
    const zNear = 0.1;
    const zFar = 100.0;
    const projectionMatrix = mat4.create();
    mat4.perspective(projectionMatrix, fov, aspectRatio, zNear, zFar); //sets perspective matrix

    const modelViewMatrix = mat4.create();
    mat4.translate(modelViewMatrix, modelViewMatrix, [-0.0, 0.0, -6.0]); //1st: destination matrix, 2nd: matrix to translate, 3rd: amount to translate
    
    const numerOfComponents = 2; //number of components per generic vertex attribute
    const type = gl.FLOAT; //32 bit float data
    const normalize = false; //no normalisation
    const stride = 0; //bytes to get from one value to the next, type and num components are used to determine it instead (2*FLOAT_size)
    const offset = 0; //offset from beginning of buffer
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
    gl.vertexAttribPointer(programInfo.attribLocations.vertexPosition, numerOfComponents, type, normalize, stride, offset);
    gl.enableVertexAttribArray( programInfo.attribLocations.vertexPosition);
    
    gl.useProgram(programInfo.program); //use the shader program

    //set the uniforms for the variables we set in glsl
    gl.uniformMatrix4fv(programInfo.uniformLocations.projectionMatrix, false, projectionMatrix);
    gl.uniformMatrix4fv(programInfo.uniformLocations.modelViewMatrix, false, modelViewMatrix);
    
    const startIndex = 0; //starting index
    const vertices = 4; //number of vertices
    gl.drawArrays(gl.TRIANGLE_STRIP, startIndex, vertices); //draw using method we want (TRIANGLE_STRIP), starting index, and number of vertices
}

function main(){
    var gl = canv1.getContext("webgl");

    if(gl === null){ //strict equality test, if it wasn't initialised
        alert("WebGL unsupported.");
        return;
    }

    const shaderProgram = initShader(gl, vertexShader, fragmentShader);

    const programInfo = { //effectively convenient way to store references to where to set uniforms/attribs
        program: shaderProgram,
        attribLocations: {
            vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'), //index of the vertex attrib to be modified
        },
        uniformLocations: {
            projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'), //used in passing uniforms to these variables in glsl
            modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'), //used in passing uniforms to these variables in glsl
        },
    };

    drawScene(gl, programInfo, basic_initBuffers(gl));
}

window.addEventListener('load', function(){setWindowSize(window, canv1);});
window.addEventListener('resize', function(){setWindowSize(window, canv1);});