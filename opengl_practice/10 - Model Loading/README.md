# Assimp
Assimp stands for Open Asset Import Library.<br>

For Ubuntu, it was an easy install, however for Windows, you'll probably need to use CMake to generate the necessary include files (for Visual Studio 15 - which is Visual Studio 2017), and then get the .sln project file, open it, run it, and it will generate the needed lib files in the debug folder for you to use.<br>
In Ubuntu you just do install `libassimp-dev`, and in the `compile.sh` file, or however you're compiling, just add in `-lassimp`.<br>

For either setup, you include the following to use assimp:<br>
```
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
```
If you want to see manual object loading without a library, go to my old OpenGL with SFML repository, it does it, however, it misses out a single vertex for some reason (and only work with .obj files).<br>

Assimp supports the importing of objects stored from many different file formats, and essentially stores the data in a series of objects, which are the same no matter the object, so it makes it realtively easy to extract essential data.<br>

There is a Scene object which contains all of the data, and references to a Root node, and Children nodes, which can help to define the structure of an overall model. The Scene object itself however contains all of the materials and meshes, it's just that they're ordering and pairing is down to the Root and then the Child nodes.<br>
Also, despite the fact that you can just extract the data straight from the Scene object, you can import larger models (say, of an entire house, complete with a balcony and some trees), which would have multiple meshes in different places, and then the Root/Child nodes will help to keep the relationships of those meshes (the individual parts of the models) clear and allow us to use them relatively easily.

## Mesh class and Model class
So basically, to store the mesh, you need to store it's vertices (so there positions, normals and texture coordinates as well), the indices for an element buffer object (because indexed array drawing is better, look to `03 - Element Buffer Objects` for more info), and the textures.<br>
To make this easier, make a Vertex struct with `vec3`'s of Position, Normals and Texture Coordinates, as well as a Texture struct with an `int` id, and a `std::string` holding it's type.<br>
Then make vectors for them, in the public access specifier of the Mesh class, and a constructor for it, along with a Draw function, taking one parameter of the Shader class.<br>
Then in the private part, 3 unsigned ints for the VBO, VAO and the EBO, and a setupMesh() function, for setting up the buffer objects and such, which is essentially what we went through in the earlier tutorials, but in a function.<br>

From there you need to make a Model class with it's own Draw public function, which would just loop through all the meshes and draw them, and then in the private bit, a vector for holding meshes, and functions for retreiving them (recursively), as well as a vector for textures.<br>

To see the full implementation, just go to the files, but after this there is a functional object loader.

-----------------------------------------

# A note
I will from this point forwards be developing for windows exclusively, due to complications with my old graphics card and the drivers, or possibly some underlying syntax issue, which has prevented from my attempts at achieving model loading from reaching fruition.<br>
Though it should be noted, that I have now a decent amount of experience in developing for Ubuntu, and, despite the fact that I will no longer be able to continue, it has definitely helped me in other Linux based projects which I have undertaken (like programming a TCP socket server), so this was no an entirely fruitless endeavour.
<br>
The entire final project will be Windows only.
