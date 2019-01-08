# Element Buffer Objects
This replaces the Shaders function with a class, and uses try-catch blocks for exception catching.<br>
It also adds the feature of setting uniforms through a member function of the shader class.<br>
The glsl files have been slightly modified to experiment with the added features.<br>
The output colour of the triangle is black in the bottom left part, because I have made it equal to the NDC coordinates of that pixel, which is run for every pixel of course (like shaders are), so, as the coordinate is 0,0 for that corner of the triangle, and it gets closer to 0 in both x and y, the colour gets closer to black until the bottom right corner is completely black (as it is at 0,0 as mentioned earlier).
