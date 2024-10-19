# Lighting
A normal `vec3` vector can be used to represent colour, and each component should be between 0.0f and 1.0f.<br>
The colour can be used for both the light and the material, and, to get the resultant colour of light shining on a material, simply do `colourLight * colourMaterial`.<br>

Then we can make another shader, just to hold our light source/lamp, and then make a cube and transform it, and use this shader for it (passing the view and projection matrices to it as well), for our independent lamp.<br>

Then we get started on the actual proper lighting stuff.<br>

Ambient is just lighting from the surroundings, so an object is not completely black, diffuse is making parts of an object closer to the light source brighter, and those that are further/are not facing the source, darker. And finally you add in specular highlights, which is that the closest parts to the light source are much brighter, which can be used to give a plastic-like look to objects.<br>
This is the phong reflection model (ambient + diffuse + speculare = phong reflection).<br>

Whenever we apply a non-uniform scale the normal vectors are not perpendicular to the corresponding surface anymore which distorts the lighting. To fix this you take the transpose of the inverse of the model matrix, which applies a similar transform to the normal, but without making it become not-perpendicular.<br>

To make the diffuse lighting work, you need the direction of the in coming ray, and the normal vector, which needs to be supplied by the model.<br>

For this part, I decided to ditch the EBO (element buffer object) and just use an unindexed vertex array, as the one I used came with the normals already in it, and it would be too much work to make this manually, when I'll be learning model loading later.<br>

For specular highlights, firstly get the vector from the camera to the fragment (so `cameraPosVec - fragPosVec`), and then get the negative of the light direction vector from earlier, (so it is from the light source to the fragment), and reflect it relative to the normal (using the `reflect()` function in OpenGL), and then do the dot product of the camera-to-fragment vector and the reflected ray vector, so it is closer to 1 when you're closer to looking at the reflection, and then clamp it to a minimum of 0 using the `max()` function. And then, raise that to a power (the higher, the more focused the highlight, a power of 32 is decent), using the `pow()` function, then scale this highlight by however much (0.5 is decent), and then multiply it by the colour of the light.<br>

The reflectivity can also be referred to as the 'shinniness', and typically, it's talking about the power you raise the specular bit to.

Finally, once the ambient, diffuse and specular components have been retrieved, do this:<br>
`vec3 resultantVector = (ambient + diffuse + specular) * objectColour;`, and then just pass it on to the output `vec4` vector for colour, and you're done.

## Materials
You can set all of the uniforms and stuff in the fragment shader to make things easier, and to speed things up even more, you can make structs to hold data for materials.

Literally just moving all the stuff to structs and uniforms from the previous section, and adding more customisability through the uniforms.
Allows you to make certain materials look specifically a certain way.