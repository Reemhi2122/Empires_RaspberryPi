#version 330 core
precision mediump float; 
out vec4 FragColor;

in vec3 vertexColor;
in vec4 v_normal;
in vec2 v_texCoord;
in vec3 v_lightDir;
in vec3 v_fragPos;

uniform sampler2D s_texture;

void main()                                         
{
    vec3 lightColor = vec3(1,1,1);
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;  

	float diff = max(dot(vec3(v_normal.x, v_normal.y, v_normal.z), v_lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1;
    vec3 viewDir = normalize(v_fragPos);
    vec3 reflectDir = reflect(-v_lightDir, vec3(v_normal.x, v_normal.y, v_normal.z));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 result = (ambient + diffuse + specular) * vertexColor; 

	FragColor = texture2D( s_texture, v_texCoord ) * vec4(result, 1);
}