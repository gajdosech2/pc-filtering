#version 430
#include "api_frag.glsl"
#define PI 3.1415926

// IN
layout (location = 0) in vec3 in_position_eye;
layout (location = 1) in vec3 in_normal_eye;
layout (location = 2) in float in_intensity;

layout (location = 3) in vec3 in_source_color;
layout (location = 4) in float in_shininess;
layout (location = 5) in float in_ambient;
layout (location = 6) in float in_diffuse;
layout (location = 7) in float in_specular;
layout (location = 8) flat in uint in_shader;
layout (location = 9) in float in_roughness;
layout (location = 10) in float in_metalness;
layout (location = 11) in float in_shin_t;
layout (location = 12) in float in_shin_b;
layout (location = 13) flat in uint in_is_energy_conserving;

// Uniforms
layout (location = ULOC_CUSTOM_1) uniform uint in_add_intensity;

// vec3 hsv2rgb(vec3 c)
// {
//     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
// }

vec3 GetLightVector()
{
  vec3 L;
  int lid = SCENES[SCENE_ID].light_ids[0];
  vec3 light_eye = LIGHTS[lid].position.xyz;
  if (LIGHTS[lid].in_camera_space < 0.5f)
  {
    light_eye = (VIEW_MAT * LIGHTS[lid].position).xyz;
  }

  if (LIGHTS[lid].position.w > 0.5f)
  {
    L = normalize(light_eye - in_position_eye);
  }
  else
  {
    L = normalize(-light_eye);
  }
  return L;
}

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// vec3 Cook_Torrance(float D, vec3 F, float G, vec3 V, vec3)
// {

// }

void main (void)
{
  // float hi = in_label/1000.0;
  //out_color  = hsv2rgb(vec3(hi,1,1));
  // out_color = vec3(in_intensity);
  // return;
  vec3 I = vec3(1);
  vec3 color = in_source_color;

  if(in_shader == 0) // Blinn-Phong
  {
    vec3 in_position = in_position_eye;
    vec3 in_normal = in_normal_eye;

    
    float shininess = in_shininess;
    float K_ambient = in_ambient;
    float K_diffuse = in_diffuse;
    float K_specular = in_specular;

    float I_ambient = 0.1f;
    float I_diffuse = 0.0f;
    float I_specular = 0.0f;

    vec3 position_eye = vec3(0.0f);

    vec3 V = normalize(-in_position_eye);
    vec3 N = normalize(in_normal_eye);

    vec3 L = GetLightVector();

    I_diffuse += max(dot(N, L), 0.0f);  

    vec3 H = normalize((V + L)/2);
    I_specular += max(pow(dot(N, H), shininess), 0.0f);

    I = vec3(K_ambient * I_ambient + K_diffuse * I_diffuse + K_specular * I_specular);
    //I = I + in_intensity;
  }
  else if(in_shader == 1) // Cook-Torrance
  {
    //vec3 color = in_source_color;
    float K_diffuse = in_diffuse;
    float K_specular = in_specular;
    float a = in_roughness;


    vec3 N = normalize(in_normal_eye);
    vec3 V = normalize(-in_position_eye);
    vec3 L = GetLightVector();
    vec3 H = normalize((V + L)/2);

    vec3 f_Lambert = color/PI;

    float k_direct = pow((a+1),2)/8;
    float k_IBL = pow(a,2)/2;
    vec3 F0 = vec3(0.04);
    float metalness = in_metalness;
    F0 = mix(F0, color.rgb, metalness);
    float cosTheta = dot(N,V);

    vec3 DFG = DistributionGGX(N,H,a)*fresnelSchlick(cosTheta, F0)*GeometrySmith(N,V,L,k_direct);
    float denom = 4*dot(V,N)*dot(L,N);
    vec3 f_Cook_Torrance = DFG/denom;
    //float f_Cook_Torrance = DistributionGGX(N,H,a);
    //float f_Cook_Torrance = GeometrySmith(N,V,L,k_direct);
    //vec3 f_Cook_Torrance = fresnelSchlick(cosTheta, F0);

    I = K_diffuse * f_Lambert + K_specular * f_Cook_Torrance;
  }
  else if(in_shader == 2) // Oren-Nayar
  {
    //vec3 color = in_source_color;
    float a = in_roughness;

    vec3 N = normalize(in_normal_eye);
    vec3 V = normalize(-in_position_eye);
    vec3 L = GetLightVector();

    float NdotL = dot(N, L);
    float NdotV = dot(N, V); 
    float angleVN = acos(NdotL);
    float angleLN = acos(NdotV);
    float alpha = max(angleVN,angleLN);
    float beta = min(angleVN,angleLN);
    float cos_theta = dot(normalize(V-N*dot(V,N)), normalize(L-N*dot(L,N)));
    float A = 1.0 - 0.5 * (pow(a,2)/(pow(a,2)+0.33));
    float B = 0.45 * (pow(a,2)/(pow(a,2)+0.09));
    float C = sin(alpha)*tan(beta);

    I = color*NdotL*(A+B*max(0,cos_theta)*C);
  }
  else if(in_shader == 3) // Ashikhmin-Shirley
  {
    //vec3 color = in_source_color;

    vec3 N = normalize(in_normal_eye);
    vec3 V = normalize(-in_position_eye);
    vec3 L = GetLightVector();
    vec3 H = normalize((V + L)/2);

    float shin_t = in_shin_t; 
    float shin_b = in_shin_b; 

    vec3 epsilon = vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(N, epsilon));
    vec3 bitangent = normalize(cross(N, tangent));
    float NdotL = dot(N,L);

    vec3 I_diffuse = vec3(0);
    if(in_is_energy_conserving == 0){
      I_diffuse = color*NdotL;
    }
    else{
      I_diffuse = (28.0*color/23.0*PI)*(vec3(1.0) - color)*(1-pow((1-(dot(N,L)/2)),5))*(1-pow((1-(dot(N,V)/2)),5));
    }

    float A = sqrt((shin_t+1)*(shin_b+1))/(8*PI);
    float exponent_nom = shin_t*pow(dot(H,tangent),2) + shin_b*pow(dot(H,bitangent),2);
    float exponent_denom = 1-pow(dot(H,N),2);
    float exponent = exponent_nom/exponent_denom;
    float B_nom = pow(dot(N,H),exponent);
    float B_denom = dot(H,L)*max(dot(N,L),dot(N,V));
    float B = B_nom/B_denom;

    vec3 F0 = vec3(0.04);
    float cosTheta = dot(N,L);
    vec3 F = fresnelSchlick(cosTheta, F0);
    vec3 I_specular = A*B*F;

    I = I_diffuse + I_specular;
  }

  if(in_add_intensity == 1){
    I = I + in_intensity;
  }

  float r = clamp(color.r*I.r,0.0f,1.0f);
  float g = clamp(color.g*I.g,0.0f,1.0f);
  float b = clamp(color.b*I.b,0.0f,1.0f);
  
  WriteFragment(vec3(r,g,b));
}