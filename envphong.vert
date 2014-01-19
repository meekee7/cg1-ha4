#version 140
uniform int envrotate;
uniform mat4 rotatemat;
varying vec3 normal;
varying vec3 vertex;

void main() {
        normal = normalize(gl_NormalMatrix * gl_Normal); //transform normal into eye space
        vertex = vec3(gl_ModelViewMatrix * gl_Vertex); //set vertex position in eye space
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; //set correct position

		gl_TexCoord[0] = gl_MultiTexCoord0;
        vec3 r;
		if (envrotate == 0)
			r = reflect(normalize(vec3((gl_ModelViewMatrix) * gl_Vertex)), normalize((gl_NormalMatrix) * gl_Normal));
		else 
			r = reflect(normalize(vec3((gl_ModelViewMatrix * rotatemat) * gl_Vertex)), normalize(mat3(inverse(transpose(gl_ModelViewMatrix * rotatemat))) * gl_Normal));
		r.z += 1.0;
		float m = 2.0 * length(r);
		gl_TexCoord[1].s = r.x / m + 0.5;
        gl_TexCoord[1].t = r.y / m + 0.5;
}


/* //Extracted from the sample solution
varying vec3 normal;
varying vec3 reflection;
uniform mat4 cameraMatrix;
uniform mat3 cameraNormalMatrix;
uniform mat4 mirrorMatrix;
uniform mat3 mirrorNormalMatrix;
uniform bool moveEnvironment;
void main(){
	if(!moveEnvironment){  
		vec3 view= normalize(gl_ModelViewMatrix*gl_Vertex).xyz;  
		normal= normalize(gl_NormalMatrix*gl_Normal);  
		reflection= reflect(view, normal);  
		gl_Position= gl_ModelViewProjectionMatrix*gl_Vertex; 
	} else{   
		vec3 n= normalize((mirrorNormalMatrix)*gl_Normal);   
		vec3 v= normalize(((mirrorMatrix)*gl_Vertex).xyz);   
		reflection= reflect(v, n);   
		gl_Position= gl_ProjectionMatrix*cameraMatrix*gl_Vertex;   
		normal= normalize(cameraNormalMatrix*gl_Normal); 
	}
}*/