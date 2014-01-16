uniform float envrotate;

void main() {
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_TexCoord[0] = gl_MultiTexCoord0;
        vec3 r = reflect(normalize(vec3(gl_ModelViewMatrix * gl_Vertex)), normalize(gl_NormalMatrix * gl_Normal));
		r.z += 1.0;
		float m = 2.0 * length(r);
		gl_TexCoord[1].s = r.x / m + 0.5;
        gl_TexCoord[1].t = r.y / m + 0.5;
		if (envrotate == 1.0)
			gl_TexCoord[1] = gl_ModelViewMatrix * gl_TexCoord[1];
}