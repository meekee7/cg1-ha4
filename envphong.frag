uniform sampler2D envMap;

varying vec3 normal; // surface nomal in eye coords
varying vec3 vertex; // vertex in eye coords

void main() {
		vec3 light = normalize(vec3(gl_LightSource[0].position) - vertex);
        vec3 eye = normalize(-vertex); //use -vertex because vertex is in eye space (0,0,0), direction from surface to eye is needed
        vec3 halfv = normalize(eye + light); //half vector //always add ambient light
        vec4 colpix = texture2D(envMap, gl_TexCoord[1].st);
		vec4 colour = colpix * (gl_LightSource[0].ambient + gl_LightModel.ambient);
		float ndotl = dot(normal, light);
		if (ndotl > 0.0) { //check light direction, add diffuse/specular light only if reflection is possible
				colour += colpix * gl_LightSource[0].diffuse * ndotl; //add diffuse light
                float ndoth = dot(normal, halfv); 
                if (ndoth > 0.0) //Add specular light when necessary
                        colour += colpix * gl_LightSource[0].specular * pow(ndoth, gl_FrontMaterial.shininess);
        }
        gl_FragColor = colour * 0.35; //set current pixel colour
		
		//gl_FragColor = texture2D(envMap, gl_TexCoord[1].st);
}