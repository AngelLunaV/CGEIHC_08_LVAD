#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    vec3 specular; // O vec3 specular para iluminacion plana
    float shininess;
};
struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // ambient
    vec3 ambient  = light.ambient * vec3(texture (texture_diffuse1, TexCoords));
    //vec3 ambient  = light.ambient * material.ambient; //(cuando hay material.ambient es vec3)
    //vec3 ambient  = light.ambient * vec3 (1.0, 1.0, 1.0);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * diff * vec3(texture (texture_diffuse1, TexCoords));
    //vec3 diffuse  = light.diffuse * (diff * material.diffuse); //(cuando material.diffuse es vec3)
    //vec3 diffuse  = light.diffuse * (diff * vec3 (1.0, 1.0, 1.0));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * spec * vec3(texture (material.specular, TexCoords));
    vec3 specular = light.specular * (spec * material.specular); //(cuando material.specular es vec3)
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}