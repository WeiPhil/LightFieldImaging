#version 330 core

out vec4 fragColor;

uniform int mode;

uniform float x_pan;
uniform float y_pan;

uniform float alpha;
uniform int fstop;

uniform vec3 image_properties;

uniform sampler2DRect texture1;

in vec2 TexCoord;

void main() {
    float side = image_properties.z;
    vec2 pixel = vec2(image_properties.xy) * vec2(1 - TexCoord.x, TexCoord.y); // width,heigh * uv_coord
    vec2 pan = vec2(x_pan, y_pan);
    vec2 mod_pixel = mod(pixel, side);
    float limit = side - (side - fstop);
    float half_side = floor((side / 2));
    vec2 clamped_pan = clamp(pan - half_side, -limit + 1, limit - 1);

    if (mode == 1) {

        vec2 res = vec2(0.0);

        vec4 radiance = vec4(0.0);
        for (int u = fstop; u < (side - fstop); u++) {
            for (int v = fstop; v < (side - fstop); v++) {
                res = vec2(u, v) + clamped_pan;
                radiance += texture(texture1, pixel - mod_pixel + res);
            }
        }

        int norm_factor = (int(side - 2 * fstop));
        fragColor = radiance / (norm_factor * norm_factor);
    } else if (mode == 2) {
        vec2 shift = vec2(0.0);

        vec4 radiance = vec4(0.0);
        for (int u = fstop; u < (side - fstop); u++) {
            for (int v = fstop; v < (side - fstop); v++) {
                shift = int(alpha) * vec2(half_side - u, half_side - v);
                radiance += texture(texture1, pixel - mod_pixel + vec2(u, v) + side * shift - clamped_pan);
            }
        }

        int norm_factor = (int(side - 2 * fstop));
        fragColor = radiance / (norm_factor * norm_factor);

    } else {
        fragColor = texture(texture1, pixel - mod_pixel + pan);
    }
}