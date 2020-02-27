function setWindowSize(window, box){
    box.setAttribute("width", window.innerWidth);
    box.setAttribute("height", window.innerHeight);
}

function loadShader(gl, type, source){ //creates a shader of a given type, uploads source and compiles
    //make a shader
    const shader = gl.createShader(type);

    //set the shader source to the shader object, source code is sent to shader using this
    gl.shaderSource(shader, source);

    //compile the shader program
    gl.compileShader(shader);

    //if unsuccessful compilation
    if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS)){
        alert("An error occured while compiling the shaders" + gl.getShaderInfoLog(shader));
        gl.deleteShader(shader); //clean up after you
        return null;
    }

    //return the shader
    return shader;
}

function readTextFile(filePath){
    return fetch(filePath)
        .then(function(response){
            return response.text(); //this returns a promise, so this entire function returns a promise to use .then on
        });
}

function initShader(gl, vertex, fragment){
    //load shaders
    const vertShader = loadShader(gl, gl.VERTEX_SHADER, vertex);
    const fragShader = loadShader(gl, gl.FRAGMENT_SHADER, fragment);

    //create shader program
    const shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertShader);
    gl.attachShader(shaderProgram, fragShader);
    gl.linkProgram(shaderProgram);

    //did the program creation fail? if so, print something useful
    if(!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)){
        alert("Unable to initialise the shader program: " + gl.getProgramInfoLog(shaderProgram));
        return null;
    }

    //return the program
    return shaderProgram;
}