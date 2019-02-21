# Camera Stuff
So you set a camera position, and a target for what it is pointing at.<br>
Then you subtract the former from the latter to get a vector pointing towards positive z (i.e out of the screen).
Then we make an arbritary up vector `vec3(0.0f, 1.0f, 0.0f)`, just pointing upwards.<br>
We use this to do `glm::normalize(glm::cross(up, cameraDirection))`, to normalise the cross product of the up and the camera direction, which gets a vector perpendicular to both, which would be the positive x axis in this case `std::string viewMatName`, `std::string projectMatName`.<br>
Then we do `glm::cross(cameraDirection, cameraRight)`, a cross product with 2 direction vectors, to get the up vector of the camera, rather than using the arbritary one.<br>
That would allow you to manually create a 'lookAt()' matrix, but GLM does it for you.<br>

So normally, you will make a 'view' vector, which employs glm::lookAt(), and takes 3 vectors, and creates a view matrix for you:<br>
![matrix](https://res.cloudinary.com/deylrqt2d/image/upload/v1550683537/lookAt_k68exa.png)

### Movement and the lookAt() function
The lookAt function is like this: `view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp)`
The first argument is the camera positon, the second is the 'target' vector, the one the camera points at, and the third is the up vector, which is literally just like `vec3(0.0f, 1.0f, 0.0f)`, and is used for the cross product internally, as described above.
The second argument has `cameraFront + cameraPos`, as we specify that the target is translated the same amount as the camera itself, so it is always looking into the negative Z in front of the camera, by adding those vectors up.
They are all `vec3`.

Then movement is simple, you change the position based on WASD input or something, and also change the target for the lookAt() function as well, so that the target is always the same distance in front of the camera (negative Z though, as positive Z is towards the viewer).

#### Delta Time
Delta time is used, which is just the time since the last frame, so `currentFrame - lastFrame`, (assuming those variables hold times), and then multiplying this variable by the speed to obtain the new speed, which will mean that the speed of the camera will be the same across systems regardless of the fps.

### Euler Angles<br>
![eulerAngles](https://learnopengl.com/img/getting-started/camera_pitch_yaw_roll.png)<br>
Using a combination of the pitch, yaw, and roll, any 3D rotation can be represented.

For a traditional FPS style camera, you're only interested in the pitch and the yaw.

Also, you would manipulate the `cameraFront` variable, as that is the target that is used in the movement (that multiplied by the speed is the amount you move in a given time frame).
<br>
![xyzThing](https://uploads.disquscdn.com/images/58fa1f1a3dd8d736a9345b3b168dd55caf0f14d485e9dae7e06b8e185348a42a.png)
<br>
The red phi symbol is the yaw, the pink theta symbol is the pitch.
As you can see, `sin(theta)` gives the y displacement from origin, `cos(theta) * sin(phi)` gives the z displacement from origin, and `cos(theta) * cos(phi)` gives the x displacement from origin, in other words, those give the projection of that vector on all three axes separately.

So now you need to obtain the `theta` and `phi` previously mentioned.

And that's easy, literally just set it to be the pixel displacement, and then constrain one of them to 180 degrees, like pitch for example.

Then apply the transformations to the `cameraFront` variable, and normalise it, then you're done.

#### Zooming
Zooming is also very easy, just another callback function, which gives you the data from the mousewheel, and then you can use that to increase/decrease FoV, which acts as zooming in/out.

And basically done, next is lighting.