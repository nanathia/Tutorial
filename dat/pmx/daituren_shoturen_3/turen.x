xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 109;
 -0.00988;-0.14465;-5.09426;,
 -0.00988;-0.08812;-5.04814;,
 0.00000;-0.16063;-5.24459;,
 0.00000;-0.15567;-5.10246;,
 0.00000;-0.01386;-5.05794;,
 0.00000;-0.07863;-5.19321;,
 -0.00988;-0.05017;-2.17193;,
 -0.00988;-0.07684;-3.61794;,
 0.00000;-0.08754;-3.61776;,
 0.00000;-0.06088;-2.17176;,
 -0.00988;0.02570;-2.17319;,
 -0.00988;-0.00678;-3.61919;,
 0.00000;0.09800;-2.17438;,
 0.00000;0.05822;-3.62026;,
 -0.01330;-0.06350;-0.73339;,
 0.00000;-0.07420;-0.73339;,
 -0.00988;0.01240;-0.73339;,
 0.00000;0.08470;-0.73339;,
 -0.01406;-0.06276;-0.66559;,
 -0.01330;-0.06350;-0.73339;,
 0.00000;-0.07420;-0.73339;,
 0.00000;-0.07520;-0.66559;,
 -0.01375;0.01210;-0.66559;,
 -0.00988;0.01240;-0.73339;,
 0.00000;0.08612;-0.66559;,
 0.00000;0.08470;-0.73339;,
 -0.09316;-0.15571;-0.65249;,
 -0.09316;-0.15571;-0.66559;,
 0.00000;-0.20130;-0.66559;,
 0.00000;-0.20130;-0.65249;,
 -0.13174;0.00000;-0.65249;,
 -0.13174;0.00000;-0.66559;,
 -0.09316;-0.15571;-0.66559;,
 -0.09316;-0.15571;-0.65249;,
 -0.09316;0.15571;-0.66559;,
 -0.09316;0.15571;-0.65249;,
 0.00000;0.20130;-0.65249;,
 0.00000;0.20130;-0.66559;,
 -0.09316;0.15571;-0.65249;,
 -0.09316;0.15571;-0.66559;,
 0.00988;-0.08812;-5.04814;,
 0.00988;-0.14465;-5.09426;,
 0.00988;-0.07684;-3.61794;,
 0.00988;-0.05017;-2.17193;,
 0.00988;-0.00678;-3.61919;,
 0.00988;0.02570;-2.17319;,
 0.01330;-0.06350;-0.73339;,
 0.00988;0.01240;-0.73339;,
 0.01330;-0.06350;-0.73339;,
 0.01406;-0.06276;-0.66559;,
 0.00988;0.01240;-0.73339;,
 0.01375;0.01210;-0.66559;,
 0.09316;-0.15571;-0.66559;,
 0.09316;-0.15571;-0.65249;,
 0.09316;-0.15571;-0.66559;,
 0.13174;0.00000;-0.66559;,
 0.13174;0.00000;-0.65249;,
 0.09316;-0.15571;-0.65249;,
 0.09316;0.15571;-0.65249;,
 0.09316;0.15571;-0.66559;,
 0.09316;0.15571;-0.65249;,
 0.09316;0.15571;-0.66559;,
 -0.01375;0.01210;-0.66559;,
 -0.13174;0.00000;-0.66559;,
 0.00000;0.08612;-0.66559;,
 -0.01406;-0.06276;-0.66559;,
 -0.09316;-0.15571;-0.66559;,
 -0.09316;0.15571;-0.66559;,
 0.00000;0.20130;-0.66559;,
 0.00000;-0.07520;-0.66559;,
 0.00000;-0.20130;-0.66559;,
 0.13174;0.00000;-0.66559;,
 0.01375;0.01210;-0.66559;,
 0.01406;-0.06276;-0.66559;,
 0.09316;-0.15571;-0.66559;,
 0.09316;0.15571;-0.66559;,
 -0.02808;-0.08920;0.80604;,
 -0.03719;-0.05043;-0.65249;,
 0.00000;-0.08810;-0.65249;,
 0.00000;-0.11772;0.79753;,
 -0.02092;0.01935;0.81078;,
 -0.02669;0.07769;-0.65249;,
 0.00000;0.04342;0.80456;,
 0.00000;0.10798;-0.65249;,
 0.03719;-0.05043;-0.65249;,
 0.02809;-0.08920;0.80604;,
 0.02669;0.07769;-0.65249;,
 0.02093;0.01935;0.81078;,
 0.02809;-0.08920;0.80604;,
 -0.02808;-0.08920;0.80604;,
 0.00000;-0.11772;0.79753;,
 0.02093;0.01935;0.81078;,
 -0.02092;0.01935;0.81078;,
 -0.02092;0.01935;0.81078;,
 0.00000;0.04342;0.80456;,
 -0.02669;0.07769;-0.65249;,
 -0.13174;0.00000;-0.65249;,
 -0.03719;-0.05043;-0.65249;,
 -0.09316;-0.15571;-0.65249;,
 0.00000;0.20130;-0.65249;,
 -0.09316;0.15571;-0.65249;,
 0.00000;0.10798;-0.65249;,
 0.00000;-0.08810;-0.65249;,
 0.00000;-0.20130;-0.65249;,
 0.03719;-0.05043;-0.65249;,
 0.13174;0.00000;-0.65249;,
 0.02669;0.07769;-0.65249;,
 0.09316;-0.15571;-0.65249;,
 0.09316;0.15571;-0.65249;;
 
 116;
 3;0,1,2;,
 3;0,2,3;,
 3;1,4,5;,
 3;1,5,2;,
 3;6,7,8;,
 3;6,8,9;,
 3;10,11,7;,
 3;10,7,6;,
 3;10,12,13;,
 3;10,13,11;,
 3;14,6,9;,
 3;14,9,15;,
 3;16,10,6;,
 3;16,6,14;,
 3;17,12,10;,
 3;17,10,16;,
 3;8,7,0;,
 3;8,0,3;,
 3;11,1,0;,
 3;11,0,7;,
 3;13,4,1;,
 3;13,1,11;,
 3;18,19,20;,
 3;18,20,21;,
 3;22,23,19;,
 3;22,19,18;,
 3;24,25,23;,
 3;24,23,22;,
 3;26,27,28;,
 3;26,28,29;,
 3;30,31,32;,
 3;30,32,33;,
 3;34,35,36;,
 3;34,36,37;,
 3;31,30,38;,
 3;31,38,39;,
 3;2,40,41;,
 3;3,2,41;,
 3;5,4,40;,
 3;2,5,40;,
 3;8,42,43;,
 3;9,8,43;,
 3;42,44,45;,
 3;43,42,45;,
 3;13,12,45;,
 3;44,13,45;,
 3;9,43,46;,
 3;15,9,46;,
 3;43,45,47;,
 3;46,43,47;,
 3;45,12,17;,
 3;47,45,17;,
 3;41,42,8;,
 3;3,41,8;,
 3;41,40,44;,
 3;42,41,44;,
 3;40,4,13;,
 3;44,40,13;,
 3;20,48,49;,
 3;21,20,49;,
 3;48,50,51;,
 3;49,48,51;,
 3;50,25,24;,
 3;51,50,24;,
 3;28,52,53;,
 3;29,28,53;,
 3;54,55,56;,
 3;57,54,56;,
 3;36,58,59;,
 3;37,36,59;,
 3;60,56,55;,
 3;61,60,55;,
 3;62,63,64;,
 3;65,63,62;,
 3;66,63,65;,
 3;64,63,67;,
 3;64,67,68;,
 3;66,65,69;,
 3;66,69,70;,
 3;64,71,72;,
 3;72,71,73;,
 3;73,71,74;,
 3;75,71,64;,
 3;68,75,64;,
 3;69,73,74;,
 3;70,69,74;,
 3;76,77,78;,
 3;76,78,79;,
 3;80,81,77;,
 3;80,77,76;,
 3;82,83,81;,
 3;82,81,80;,
 3;78,84,85;,
 3;79,78,85;,
 3;84,86,87;,
 3;85,84,87;,
 3;86,83,82;,
 3;87,86,82;,
 3;88,89,90;,
 3;88,91,92;,
 3;88,92,89;,
 3;93,91,94;,
 3;95,96,97;,
 3;97,96,98;,
 3;99,100,101;,
 3;102,97,98;,
 3;103,102,98;,
 3;104,105,106;,
 3;107,105,104;,
 3;101,108,99;,
 3;107,104,102;,
 3;107,102,103;,
 3;95,101,100;,
 3;95,100,96;,
 3;106,105,108;,
 3;106,108,101;;
 
 MeshMaterialList {
  1;
  116;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "tex.jpg";
   }
  }
 }
 MeshNormals {
  67;
  0.000000;-0.999999;-0.001286;,
  -0.934106;-0.356985;0.002694;,
  -0.996823;0.079639;-0.000658;,
  -0.990801;0.135322;-0.001236;,
  0.000000;-0.999483;0.032165;,
  -0.956932;-0.290162;0.009338;,
  -0.998183;0.060208;-0.002389;,
  -0.989905;0.141624;-0.005551;,
  0.000000;-0.999974;-0.007268;,
  -0.918650;-0.394771;-0.015435;,
  -0.995688;0.090732;-0.019313;,
  -0.990798;0.135348;-0.000115;,
  0.000000;-0.999185;0.040373;,
  -0.934747;-0.355305;0.002576;,
  -0.995824;0.084203;-0.035280;,
  -0.991293;0.127481;-0.032961;,
  -0.977501;-0.206997;-0.040555;,
  -0.994644;0.082454;-0.062323;,
  0.000000;-0.348965;-0.937136;,
  -0.560414;-0.373016;-0.739456;,
  -0.812694;0.063154;-0.579259;,
  -0.309212;0.049799;-0.949688;,
  0.000000;-0.668229;0.743956;,
  -0.454407;-0.500391;0.736969;,
  -0.695345;-0.057438;0.716377;,
  0.000000;-0.873747;-0.486382;,
  -0.584870;-0.338829;-0.736969;,
  -0.587852;0.048559;-0.807510;,
  0.000000;0.873747;0.486382;,
  -0.584870;0.338829;0.736969;,
  0.000000;0.668229;-0.743956;,
  -0.543795;0.598824;-0.587960;,
  0.934106;-0.356985;0.002694;,
  0.996823;0.079639;-0.000658;,
  0.956932;-0.290162;0.009338;,
  0.998183;0.060208;-0.002389;,
  0.918650;-0.394771;-0.015435;,
  0.995688;0.090732;-0.019313;,
  0.934747;-0.355305;0.002576;,
  0.995824;0.084203;-0.035280;,
  0.560414;-0.373016;-0.739456;,
  0.812694;0.063154;-0.579259;,
  0.454407;-0.500391;0.736969;,
  0.695345;-0.057438;0.716377;,
  0.584870;-0.338829;-0.736969;,
  0.587852;0.048559;-0.807510;,
  0.584870;0.338829;0.736969;,
  0.543795;0.598824;-0.587960;,
  -0.663431;-0.136166;0.735743;,
  -0.595201;0.334326;0.730727;,
  0.000000;0.309679;0.950841;,
  -0.000015;-0.579513;0.814963;,
  0.663433;-0.136166;0.735742;,
  0.595182;0.334353;0.730730;,
  -0.000039;-0.875541;0.483145;,
  -0.688305;-0.473065;0.549950;,
  -0.655433;0.231596;0.718868;,
  -0.000030;0.936243;0.351354;,
  0.580801;-0.409663;0.703453;,
  0.773021;0.285472;0.566520;,
  0.977501;-0.206997;-0.040555;,
  0.994644;0.082454;-0.062323;,
  0.991293;0.127481;-0.032961;,
  0.989905;0.141624;-0.005551;,
  0.990801;0.135322;-0.001236;,
  0.990798;0.135348;-0.000115;,
  0.986861;0.158936;-0.029074;;
  116;
  3;13,14,16;,
  3;13,16,12;,
  3;14,15,17;,
  3;14,17,16;,
  3;1,5,4;,
  3;1,4,0;,
  3;2,6,5;,
  3;2,5,1;,
  3;2,3,7;,
  3;2,7,6;,
  3;9,1,0;,
  3;9,0,8;,
  3;10,2,1;,
  3;10,1,9;,
  3;11,3,2;,
  3;11,2,10;,
  3;4,5,13;,
  3;4,13,12;,
  3;6,14,13;,
  3;6,13,5;,
  3;7,15,14;,
  3;7,14,6;,
  3;19,9,8;,
  3;19,8,18;,
  3;20,10,9;,
  3;20,9,19;,
  3;21,11,10;,
  3;21,10,20;,
  3;23,26,25;,
  3;23,25,22;,
  3;24,27,26;,
  3;24,26,23;,
  3;31,29,28;,
  3;31,28,30;,
  3;27,24,29;,
  3;27,29,31;,
  3;60,39,38;,
  3;12,60,38;,
  3;61,62,39;,
  3;60,61,39;,
  3;4,34,32;,
  3;0,4,32;,
  3;34,35,33;,
  3;32,34,33;,
  3;63,64,33;,
  3;35,63,33;,
  3;0,32,36;,
  3;8,0,36;,
  3;32,33,37;,
  3;36,32,37;,
  3;33,64,65;,
  3;37,33,65;,
  3;38,34,4;,
  3;12,38,4;,
  3;38,39,35;,
  3;34,38,35;,
  3;39,62,63;,
  3;35,39,63;,
  3;8,36,40;,
  3;18,8,40;,
  3;36,37,41;,
  3;40,36,41;,
  3;37,65,66;,
  3;41,37,66;,
  3;25,44,42;,
  3;22,25,42;,
  3;44,45,43;,
  3;42,44,43;,
  3;28,46,47;,
  3;30,28,47;,
  3;46,43,45;,
  3;47,46,45;,
  3;20,27,21;,
  3;19,27,20;,
  3;26,27,19;,
  3;21,27,31;,
  3;21,31,30;,
  3;26,19,18;,
  3;26,18,25;,
  3;21,45,41;,
  3;41,45,40;,
  3;40,45,44;,
  3;47,45,21;,
  3;30,47,21;,
  3;18,40,44;,
  3;25,18,44;,
  3;55,48,51;,
  3;55,51,54;,
  3;56,49,48;,
  3;56,48,55;,
  3;57,50,49;,
  3;57,49,56;,
  3;51,52,58;,
  3;54,51,58;,
  3;52,53,59;,
  3;58,52,59;,
  3;53,50,57;,
  3;59,53,57;,
  3;58,55,54;,
  3;58,59,56;,
  3;58,56,55;,
  3;56,59,57;,
  3;49,24,48;,
  3;48,24,23;,
  3;28,29,50;,
  3;51,48,23;,
  3;22,51,23;,
  3;52,43,53;,
  3;42,43,52;,
  3;50,46,28;,
  3;42,52,51;,
  3;42,51,22;,
  3;49,50,29;,
  3;49,29,24;,
  3;53,43,46;,
  3;53,46,50;;
 }
 MeshTextureCoords {
  109;
  0.045230;0.022610;,
  0.055150;0.042170;,
  0.012880;0.017080;,
  0.043230;0.016930;,
  0.049820;0.067880;,
  0.023930;0.045460;,
  0.674130;0.055310;,
  0.363170;0.047940;,
  0.362980;0.042370;,
  0.674160;0.051600;,
  0.673860;0.081560;,
  0.362670;0.070320;,
  0.673600;0.106590;,
  0.362440;0.092820;,
  0.985190;0.050690;,
  0.985190;0.046980;,
  0.985190;0.076950;,
  0.985190;0.101980;,
  0.485230;0.232600;,
  0.626100;0.234190;,
  0.626090;0.205490;,
  0.485230;0.203900;,
  0.485230;0.327270;,
  0.626230;0.328850;,
  0.485230;0.366080;,
  0.626340;0.367650;,
  0.960510;0.641260;,
  0.972510;0.641160;,
  0.969770;0.565430;,
  0.958250;0.565830;,
  0.955800;0.346960;,
  0.967040;0.346960;,
  0.966750;0.209590;,
  0.956370;0.210270;,
  0.955550;0.485690;,
  0.967240;0.486840;,
  0.969770;0.565430;,
  0.958250;0.565830;,
  0.955550;0.485690;,
  0.967240;0.486840;,
  0.055150;0.042170;,
  0.045230;0.022610;,
  0.363170;0.047940;,
  0.674130;0.055310;,
  0.362670;0.070320;,
  0.673860;0.081560;,
  0.985190;0.050690;,
  0.985190;0.076950;,
  0.626100;0.234190;,
  0.485230;0.232600;,
  0.626230;0.328850;,
  0.485230;0.327270;,
  0.972510;0.641160;,
  0.960510;0.641260;,
  0.966750;0.209590;,
  0.967040;0.346960;,
  0.955800;0.346960;,
  0.956370;0.210270;,
  0.967240;0.486840;,
  0.955550;0.485690;,
  0.955550;0.485690;,
  0.967240;0.486840;,
  0.800750;0.449600;,
  0.926720;0.436680;,
  0.786070;0.528630;,
  0.801080;0.369670;,
  0.885520;0.270430;,
  0.885520;0.602920;,
  0.786070;0.651590;,
  0.786070;0.356390;,
  0.786070;0.221760;,
  0.645410;0.436680;,
  0.771390;0.449600;,
  0.771050;0.369670;,
  0.686610;0.270430;,
  0.686610;0.602920;,
  0.085200;0.859310;,
  0.963870;0.864500;,
  0.964340;0.837460;,
  0.085480;0.837410;,
  0.084620;0.937180;,
  0.964350;0.949080;,
  0.084620;0.953870;,
  0.965070;0.968760;,
  0.963870;0.864500;,
  0.085200;0.859310;,
  0.964350;0.949080;,
  0.084620;0.937180;,
  0.063040;0.860490;,
  0.024110;0.860490;,
  0.045560;0.840710;,
  0.058070;0.935790;,
  0.029070;0.935790;,
  0.029080;0.935810;,
  0.045560;0.952490;,
  0.814290;0.520530;,
  0.928090;0.436620;,
  0.825660;0.382160;,
  0.886290;0.268460;,
  0.785380;0.654020;,
  0.886290;0.604790;,
  0.785380;0.553230;,
  0.785380;0.341480;,
  0.785380;0.219230;,
  0.745090;0.382160;,
  0.642670;0.436620;,
  0.756470;0.520530;,
  0.684470;0.268460;,
  0.684470;0.604790;;
 }
 MeshVertexColors {
  109;
  0;1.000000;1.000000;1.000000;1.000000;,
  1;1.000000;1.000000;1.000000;1.000000;,
  2;1.000000;1.000000;1.000000;1.000000;,
  3;1.000000;1.000000;1.000000;1.000000;,
  4;1.000000;1.000000;1.000000;1.000000;,
  5;1.000000;1.000000;1.000000;1.000000;,
  6;1.000000;1.000000;1.000000;1.000000;,
  7;1.000000;1.000000;1.000000;1.000000;,
  8;1.000000;1.000000;1.000000;1.000000;,
  9;1.000000;1.000000;1.000000;1.000000;,
  10;1.000000;1.000000;1.000000;1.000000;,
  11;1.000000;1.000000;1.000000;1.000000;,
  12;1.000000;1.000000;1.000000;1.000000;,
  13;1.000000;1.000000;1.000000;1.000000;,
  14;1.000000;1.000000;1.000000;1.000000;,
  15;1.000000;1.000000;1.000000;1.000000;,
  16;1.000000;1.000000;1.000000;1.000000;,
  17;1.000000;1.000000;1.000000;1.000000;,
  18;1.000000;1.000000;1.000000;1.000000;,
  19;1.000000;1.000000;1.000000;1.000000;,
  20;1.000000;1.000000;1.000000;1.000000;,
  21;1.000000;1.000000;1.000000;1.000000;,
  22;1.000000;1.000000;1.000000;1.000000;,
  23;1.000000;1.000000;1.000000;1.000000;,
  24;1.000000;1.000000;1.000000;1.000000;,
  25;1.000000;1.000000;1.000000;1.000000;,
  26;1.000000;1.000000;1.000000;1.000000;,
  27;1.000000;1.000000;1.000000;1.000000;,
  28;1.000000;1.000000;1.000000;1.000000;,
  29;1.000000;1.000000;1.000000;1.000000;,
  30;1.000000;1.000000;1.000000;1.000000;,
  31;1.000000;1.000000;1.000000;1.000000;,
  32;1.000000;1.000000;1.000000;1.000000;,
  33;1.000000;1.000000;1.000000;1.000000;,
  34;1.000000;1.000000;1.000000;1.000000;,
  35;1.000000;1.000000;1.000000;1.000000;,
  36;1.000000;1.000000;1.000000;1.000000;,
  37;1.000000;1.000000;1.000000;1.000000;,
  38;1.000000;1.000000;1.000000;1.000000;,
  39;1.000000;1.000000;1.000000;1.000000;,
  40;1.000000;1.000000;1.000000;1.000000;,
  41;1.000000;1.000000;1.000000;1.000000;,
  42;1.000000;1.000000;1.000000;1.000000;,
  43;1.000000;1.000000;1.000000;1.000000;,
  44;1.000000;1.000000;1.000000;1.000000;,
  45;1.000000;1.000000;1.000000;1.000000;,
  46;1.000000;1.000000;1.000000;1.000000;,
  47;1.000000;1.000000;1.000000;1.000000;,
  48;1.000000;1.000000;1.000000;1.000000;,
  49;1.000000;1.000000;1.000000;1.000000;,
  50;1.000000;1.000000;1.000000;1.000000;,
  51;1.000000;1.000000;1.000000;1.000000;,
  52;1.000000;1.000000;1.000000;1.000000;,
  53;1.000000;1.000000;1.000000;1.000000;,
  54;1.000000;1.000000;1.000000;1.000000;,
  55;1.000000;1.000000;1.000000;1.000000;,
  56;1.000000;1.000000;1.000000;1.000000;,
  57;1.000000;1.000000;1.000000;1.000000;,
  58;1.000000;1.000000;1.000000;1.000000;,
  59;1.000000;1.000000;1.000000;1.000000;,
  60;1.000000;1.000000;1.000000;1.000000;,
  61;1.000000;1.000000;1.000000;1.000000;,
  62;1.000000;1.000000;1.000000;1.000000;,
  63;1.000000;1.000000;1.000000;1.000000;,
  64;1.000000;1.000000;1.000000;1.000000;,
  65;1.000000;1.000000;1.000000;1.000000;,
  66;1.000000;1.000000;1.000000;1.000000;,
  67;1.000000;1.000000;1.000000;1.000000;,
  68;1.000000;1.000000;1.000000;1.000000;,
  69;1.000000;1.000000;1.000000;1.000000;,
  70;1.000000;1.000000;1.000000;1.000000;,
  71;1.000000;1.000000;1.000000;1.000000;,
  72;1.000000;1.000000;1.000000;1.000000;,
  73;1.000000;1.000000;1.000000;1.000000;,
  74;1.000000;1.000000;1.000000;1.000000;,
  75;1.000000;1.000000;1.000000;1.000000;,
  76;1.000000;1.000000;1.000000;1.000000;,
  77;1.000000;1.000000;1.000000;1.000000;,
  78;1.000000;1.000000;1.000000;1.000000;,
  79;1.000000;1.000000;1.000000;1.000000;,
  80;1.000000;1.000000;1.000000;1.000000;,
  81;1.000000;1.000000;1.000000;1.000000;,
  82;1.000000;1.000000;1.000000;1.000000;,
  83;1.000000;1.000000;1.000000;1.000000;,
  84;1.000000;1.000000;1.000000;1.000000;,
  85;1.000000;1.000000;1.000000;1.000000;,
  86;1.000000;1.000000;1.000000;1.000000;,
  87;1.000000;1.000000;1.000000;1.000000;,
  88;1.000000;1.000000;1.000000;1.000000;,
  89;1.000000;1.000000;1.000000;1.000000;,
  90;1.000000;1.000000;1.000000;1.000000;,
  91;1.000000;1.000000;1.000000;1.000000;,
  92;1.000000;1.000000;1.000000;1.000000;,
  93;1.000000;1.000000;1.000000;1.000000;,
  94;1.000000;1.000000;1.000000;1.000000;,
  95;1.000000;1.000000;1.000000;1.000000;,
  96;1.000000;1.000000;1.000000;1.000000;,
  97;1.000000;1.000000;1.000000;1.000000;,
  98;1.000000;1.000000;1.000000;1.000000;,
  99;1.000000;1.000000;1.000000;1.000000;,
  100;1.000000;1.000000;1.000000;1.000000;,
  101;1.000000;1.000000;1.000000;1.000000;,
  102;1.000000;1.000000;1.000000;1.000000;,
  103;1.000000;1.000000;1.000000;1.000000;,
  104;1.000000;1.000000;1.000000;1.000000;,
  105;1.000000;1.000000;1.000000;1.000000;,
  106;1.000000;1.000000;1.000000;1.000000;,
  107;1.000000;1.000000;1.000000;1.000000;,
  108;1.000000;1.000000;1.000000;1.000000;;
 }
}
