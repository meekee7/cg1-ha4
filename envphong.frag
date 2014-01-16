uniform sampler2D envMap;

void main() {
		gl_FragColor = texture2D(envMap, gl_TexCoord[1].st);
}