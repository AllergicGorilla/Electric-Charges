//uniform sampler2D texture;
vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
void main()
{
    // lookup the pixel in the texture
    //vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color

    gl_FragColor = mix(red, gl_Color, 0.1) * gl_Color;
}
