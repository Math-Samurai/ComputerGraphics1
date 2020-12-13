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

vec3 ambient()
{
    const float ambientStrength = 0.6;
    return ambientStrength * lightColor;
}

vec3 specular(vec3 Normal)
{
    vec3 lightDir   = normalize(lightPos - Position);
    vec3 viewDir    = normalize(cameraPos - Position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 1000.0f);
    return lightColor * spec;
}

vec3 diffuse(vec3 Normal)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * lightColor;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    const int numLayers = 100;
    float layerDepth = 1.0f / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * 0.05f; 
    vec2 deltaTexCoords = P / numLayers;

    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthTex, currentTexCoords).r;
  
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthTex, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }

    deltaTexCoords /= 2;
    layerDepth /= 2;
    currentTexCoords += deltaTexCoords;
    currentLayerDepth -= layerDepth;
    for (int i = 0; i < 5; ++i)
    {
        currentDepthMapValue = texture(depthTex, currentTexCoords).r;
        deltaTexCoords /= 2;
        layerDepth /= 2;
        if (currentDepthMapValue > currentLayerDepth)
        {
            currentTexCoords -= deltaTexCoords;
            currentLayerDepth += layerDepth;
        } else
        {
            currentTexCoords += deltaTexCoords;
            currentLayerDepth -= layerDepth;
        }
    }

    return currentTexCoords; 
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