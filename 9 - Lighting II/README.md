# Material/Lighting Maps
Using the material structs from earlier, just removing the diffuse and material vectors, and replacing them with a single `sampler2D`, allows us to sample directly from a texture to generate a map, and will be able to do more interesting things later.<br>
For now, adding this to the fragment shader, and using it in the ambient and diffuse lighting vectors (like this: `vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));` and `vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));`) sets the precedence for more interesting effects, like specular maps.<br><br>

You can also add the `sampler2D` thing for the specular component, so you can make certain parts of the object shinier than others.<br>
You can also use it to overlay another texture, in the form of an emission map, to certain parts of the object.<br>

# Light types
### Directional lights
Essentially modelling the light as infinitely far away, so just comes from a direction.<br>
As such, you don't need the position of the light anymore, just the direction.<br>

It is convention to give the direction from the source to the fragment, rather than the other way round, so when normalising the direction, you need to normalise the negative of the light direction, as that is what we use to do our calculations.

### Point lights
They're essentially the one's which I used since the beginning, except that they didn't ever decrease in intensity with distance, which is generally called attenuation. It's like the typical 1/d^2, but point lights can act differently, so to get a more realistic effect, you do:<br>
![attenuation equation](https://res.cloudinary.com/deylrqt2d/image/upload/v1550853379/Screenshot_from_2019-02-22_16-36-03_priljc.png)<br>
As you can see, it is a quadratic equation.<br>
`d` is the distance, and the other terms (`Kc` (constant term), `Kl` (linear term), `Kq` (quadratic term)), are generally just inputted based on your needs, a good guide is this table:<br>
![table of values](https://res.cloudinary.com/deylrqt2d/image/upload/v1550853854/pointLightValues_vtnbtr.png)<br>
Implementing it is easy enough, just another 3 floats to send through to the fragment shader through the light struct uniform, then multiply the ambient, diffuse and specular resultant components by this attenuation - so you generally leave the emission bit alone, and it really does make it look like it is bright enough to be glowing (it's too bright for testing though, to be honest).<br>
Note that this is different from a spotlight.

### Spot lights
They have a maximum angle, which forms a cone like shape of illumination.<br>
![spot light](https://learnopengl.com/img/lighting/light_casters_spotlight_angles.png)<br><br>

All you need to make it, is add two variables to your struct, a `vec3` for the direction of the light (normalised), and a `float` for the cutoff, which, if you're dealing with degrees, you should input like this: `cos(glm::radians(15.0f))`. This means that the inverse cos of the dot product doesn't have to be calculated later on, and it is an expensive operation, so we don't want to. Then all you need to do is: `dot(lightDirToSourceFromFragment, normalize(-light.direction))`, and compare it to the cutoff from the uniform in the struct you specified, if `cosThetaBetweenFragmentAndLight < light.cutOff`, then just make the attenuation 0, as that will mean that specular, diffuse and ambient are 0, so nothing will be seen past the cutoff point.<br>
Remember, we are checking if the `cosThetaBetweenFragmentAndLight` angle is less than the cutoff angle because on a cos graph, a lower value is a higher angle, so it is checking if the `cosThetaBetweenFragmentAndLight` is greater than the cutoff angle, and then doing something based on that. <br><br>

To ease the sharp cut off at the border of the spotlight, you can add in another cutoff, and sort of interpolate between the inner border and outer border to achieve a more smooth transition. I've set the outerborder to always be 0.05 less than the inner border (it is a cos value, so the angle is higher), and have multiplied the specular and diffuse components of the lighting by it.<br>
This is the equation (the intensity equation):
![intensity equation](https://res.cloudinary.com/deylrqt2d/image/upload/v1550921877/intensity_t0iyov.png)<br><br>

This means that the center has a value much higher than 1, and anything past the outer cutoff has a value less than 0, so clamping it to a range of 0 to 1 means that the blur which is observed between the cutoffs is the only range of light visible, however I've noticed, that just clamping to a min of 0 and max of something higher makes it more realistic (as the center is brighter and so more specular lighting occurs when you're closer).