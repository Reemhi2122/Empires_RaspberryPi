#version 300 es
precision highp float; 
out vec4 FragColor;

in vec3 vertexColor;
in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_lightDir;
in vec3 v_fragPos;

uniform sampler2D s_texture;

void main()                                         
{
    vec3 lightColor = vec3(1,1,1);
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;  

	float diff = max(dot(normalize(v_normal), normalize(v_lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(v_fragPos);
    vec3 reflectDir = reflect(-normalize(v_lightDir), normalize(v_normal));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor; 

    float lightBrightness = 5.0;
    vec3 result = ((ambient + diffuse + specular) * vertexColor); 

	FragColor = texture2D( s_texture, v_texCoord ) * vec4(result, 1);
}