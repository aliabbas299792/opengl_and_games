# Element Buffer Objects
This program is the same as the 'Triangles and Shaders' one, except that it uses an Element Buffer Object, where you can use this buffer to store indicies of repeated vertices rather than each vertex separately.
For drawing a square, 6 verticies are needed, but using an EBO only 4 need to be specified, so it is much more efficient, moreso in larger examples of course.
Additionally I have used uniforms to update the colours on a per frame basis, and as such the rectangle updates it's colours on a per frame basis.
And have now commented that out, and updated the vertices array to hold colour as well.
