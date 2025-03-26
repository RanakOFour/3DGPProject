varying vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_FragPos;
uniform sampler2D u_Texture;
uniform vec3 u_CameraPos;

void main()
{
    vec3 lightPos = vec3(10, 10, 10);
    vec3 specularColor = vec3(1, 1, 1);
    vec3 diffuseColor = vec3(1, 1, 1);

    vec3 viewDir = normalize(u_CameraPos - v_FragPos);
    vec3 lightDir = normalize(lightPos - v_FragPos);

    vec3 norm = normalize(v_Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseColor * diff;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * specularColor;

    vec3 lighting = diffuse + specular;
    vec4 tex = texture2D(u_Texture, v_TexCoord);
    gl_FragColor = vec4(lighting, 1) * tex;
}