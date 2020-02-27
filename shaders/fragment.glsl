varying lowp vec4 vertColour;

void main() {
    gl_FragColor = vertColour; //simply sets colour for this fragment (it's interpolate automatically between the vertices)
}