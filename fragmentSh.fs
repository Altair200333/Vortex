#version 330 core
out vec4 color;
  
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 objectColor;
uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
struct Light {
	int type;
    vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform Light light; 
uniform Material material;

void main()
{    
    // ambient
	vec3 ambient  = light.ambient * material.ambient;

    // diffuse 
    vec3 norm = normalize(Normal);
	vec3 lightDir;
	float distance    = 1;
	float attenuation = 1;

	if(light.type == 2)
	{
		vec3 ambient = light.ambient * material.ambient;
    
		// diffuse 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * material.diffuse;  
		
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * material.specular;  
		
		// spotlight (soft edges)
		float theta = dot(lightDir, normalize(-light.direction)); 
		float epsilon = (light.cutOff - light.outerCutOff);
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		diffuse  *= intensity;
		specular *= intensity;
		
		// attenuation
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
		ambient  *= attenuation; 
		diffuse   *= attenuation;
		specular *= attenuation;   
			
		vec3 result = ambient + diffuse + specular;
		color = vec4(result, 1.0);
		
	}
	else
	{
		if(light.type==1)
		{
			lightDir = normalize(light.position - FragPos);
			distance    = length(light.position - FragPos);
			attenuation = 1.0 / (light.constant + light.linear * distance + 
					light.quadratic * (distance * distance));
		}
		else
		{
			lightDir = normalize(-light.direction);
		}

		
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse  = light.diffuse * (diff * material.diffuse);

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * (spec * material.specular);  

		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular;
		color = vec4(result, 1.0);
	}
	
}