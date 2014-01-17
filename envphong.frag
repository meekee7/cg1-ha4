uniform sampler2D envMap;
uniform int lighting;

varying vec3 normal; // surface nomal in eye coords
varying vec3 vertex; // vertex in eye coords

void main() { //This is our Blinn-Phong-Shader from the 3rd assignment, but instead of the object material
		if (lighting == 1) { //the environment map is used as the base colour source
			vec3 light = normalize(vec3(gl_LightSource[0].position) - vertex);
			vec3 eye = normalize(-vertex); //use -vertex because vertex is in eye space (0,0,0), direction from surface to eye is needed
			vec3 halfv = normalize(eye + light); //half vector //always add ambient light
		    vec4 evmpixcolour = texture2D(envMap, gl_TexCoord[1].st);
			gl_FragColor = evmpixcolour * (gl_LightSource[0].ambient + gl_LightModel.ambient);
			float ndotl = dot(normal, light);
			if (ndotl > 0.0) { //check light direction, add diffuse/specular light only if reflection is possible
					gl_FragColor += evmpixcolour * gl_LightSource[0].diffuse * ndotl; //add diffuse light
					float ndoth = dot(normal, halfv); 
					if (ndoth > 0.0) //Add specular light when necessary
				            gl_FragColor += evmpixcolour * gl_LightSource[0].specular * pow(ndoth, gl_FrontMaterial.shininess);
			}
			gl_FragColor *= 0.35; //set current pixel colour, but dim it
		} else //No lighting, just assign directly from the environment map
			gl_FragColor = texture2D(envMap, gl_TexCoord[1].st);
}



/* //Extracted from the sample solution
varying vec3 normal;
varying vec3 reflection;
varying vec3 view;
uniform sampler2D sphereMap;
uniform bool lighting;
uniform bool silhouette;
uniform bool moveEnvironment;
void main(){  
	vec4 color = vec4(0,0,0,1);  
	vec3 n= normalize(normal);  
	if(lighting){    
		color+= gl_LightSource[0].ambient*gl_FrontMaterial.ambient;    
		vec3 lightDirection= normalize(gl_LightSource[0].position.xyz);    
		vec3 h= normalize(gl_LightSource[0].halfVector.xyz);    
		vec4 diff= gl_FrontMaterial.diffuse;    
		float s= max(dot(n, h),0.0);   
		vec4 spec= gl_FrontMaterial.specular*pow(s,gl_FrontMaterial.shininess);  
		float nDotL= max(dot(n, lightDirection),0.0);  
		color+= nDotL*gl_LightSource[0].diffuse*(diff+spec);  
	} 
	vec3 r= normalize(reflection); 
	float p= sqrt(r.x*r.x+r.y*r.y+(1.0+r.z)*(1.0+r.z));  
	vec2 coord= vec2(r.x, r.y);  float m= sqrt(2.0*(r.z+1.0));  
	coord/= m;  
	coord*= 0.5;  
	coord+= 0.5;  
	if(r.z < -1.0) 
		color= vec4(0,0,0,1); 
	else if(lighting) 
		color*= texture2D(sphereMap, coord); 
	else    
		color= texture2D(sphereMap, coord); 
		gl_FragColor= color;
} */