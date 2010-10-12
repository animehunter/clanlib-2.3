uniform sampler2D BaseTexture;
uniform float BaseIntensity;
uniform float BaseSaturation;

uniform sampler2D BloomTexture;
uniform float BloomIntensity;
uniform float BloomSaturation;
varying vec2 TexCoord;

vec4 AdjustSaturation(in vec4 color, in float saturation)
{
	float grey = dot(color, vec4(vec3(0.3, 0.59, 0.11), 0.0));
	vec4 grey_color = vec4(grey, grey, grey, 0.0);
	
	return mix(grey_color, color, saturation);
}

vec4 BloomCombine()
{
    vec4 bloom = texture2D(BloomTexture, TexCoord);
    bloom = (AdjustSaturation(bloom, BloomSaturation) * BloomIntensity);

    vec4 base = texture2D(BaseTexture, TexCoord);
    base = (AdjustSaturation(base, BaseSaturation) * BaseIntensity);
    
    base *= (1.0 - clamp(bloom, 0.0, 1.0));
    
    return (base + bloom);
}

void main() 
{
    gl_FragColor = BloomCombine();
}
