# Transformations
You can normalise a vector into a unit vector by dividing each component by the magnitude of the vector.

## Vectors

### Vector-vector multiplication
You have the the dot product and the cross product.

#### Dot product
The dot product, so v⋅k, with vectors v and k, is ||v||⋅||k||⋅cosθ, i.e the magnitude of the vectors and cos of the angle between them multiplied.

This allows us to get the angle between them, by first normalising the vectors and multiplying, so the answer when put through that formula is simply cosθ.

You can work out the dot product by multiplying the product of the x components of v and k, then add to the product of the other two components.
So if v is (0.8, -0.6, 0) and k is (0, 1, 0), then the dot product is -0.8.

Then you can reverse the ||v||⋅||k||⋅cosθ (as v and k were normalised above), and get the angle between them, which is useful for lighting calculations.

#### Cross product
![equation](https://latex.codecogs.com/png.latex?%5Cinline%20%5Cbg_white%20%5Cfn_jvn%20%5Cbegin%7Bpmatrix%7D%20A_%7Bx%7D%20%5C%5C%20A_%7By%7D%20%5C%5C%20A_%7Bz%7D%20%5Cend%7Bpmatrix%7D%20%5Ctimes%20%5Cbegin%7Bpmatrix%7D%20B_%7Bx%7D%20%5C%5C%20B_%7By%7D%20%5C%5C%20B_%7Bz%7D%20%5Cend%7Bpmatrix%7D%20%3D%20%5Cbegin%7Bpmatrix%7D%20A_%7By%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bz%7D%20-%20A_%7Bz%7D%20%5C%20%5Ccdot%20%5C%20B_%7By%7D%20%5C%5C%20A_%7Bz%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bx%7D%20-%20A_%7Bx%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bz%7D%20%5C%5C%20A_%7Bx%7D%20%5C%20%5Ccdot%20%5C%20B_%7By%7D%20-%20A_%7By%7D%20%5C%20%5Ccdot%20%5C%20B_%7B%7D%20%5Cend%7Bpmatrix%7D)<br>
For starters imagine a sequence xyzxyz, keep that in mind.<br><br>

The cross product uses the other 2 components out of x, y and z to make up one component, so x⋅y = z, z⋅x = y and y⋅x = z.<br>Note that the x and y are after one another in the sequence and output a positive z, however if it was instead y⋅x, we'd have negative z.<br>So the order of the components picked out from that sequence decided whether it is positive or negative, and as such there will be 2 ways to find every component, one will be positive, the other will be negative.<br><br>They will add up to give the output component of the cross product.
The total number of combinations to get output components is 6, (2\*3), and the way they make each component is listed below:<br>
![equation](https://latex.codecogs.com/png.latex?%5Cinline%20%5Cbg_white%20%5Cfn_jvn%20%5C%5COutput_%7Bx%7D%20%3D%20A_%7By%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bz%7D%20-%20A_%7Bz%7D%20%5C%20%5Ccdot%20%5C%20B_%7By%7D%20%5C%5COutput_%7By%7D%20%3D%20A_%7Bz%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bx%7D%20-%20A_%7Bx%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bz%7D%20%5C%5COutput_%7Bz%7D%20%3D%20A_%7Bx%7D%20%5C%20%5Ccdot%20%5C%20B_%7By%7D%20-%20A_%7By%7D%20%5C%20%5Ccdot%20%5C%20B_%7Bx%7D)<br>
Which is essentially what the output matrix is.

## Matrices
### Matrix addition and subtraction
![equation](https://latex.codecogs.com/png.latex?%5Cinline%20%5Cbg_white%20%5Cfn_jvn%20%5Cbegin%7Bbmatrix%7D%20a%20%26%20b%20%5C%5C%20c%20%26%20d%20%5Cend%7Bbmatrix%7D%20&plus;%20k%20%3D%20%5Cbegin%7Bbmatrix%7D%20a%20&plus;%20k%20%26%20b%20&plus;%20k%20%5C%5C%20c%20&plus;%20k%20%26%20d%20&plus;%20k%20%5Cend%7Bbmatrix%7D)<br>
The same rules apply for subtracting one value.

Adding and subtracting same sized matrices works just as expected:<br>
![equation](https://latex.codecogs.com/png.latex?%5Cinline%20%5Cbg_white%20%5Cfn_jvn%20%5Cbegin%7Bbmatrix%7D%20a%20%26%20b%20%5C%5C%20c%20%26%20d%20%5Cend%7Bbmatrix%7D%20&plus;%20%5Cbegin%7Bbmatrix%7D%20e%20%26%20f%20%5C%5C%20g%20%26%20h%20%5Cend%7Bbmatrix%7D%20%3D%20%5Cbegin%7Bbmatrix%7D%20a%20&plus;%20e%20%26%20b%20&plus;%20f%20%5C%5C%20c%20&plus;%20g%20%26%20d%20&plus;%20h%20%5Cend%7Bbmatrix%7D)<br>
Same for subtracting.

### Matrix multiplication
This does not work as expected, but it does work as expected for a scalar.<br>
![equation](https://latex.codecogs.com/png.latex?%5Cinline%20%5Cbg_white%20%5Cfn_jvn%20%5Cbegin%7Bbmatrix%7D%20a%20%26%20b%5C%5C%20c%20%26%20d%20%5Cend%7Bbmatrix%7D%20%5Ctimes%20k%20%3D%20%5Cbegin%7Bbmatrix%7D%20a%5Ccdot%20k%20%26%20b%5Ccdot%20k%20%5C%5C%20c%5Ccdot%20k%20%26%20d%5Ccdot%20k%20%5Cend%7Bbmatrix%7D)

Matrix-matrix multiplication is different:<br>
![equation](https://learnopengl.com/img/getting-started/matrix_multiplication.png)

#### Matrix-vector multiplication
A vector is a 1x(number of components) matrix, so it can be used in multiplication.<br>
![equation](https://latex.codecogs.com/png.latex?%5Cinline%20%5Cbg_white%20%5Cfn_jvn%20%5Cbegin%7Bbmatrix%7D%20%7B%5Ccolor%7BRed%7D%20k%7D%20%26%200%20%26%200%20%26%20%7B%5Ccolor%7BBlue%7D%20e%7D%5C%5C%200%20%26%20%7B%5Ccolor%7BRed%7D%20l%7D%20%26%200%20%26%20%7B%5Ccolor%7BBlue%7D%20f%7D%5C%5C%200%20%26%200%20%26%20%7B%5Ccolor%7BRed%7D%20m%7D%20%26%20%7B%5Ccolor%7BBlue%7D%20g%7D%5C%5C%200%20%26%200%20%26%200%20%26%201%20%5Cend%7Bbmatrix%7D%20%5Ctimes%20%5Cbegin%7Bbmatrix%7D%20a%5C%5C%20b%5C%5C%20c%5C%5C%201%20%5Cend%7Bbmatrix%7D%20%3D%20%5Cbegin%7Bbmatrix%7D%20%7B%5Ccolor%7BRed%7D%20k%7Da%20&plus;%20%7B%5Ccolor%7BBlue%7D%20e%7D%5C%5C%20%7B%5Ccolor%7BRed%7D%20l%7Db%20&plus;%20%7B%5Ccolor%7BBlue%7D%20f%7D%5C%5C%20%7B%5Ccolor%7BRed%7D%20m%7Dc%20&plus;%20%7B%5Ccolor%7BBlue%7D%20g%7D%5C%5C%201%20*%201%20%5Cend%7Bbmatrix%7D%20%5C%5C%5C%5CThe%20%5C%20vector%20%5C%20represents%20%5C%20the%20%5C%20current%20%5C%20coordinate.%20%5C%5C%5C%5CThe%20%5C%20final%20%5C%20component%20%5C%20just%20%5C%20tells%20%5C%20us%20%5C%20whether%20%5C%5C%20or%20%5C%20not%20%5C%20it%20%5C%20is%20a%20%5C%20direction%20%5C%20vector.%20%5C%200%20%5C%20indicates%20%5C%5C%20it%20%5C%20is.)<br>
The red components of the 4x4 (translation) matrix, are used to scale the vector.
The blue components of the matrix are used to translate the vector.

This is rotation:<br>
![rotationEquations](https://res.cloudinary.com/deylrqt2d/image/upload/v1547336451/rotation_jbg2lj.png)<br><br>

That is the maths bit, I'll be applying it using GLM for this program.

But before doing that I've got some commented out code for plotting a cubic graph in main.cpp.
