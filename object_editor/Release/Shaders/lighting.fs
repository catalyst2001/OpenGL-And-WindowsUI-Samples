#version 120

uniform vec3 CameraPosition;

varying vec3 var_Position, var_Normal;

void main()
{
	gl_FragColor = gl_Color;

	vec3 LightDirection = normalize(CameraPosition - var_Position);

	vec3 Normal = normalize(var_Normal);

	float NdotLD = max(dot(Normal, LightDirection), 0.0);

	gl_FragColor.rgb *= 0.25 + 0.75 * NdotLD;
}
