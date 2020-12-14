#version 330 core

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
in mat3 TBN;

out vec4 color;
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D depthTex;
uniform vec3 cameraPos;
uniform int id;

vec3 lightPos = {0.0f, 0.0f, -11.0f};
vec3 lightColor = {1.0f, 1.0f, 1.0f};

vec3 ambient(void)
{
    return 0.6f * lightColor;
}

vec3 specular(vec3 Normal)
{
    Normal = normalize(Normal);
    vec3 viewVec = normalize(cameraPos - Position);  
    vec3 lightVec = normalize(lightPos - Position);  
    vec3 medianVec = normalize(viewVec + lightVec);
    float res = pow(max(dot(Normal, medianVec), 0.0), 1000.0f);
    return lightColor * res;
}

vec3 diffuse(vec3 Normal)
{
    Normal = normalize(Normal);
    vec3 lightVec = normalize(lightPos - Position);
    float res = max(dot(Normal, lightVec), 0.0);
    return lightColor * res;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    const int layersCount = 100;
    float depthOfLayer = 1.0f / layersCount;
    float currentDepth = 0.0;
    vec2 res = viewDir.xy * 0.05f; 
    vec2 deltaTexCoords = res / layersCount;
    vec2  currentCoords = texCoords;
    float currentDepthMap = texture(depthTex, currentCoords).r;
    while(currentDepthMap > currentDepth)
    {
        currentCoords -= deltaTexCoords;
        currentDepthMap = texture(depthTex, currentCoords).r;  
        currentDepth += depthOfLayer;  
    }
    deltaTexCoords /= 2;
    depthOfLayer /= 2;
    currentDepth -= depthOfLayer;
    currentCoords += deltaTexCoords;
    for (int i = 0; i < 10; ++i)
    {
        currentDepthMap = texture(depthTex, currentCoords).r;
        deltaTexCoords /= 2;
        depthOfLayer /= 2;
        if (currentDepth < currentDepthMap)
        {
            currentCoords -= deltaTexCoords;
            currentDepth += depthOfLayer;
        } else
        {
            currentCoords += deltaTexCoords;
            currentDepth -= depthOfLayer;
        }
    }
    return currentCoords; 
}

void main()
{
    if (id == 0) {
        vec3 norm = normalize(Normal);
        color = vec4(ambient() + diffuse(norm) + specular(norm), 1.0f) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
    } else
    {
        vec3 viewDir = normalize(TBN * cameraPos - TBN * Position);
        vec2 texCoord = ParallaxMapping(TexCoord, viewDir);
        vec3 norm = normalize((texture(normalTex, texCoord).rgb - 0.5f) * 2.0f);
        norm = normalize(TBN * norm);
        color = vec4(ambient() + diffuse(norm) + specular(norm), 1.0f) * texture(diffuseTex, texCoord);
    }
}