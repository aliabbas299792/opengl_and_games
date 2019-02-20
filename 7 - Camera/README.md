# Camera Stuff
So you set a camera position, and a target for what it is pointing at.
Then you subtract the former from the latter to get a vector pointing towards positive z (i.e out of the screen).
Then we make an arbritary up vector `vec3(0.0f, 1.0f, 0.0f)`, just pointing upwards.
We use this to do `glm::normalize(glm::cross(up, cameraDirection))`, to normalise the cross product of the up and the camera direction, which gets a vector perpendicular to both, which would be the positive x axis in this case.
Then we do `glm::cross(cameraDirection, cameraRight)`, a cross product with 2 direction vectors, to get the up vector of the camera, rather than using the arbritary one.
That would allow you to manually create a 'lookAt()' matrix, but GLM does it for you.

So normally, you will make a 'view' vector, which employs glm::lookAt(), and takes 3 vectors, and creates a view matrix for you:
![matrix](https://res.cloudinary.com/deylrqt2d/image/upload/v1550683537/lookAt_k68exa.png)

The lookAt function is like this: `view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp)`

Then movement is simple, you change the position based on WASD input or something, and also change the target for the lookAt() function as well, so that the target is always the same distance in front of the camera (negative Z though, as positive Z is towards the viewer).

