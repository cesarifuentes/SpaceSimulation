#version 120

attribute vec4 aPos;
attribute vec3 aNor;
attribute vec2 aTex;

uniform mat4 P;
uniform mat4 MV;

varying vec3 vPos; // camera space
varying vec3 vNor;
varying vec2 vTex;

void main()
{
    
    vec4 posCam = MV * aPos;
    vec3 norCam = (MV * vec4(aNor, 0.0)).xyz;
    gl_Position = P * posCam;
    vPos = posCam.xyz;
    vNor = norCam;
    vTex = aTex;
    
}

//     gl_Position = P * MV * aPos;
//     vTex = aTex;
//
//    vPos = (MV * aPos).rgb;
//     vec4 n = vec4(aNor, 0.0);
//     vNor = (MVit * n).rgb; // into camera space
