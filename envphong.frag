uniform sampler2D envMap;

void main (void) {
		gl_FragColor = texture2D(envMap, gl_TexCoord[1].st);
}