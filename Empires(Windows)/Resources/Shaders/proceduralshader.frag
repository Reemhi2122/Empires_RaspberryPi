#version 330 core
precision highp float;     

out vec4 FragColor;
in vec2 v_texCoord;    
in vec3 v_faction_color;
in vec4 v_normal;
in vec3 v_lightDir;
in vec3 v_fragPos;

uniform sampler2D s_texture;                        
uniform vec4  Ambient;
uniform vec3 v_LandColor;
uniform vec3 v_Water_Color;

uniform uint u_TexSeed;
uniform uint u_TexHelpSeed;
uniform uint u_NoiseScale;


// simple conversion of the linear interpolation function to
// interpolating between vec3 (lerp is called mix in GLSL, thus the name)
vec3 mixVec3(in vec3 vec_a, in vec3 vec_b, in float time)
{
	return (1.0 - time) * vec_a + time * vec_b;
};


 //this function was taken from http://science-and-fiction.org/rendering/noise.html#perlin_noise
float rand3D(in vec3 co){
    return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}

// this is the same function as above, just adjusted to 2D
float rand2D(in vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// fallback function if one of the colors is black
vec3 RandColor(uint color_seed)
{
    float r = rand2D(vec2(u_TexSeed, color_seed));
    float g = rand2D(vec2(color_seed, u_TexSeed));
    float b = rand2D(vec2(rand2D(vec2(u_TexSeed, color_seed)), rand2D(vec2(color_seed, u_TexSeed))));

    return vec3(r, g, b);
}

// taken from https://docs.unity3d.com/Packages/com.unity.shadergraph@7.1/manual/Simple-Noise-Node.html
// and adapted to using a seedable random generator
float value_noise(in vec2 uv)
{	
	vec2 i = floor(uv);
	vec2 f = fract(uv);

	f = f * f * (3.0 - 2.0 * f);

    uv = abs(fract(uv) - 0.5);
    vec2 c0 = i + vec2(0.0, 0.0);
    vec2 c1 = i + vec2(1.0, 0.0);
    vec2 c2 = i + vec2(0.0, 1.0);
    vec2 c3 = i + vec2(1.0, 1.0);
    float r0 = rand3D(vec3(c0, u_TexSeed));
    float r1 = rand3D(vec3(c1, u_TexSeed));
    float r2 = rand3D(vec3(c2, u_TexSeed));
    float r3 = rand3D(vec3(c3, u_TexSeed));

	float bottomOfGrid = mix(r0, r1, f.x);
    float topOfGrid = mix(r2, r3, f.x);
    float t = mix(bottomOfGrid, topOfGrid, f.y);
    return t;
}

// taken from https://docs.unity3d.com/Packages/com.unity.shadergraph@7.1/manual/Simple-Noise-Node.html
float simple_noise(in vec2 uv, in float scale)
{
	float t = 0.0;

    float freq = pow(2.0, float(0));
    float amp = pow(0.5, float(3-0));
    t += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;

    freq = pow(2.0, float(1));
    amp = pow(0.5, float(3-1));
    t += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;

    freq = pow(2.0, float(2));
    amp = pow(0.5, float(3-2));
    t += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;

    return t;
}

void main()                                         
{    
    // generating noise color
	float noise = simple_noise(v_texCoord, u_NoiseScale);
	vec3 color;
    vec3 color2;
    vec3 output_color;

	float water = 0.45;
    float mountain_tops = 0.7;

	color = v_Water_Color;
    color2 = v_LandColor;

    // fallback if color is black
    if (color == vec3(0., 0., 0.))
    {
        color = RandColor(u_TexHelpSeed);
    }

    if (color2 == vec3(0., 0., 0.))
    {
        color = RandColor(u_TexHelpSeed / uint(2));
    }

    // the idea for interpolating the colors like this was 
    // introduced to me by Tom van het Ende (200640)
	if(noise < water)
	{
        output_color = mixVec3(color/4, color, noise/water);
	} else output_color = mixVec3(color2/4, color2, noise/1-water);

    if(noise > mountain_tops)
    {
        output_color = mixVec3(color2, vec3(0.9, 0.9, 0.9), noise);
    }

    // lighting
    float lightBrightness = 5;

    vec3 lightColor = vec3(1,1,1);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;  

	float diff = max(dot(vec3(v_normal.x, v_normal.y, v_normal.z), v_lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1;
    vec3 viewDir = normalize(v_fragPos);
    vec3 reflectDir = reflect(-v_lightDir, vec3(v_normal.x, v_normal.y, v_normal.z));  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 result = ((ambient + diffuse + specular) * v_faction_color) * lightBrightness; 

	FragColor = vec4(output_color, 0.5) * vec4(result, 1);
}   