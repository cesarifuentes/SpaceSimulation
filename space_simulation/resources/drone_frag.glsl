#version 120

uniform vec3 lightPos; // in camera space
varying vec3 vNor; // camera space
varying vec3 vPos;
varying vec2 vTex;
uniform vec3 ka;
uniform vec3 ks;
uniform float s;
uniform sampler2D kdTex;
uniform sampler2D ksTex;
uniform sampler2D nmTex;

void main()
{
    
    vec3 n = normalize(vNor);
    vec3 l = normalize(lightPos - vPos);
    vec3 v = -normalize(vPos);
    vec3 h = normalize(l + v);
    
    vec4 colorT = texture2D(kdTex, vTex.st).rgba;
    vec4 lightT = texture2D(ksTex, vTex.st).rgba; // added
    vec4 normalT = texture2D(nmTex, vTex.st).rgba; // added, normal vector from normal map
    
    // n = normalT.rbg;
    
    vec3 colorA = ka;
    vec3 colorD = max(dot(l, n), 0.0) * colorT.rgb;
    vec3 colorS = pow(max(dot(h, n), 0.0), s) * lightT.rgb; // ks unused here
    vec3 color = colorA + colorD + colorS;
    gl_FragColor = vec4(color, colorT.a);

}





//    vec3 kd = texture2D(texture0, vTex0).rgb;
//    vec3 ks = texture2D(texture1, vTex0).rgb;
//    vec3 ks = texture2D(texture1, vTex0).rgb;




// colored
//    vec3 kd = texture2D(texture0, vTex0).rgb;
//    gl_FragColor = vec4(kd, 1.0);
//
//    // black and white
//    vec3 ks = texture2D(texture1, vTex0).rgb;
//
//    // clouds
//    // vec2 vTexMod = (T * vec3(vTex0,1.0)).rg; // rotate clouds
//    // vec3 kCloud = texture2D(texture2, vTexMod).rgb;
//
//    // Blinn-Phong
//    vec3 n = normalize(vNor);
//    vec3 vLte = normalize(lightPosCam - vPos); // camera space
//
//    // diffuse component
//    vec3 cd = kd * max(0, dot(vLte,n) );
//
//    // specular component
//    vec3 ce = normalize(vec3(0,0,0) - vPos); // camera space
//    vec3 vH = normalize(ce + vLte); // camera space
//    vec3 cs = ks * pow(max(0,dot(vH,n)), 50.0);
//
//    vec3 color = cd + cs; // + kCloud;
//
//    // gl_FragColor = vec4(color.r, color.g, color.b, 1.0);


//     vec3 kd = texture2D(texture0, vTex0).rgb;
//     gl_FragColor = vec4(kd, 1.0);
