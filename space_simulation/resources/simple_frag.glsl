#version 120

uniform sampler2D alphaTexture;

varying vec4 vCol;

void main()
{
    float alpha = texture2D(alphaTexture, gl_PointCoord).r;
    gl_FragColor = vec4(vCol.rgb, vCol.a*alpha);
}
