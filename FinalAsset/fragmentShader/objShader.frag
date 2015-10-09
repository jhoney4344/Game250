#version 330

const int MAX_POINT_LIGHTS = 60;
const int MAX_SPOT_LIGHTS = 5;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct BaseLight
{
	vec3  Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
	vec3  Direction;
};

struct PointLight
{
	BaseLight Base;
	vec3 Position;
	float Constant;
	float Linear;
	float Exp;
};

struct SpotLight
{
	PointLight Base;
	vec3 Direction;
	float CutOff;
};

uniform DirectionalLight gDirectionalLight;
uniform int gNumPointLights;   
uniform int gNumSpotLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];   
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gSampler;

vec4 CalculateLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)   
{
	vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                   
    float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                            
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                                                                 
                                                                                            
    if (DiffuseFactor > 0) 
	{                                                                
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;                                                                                 
    }                                                                                       
                                                                                            
    return (AmbientColor + DiffuseColor);  
}


vec4 CalculateDirectionalLight(vec3 Normal)                                                      
{                                                                                           
    return CalculateLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal); 
}    

vec4 CalculatePointLight(PointLight light, vec3 Normal)
{
	vec3 LightDirection = WorldPos0 - light.Position;                         
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
    
	if(light.Constant != 0)
	{
		vec4 Color = CalculateLightInternal(light.Base, LightDirection, Normal);       
		float Attenuation =  light.Constant +                               
							 light.Linear * Distance +                      
							 light.Exp * Distance * Distance;               
                                                                                            
		return Color / Attenuation;    
	}
	else
	{
		return vec4(0,0,0,0);
	}
}

vec4 CalculateSpotLight(SpotLight light, vec3 Normal)
{
	vec3 LightToPixel = normalize(WorldPos0 - light.Base.Position);
	float SpotFactor = dot(LightToPixel, light.Direction);
	
	if(SpotFactor > light.CutOff)
	{
		vec4 Color = CalculatePointLight(light.Base, Normal);
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - light.CutOff));
	}
	else
	{
		return vec4(0,0,0,0);
	}
}

void main()
{
   vec3 Normal = normalize(Normal0);
   vec4 TotalLight = CalculateDirectionalLight(Normal);

	for(int i = 0; i < gNumPointLights; ++i)
   {
		TotalLight += CalculatePointLight(gPointLights[i], Normal);
   }
   
   for(int i = 0; i < gNumSpotLights; ++i)
   {
		TotalLight += CalculateSpotLight(gSpotLights[i], Normal);
   }

    FragColor = texture2D(gSampler, TexCoord0.xy) * 
                TotalLight;
}